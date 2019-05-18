#ifndef QTLOCALPEER_H
#define QTLOCALPEER_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QDir>
#include "qtlockedfile.h"

class QtLocalPeer:public QObject
{
    Q_OBJECT
public:
    explicit QtLocalPeer(QObject *parent = 0,const QString &appId=QString());
    bool isClient();
    bool sendMessage(const QString &message,int timeout,bool block);
    QString applicationId()const{
        return id;
    }
    static QString appSessionId(const QString &appId);

Q_SIGNALS:
    void messageReceived(const QString &message,QObject *socket);

protected Q_SLOTS:
    void receiveConnection();

protected:
    QString id;
    QString socketName;
    QLocalServer *server;
    QtLockedFile lockFile;
};

#endif // QTLOCALPEER_H
