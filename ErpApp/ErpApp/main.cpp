#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTime>
#include <QDebug>
#include <QFileInfo>
#include <unistd.h>
#include <direct.h>
#include <QMessageBox>
#include <string>
#include "gfunctions.h"
#include "SqlManager.h"

using std::string;

bool initializeEnv()
{
    QString xAppPath = getAppPath();

    //  create db directory
    QString xDBDirectory = xAppPath + "/db";
    string xStrDBDirectory = xDBDirectory.toStdString();

    if(0 != access(xStrDBDirectory.c_str(), 0))
    {
        if(0 != mkdir(xStrDBDirectory.c_str()))
        {
            qDebug() << "cannot create director " << xStrDBDirectory.c_str();
            return false;
        }
    }

    return true;
}

bool initializeSql()
{
    QString xAppPath = getAppPath();

    //  create db directory
    QString xDBDirectory = xAppPath + "/db/product.db";

    if(!SqlManager::getInstance()->connect(xDBDirectory))
    {
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool bInitialized = false;

    //  splash window
    QSplashScreen xSplashWnd(QPixmap(":/image/splash.png"));
    xSplashWnd.show();

    Qt::Alignment eAlign = Qt::AlignRight | Qt::AlignTop;
    xSplashWnd.showMessage(QStringLiteral("正在初始化程序..."), eAlign);
    internalSleep(1000);
    bInitialized = initializeEnv();
    if(!bInitialized)
    {
        QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("初始化程序失败"));
        return -1;
    }

    xSplashWnd.showMessage(QStringLiteral("正在初始化数据库..."), eAlign);
    internalSleep(1000);

    bInitialized = initializeSql();
    if(!bInitialized)
    {
        QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("初始化数据库失败"));
        return -1;
    }

    xSplashWnd.showMessage(QStringLiteral("正在启动程序..."), eAlign);
    internalSleep(1000);

    //  main window
    xSplashWnd.hide();
    MainWindow w;
    w.show();

    return a.exec();
}
