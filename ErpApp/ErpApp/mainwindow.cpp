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
#include <QMessageBox>

#include "newskudlg.h"
#include "newproductdlg.h"
#include "mainwidget.h"

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

    pToolBar = addToolBar(QStringLiteral("浏览"));
    pAction = pToolBar->addAction(QStringLiteral("上一页"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionPrevPage()));
    pAction = pToolBar->addAction(QStringLiteral("下一页"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionNextPage()));

    m_pMainWidget = new MainWidget(this);
    setCentralWidget(m_pMainWidget);
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
        QStringList xCategorys = skudlg.getCategory();
        prodlg.setCategory(xCategorys[0], xCategorys[1], xCategorys[2]);

        int nCategorySeq = skudlg.getCategorySeq();
        if(nCategorySeq <= 0)
        {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("参数错误，无法进行下一步"));
            return;
        }

        prodlg.setCategorySeq(nCategorySeq);
        prodlg.exec();

        if(prodlg.getContentModified())
        {
            m_pMainWidget->updatePage();
        }
    }
}

void MainWindow::onActionPrevPage()
{

}

void MainWindow::onActionNextPage()
{

}
