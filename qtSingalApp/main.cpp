#include "mainwindow.h"
#include <QApplication>
#include "qtsingleapplication.h"
#include <QMessageBox>

const char appNameC[] = "Qt Creator";

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
    QtSingleApplication a(appNameC,argc,argv);
    MainWindow w;
    w.show();
    if(a.isRunning())
    {
        QMessageBox::warning(&w,QObject::tr("Warning"),QObject::tr("The app is running..."));
        return 0;
    }
    return a.exec();
}
