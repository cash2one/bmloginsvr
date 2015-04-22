#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDebug>

#include "newskudlg.h"
#include "newproductdlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::createWidgets()
{
    QToolBar* pToolBar = addToolBar(QStringLiteral("添加商品"));
    QAction* pAction = pToolBar->addAction(QIcon(":/image/button_add.png"), QStringLiteral("添加商品"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionNewProduct()));
}

//  slots
void MainWindow::onActionNewProduct()
{
    NewSKUDlg skudlg;
    skudlg.setWindowTitle(QStringLiteral("添加商品"));
    int nRet = skudlg.exec();

    if(nRet == QDialog::Accepted)
    {
        qDebug() << QStringLiteral("产生创建对话框");
        NewProductDlg prodlg;
        prodlg.setSKUCode(skudlg.getSkuNumber());
        prodlg.exec();
    }
}
