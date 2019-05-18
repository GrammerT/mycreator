#ifndef QTSINGLEAPPLICATION_H
#define QTSINGLEAPPLICATION_H

#include <QObject>
#include <QApplication>

QT_FORWARD_DECLARE_CLASS(QSharedMemory)

class QtLocalPeer;

class QtSingleApplication :public QApplication
{
    Q_OBJECT
public:
    QtSingleApplication(const QString &id,int &argc,char **argv);
    ~QtSingleApplication();

    bool isRunning(qint64 pid = -1);

    void setActiveWindow(QWidget *aw,bool activateOnMessage = true);
    QWidget *activeWindow() const;
    bool event(QEvent *event);
    QString applicationId() const ;
    void setBlock(bool value);

public Q_SLOTS:
    bool sendMessage(const QString &message,int timeout = 5000,qint64 pid = -1);
    void activeWindow();

Q_SIGNALS:
    void messageReceived(const QString &message,QObject *socket);
    void fileOpenRequest(const QString &file);

private:
    QString instanceFileName(const QString &appId);

    qint64 firstPeer;
    QSharedMemory *instance;
    QtLocalPeer *pidPeer;
    QWidget *actWin;
    QString appId;
    bool block;
};

#endif // QTSINGLEAPPLICATION_H
