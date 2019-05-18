#include "qtlocalpeer.h"
#include <QCoreApplication>
#include <QDataStream>
#include <QTime>
#include <QLibrary>
#include <qt_windows.h>

typedef BOOL(WINAPI*PProcessIdToSessionId)(DWORD,DWORD*);
static PProcessIdToSessionId pProcessIdToSessionId = 0;

static const char ack[] = "ack";

QtLocalPeer::QtLocalPeer(QObject *parent, const QString &appId)
    :QObject(parent),id(appId)
{
    if(id.isEmpty())
    {
        id=QCoreApplication::applicationFilePath();
    }

    socketName = appSessionId(id);
    server = new QLocalServer(this);
    QString lockName = QDir(QDir::tempPath()).absolutePath()+QLatin1Char('/')
            +socketName+QLatin1String("-lockfile");
    lockFile.setFileName(lockName);
    lockFile.open(QIODevice::ReadWrite);

}

bool QtLocalPeer::isClient()
{
    if(lockFile.isLocked())
        return false;
    if(!lockFile.lock(QtLockedFile::WriteLock,false))
    {
        return true;
    }
    if(!QLocalServer::removeServer(socketName))
    {
        qDebug("QtSingleCoreApplication: could not cleanup socket");
    }
    bool res = server->listen(socketName);
    if(!res)
    {
        qDebug("QtSingleCoreApplication: listen on local socket failed, %s", qPrintable(server->errorString()));
    }
    QObject::connect(server,SIGNAL(newConnection()),SLOT(receiveConnection()));
    return false;
}

bool QtLocalPeer::sendMessage(const QString &message, int timeout, bool block)
{
    if(!isClient())
        return false;
    QLocalSocket socket;
    bool connOK = false;
    for(int i=0;i<2;++i)
    {
        socket.connectToServer(socketName);
        connOK = socket.waitForConnected(timeout/2);
        if(connOK||i)
        {
            break;
        }
        int ms=250;
        Sleep(DWORD(ms));
    }
    if(!connOK)
        return false;
    QByteArray uMsg(message.toUtf8());
    QDataStream ds(&socket);
    ds.writeBytes(uMsg.constData(),uMsg.size());
    bool res=socket.waitForBytesWritten(timeout);
    res&=socket.waitForReadyRead(timeout);
    res&=(socket.read(qstrlen(ack))==ack);
    if(block)
        socket.waitForDisconnected(-1);
    return res;
}

QString QtLocalPeer::appSessionId(const QString &appId)
{
    QByteArray idc = appId.toUtf8();
    quint16 idNum = qChecksum(idc.constData(),idc.size());

    QString res = QLatin1String("qtsingleapplication-")
            + QString::number(idNum, 16);
    if(!pProcessIdToSessionId)
    {
        QLibrary lib(QLatin1String("kernel32"));
        pProcessIdToSessionId = (PProcessIdToSessionId)lib.resolve("ProcessIdToSessionId");
    }
    if(pProcessIdToSessionId)
    {
        DWORD sessionId = 0;
        pProcessIdToSessionId(GetCurrentProcessId(),&sessionId);
        res+=QLatin1Char('-')+QString::number(sessionId,16);
    }
    return res;
}

void QtLocalPeer::receiveConnection()
{
    QLocalSocket *socket = server->nextPendingConnection();
    if(!socket)
    {
        return ;
    }
    while(socket->bytesAvailable()<static_cast<int>(sizeof(quint32)))
    {
        if(!socket->isValid())
            return ;
        socket->waitForReadyRead(1000);
    }
    QDataStream ds(socket);
    QByteArray uMsg;
    quint32 remaining;
    ds>>remaining;
    uMsg.resize(remaining);
    int got = 0;
    char *uMsgBuf = uMsg.data();
    do
    {
        got=ds.readRawData(uMsgBuf,remaining);
        remaining-=got;//! if remianing=16 ,already read got=9 byte. then remaining = 7 byte
        uMsgBuf += got;
    }while(remaining&&got>=0&&socket->waitForReadyRead(2000));

    if(got<0)
    {
        qDebug()<<"QtLocalPeer: Message reception failed" << socket->errorString();
        delete socket;
        return ;
    }
    QString message = QString::fromUtf8(uMsg.constData(),uMsg.size());
    socket->write(ack,qstrlen(ack));
    socket->waitForBytesWritten(1000);
    emit messageReceived(message,socket);
}
