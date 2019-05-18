#include "qtsingleapplication.h"
#include "qtlocalpeer.h"
#include "qtlockedfile.h"
#include <QSharedMemory>
#include <QWidget>
#include <QDir>
#include <QFileOpenEvent>

static const int instancesSize = 1024;

static QString instancesLockFilename(const QString &appSessionId)
{
    const QChar slash(QLatin1Char('/'));
    QString res = QDir::tempPath();
    if (!res.endsWith(slash))
        res += slash;
    return res + appSessionId + QLatin1String("-instances");
}

QtSingleApplication::QtSingleApplication(const QString &id, int &argc, char **argv)
    :QApplication(argc,argv)
    ,firstPeer(-1)
    ,pidPeer(0)
{
    this->appId = id;
    const QString appSessionId = QtLocalPeer::appSessionId(appId);

    instance = new QSharedMemory(appSessionId,this);
    actWin = 0;
    block = false;

    const bool created = instance->create(instancesSize);
    if(!created)
    {
        if(!instance->attach())
        {
            qDebug()<<"Failed to initialize instances shared memory: "
                   << instance->errorString();
            delete instance;
            instance = 0;
            return ;
        }
    }

    QtLockedFile lockfile(instancesLockFilename(appSessionId));

    lockfile.open(QtLockedFile::ReadWrite);
    lockfile.lock(QtLockedFile::WriteLock);

    qint64 *pids = static_cast<qint64*>(instance->data());
    if(!created)
    {
        for(;*pids;++pids){
            if(firstPeer==-1&&isRunning(*pids))
            {
                firstPeer=*pids;
            }
        }
    }
    *pids++=QCoreApplication::applicationPid();
    *pids=0;
    pidPeer = new QtLocalPeer(this,appId+ QLatin1Char('-') +
                              QString::number(QCoreApplication::applicationPid()));
    connect(pidPeer,SIGNAL(messageReceived(QString,QObject*)),SIGNAL(messageReceived(QString,QObject*)));
    pidPeer->isClient();
    lockfile.unlock();

}

QtSingleApplication::~QtSingleApplication()
{
    if(!instance)
        return ;
    const qint64 appPid = QCoreApplication::applicationPid();
    QtLockedFile lockFile(instancesLockFilename(QtLocalPeer::appSessionId(appId)));
    lockFile.open(QtLockedFile::ReadWrite);
    lockFile.lock(QtLockedFile::WriteLock);
    qint64 *pids = static_cast<qint64*>(instance->data());

    qint64 *newpids = pids;
    for(;*pids;++pids)
    {
        if(*pids!=appPid&&isRunning(*pids))
        {
            *newpids++=*pids;
        }
    }
    *newpids = 0;

    lockFile.unlock();
}

bool QtSingleApplication::isRunning(qint64 pid)
{
    if(pid==-1)
    {
        pid=firstPeer;
        if(pid==-1)
            return false;
    }
    QtLocalPeer peer(this,appId+ QLatin1Char('-') + QString::number(pid, 10));

    return peer.isClient();

}

void QtSingleApplication::setActiveWindow(QWidget *aw, bool activateOnMessage)
{
    actWin = aw;
    if(!pidPeer)
        return ;
    if(activateOnMessage)
    {
        connect(pidPeer,SIGNAL(messageReceived(QString,QObject*)),this,SLOT(activeWindow()));
    }
    else
    {
        disconnect(pidPeer,SIGNAL(messageReceived(QString,QObject*)),this,SLOT(activeWindow()));
    }
}

QWidget *QtSingleApplication::activeWindow() const
{
    return actWin;

}

bool QtSingleApplication::event(QEvent *event)
{
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *foe = static_cast<QFileOpenEvent*>(event);
        emit fileOpenRequest(foe->file());
        return true;
    }
    return QApplication::event(event);
}

QString QtSingleApplication::applicationId() const
{
    return appId;
}

void QtSingleApplication::setBlock(bool value)
{
    block=value;
}

bool QtSingleApplication::sendMessage(const QString &message, int timeout, qint64 pid)
{
    if (pid == -1) {
        pid = firstPeer;
        if (pid == -1)
            return false;
    }

    QtLocalPeer peer(this, appId + QLatin1Char('-') + QString::number(pid, 10));
    return peer.sendMessage(message, timeout, block);
}

void QtSingleApplication::activeWindow()
{
    if(actWin)
    {
        actWin->setWindowState(actWin->windowState()&~Qt::WindowMinimized);
        actWin->raise();
        actWin->activateWindow();
    }
}

QString QtSingleApplication::instanceFileName(const QString &appId)
{
    return "";
}
