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
    QAction* pAction = pToolBar->addAction(QStringLiteral("添加商品"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionNewProduct()));
}

//  slots
void MainWindow::onActionNewProduct()
{
    NewSKUDlg dlg;
    dlg.setWindowTitle(QStringLiteral("添加商品"));
    dlg.exec();
}
