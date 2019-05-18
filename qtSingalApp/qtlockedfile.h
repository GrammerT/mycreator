#ifndef QTLOCKEDFILE_H
#define QTLOCKEDFILE_H
#include <QFile>

class QtLockedFile :public QFile
{
public:
    enum LockMode {
        NoLock = 0,
        ReadLock,
        WriteLock
    };

    QtLockedFile();
    QtLockedFile(const QString &name);
    ~QtLockedFile();

    bool lock(LockMode mode,bool lock = true);
    bool unlock();
    bool isLocked()const;
    LockMode lockMode()const;

private:
    Qt::HANDLE m_semaphore_hnd;
    Qt::HANDLE m_mutex_hnd;
    LockMode m_lock_mode;

};

#endif // QTLOCKEDFILE_H
