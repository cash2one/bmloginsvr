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
#include <QTableWidget>
#include <QTableWidgetItem>

#include "newskudlg.h"
#include "newproductdlg.h"
#include "mainwidget.h"
#include "SqlManager.h"
#include "finddlg.h"
#include "CategoryManager.h"
#include "categorydlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createWidgets();
    createMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::createWidgets()
{
    ui->mainToolBar->setWindowTitle(QStringLiteral("商品操作"));
    QToolBar* pToolBar = ui->mainToolBar;
    //QAction* pAction = pToolBar->addAction(QIcon(":/image/button_add.png"), QStringLiteral("添加商品"));
    QAction* pAction = pToolBar->addAction(QStringLiteral("添加商品"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionNewProduct()));
    pAction = pToolBar->addAction(QStringLiteral("修改商品"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionModifyProduct()));
    pAction = pToolBar->addAction(QStringLiteral("删除商品"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionDelete()));
    pAction = pToolBar->addAction(QStringLiteral("查找商品"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionFind()));
    pAction = pToolBar->addAction(QStringLiteral("显示全部"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionShowAll()));
    pToolBar->addSeparator();
    pAction = pToolBar->addAction(QStringLiteral("编辑目录"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionEditCategory()));

    pToolBar = addToolBar(QStringLiteral("浏览"));
    pAction = pToolBar->addAction(QStringLiteral("上一页"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionPrevPage()));
    pAction = pToolBar->addAction(QStringLiteral("下一页"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onActionNextPage()));

    m_pMainWidget = new MainWidget(this);
    setCentralWidget(m_pMainWidget);
    QTableWidget* pTable = m_pMainWidget->getTableWidget();
    pTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableMenu(QPoint)));

    setMinimumWidth(800);
    setMinimumHeight(600);
}

void MainWindow::createMenu()
{
    m_xTableMenu.addAction(QStringLiteral("添加商品"), this, SLOT(onActionNewProduct()));
    m_xTableMenu.addAction(QStringLiteral("删除商品"), this, SLOT(onActionDelete()));
    m_xTableMenu.addAction(QStringLiteral("编辑商品"), this, SLOT(onActionModifyProduct()));
    m_xTableMenu.addAction(QStringLiteral("查找商品"), this, SLOT(onActionFind()));
    m_xTableMenu.addAction(QStringLiteral("显示全部"), this, SLOT(onActionShowAll()));
    m_xTableMenu.addSeparator();
    m_xTableMenu.addAction(QStringLiteral("上一页"), this, SLOT(onActionPrevPage()));
    m_xTableMenu.addAction(QStringLiteral("下一页"), this, SLOT(onActionNextPage()));
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

void MainWindow::onActionModifyProduct()
{
    QString xSelectSKUCode = m_pMainWidget->getCurrentSelectSKUCode();
    if(xSelectSKUCode.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择有效的行"));
        return;
    }

    ProductItem item;
    if(!SqlManager::getInstance()->getProductItem(item, xSelectSKUCode))
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("获取原始信息失败"));
        return;
    }

    NewProductDlg prodlg;
    prodlg.setSKUCode(xSelectSKUCode);
    prodlg.setCategory(item.xFstCategory, item.xSecCategory, item.xTrdCategory);
    prodlg.setCategorySeq(item.nSeq);
    prodlg.exec();

    if(prodlg.getContentModified())
    {
        m_pMainWidget->updatePage();
    }
}

void MainWindow::onActionPrevPage()
{
    m_pMainWidget->PrevPage();
}

void MainWindow::onActionNextPage()
{
    m_pMainWidget->NextPage();
}

void MainWindow::onActionFind()
{
    FindDlg dlg;
    if(QDialog::Accepted == dlg.exec())
    {
        //  find...
        QString xQuerySKU = dlg.getQuerySKU();

        m_pMainWidget->setSelectFilters(QString(" where skucode='%1'").arg(xQuerySKU));
        m_pMainWidget->setSelectOrder(QString(""));
        m_pMainWidget->setOrderInsertTimeDesc(false);
    }
}

void MainWindow::onActionShowAll()
{
    m_pMainWidget->setSelectFilters(QString(""));
    m_pMainWidget->setSelectOrder(QString(""));
    m_pMainWidget->setOrderInsertTimeDesc(false);
}

void MainWindow::onActionDelete()
{
    QString xSelectSKUCode = m_pMainWidget->getCurrentSelectSKUCode();
    if(xSelectSKUCode.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择有效的行"));
        return;
    }

    ProductItem item;
    if(!SqlManager::getInstance()->getProductItem(item, xSelectSKUCode))
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("获取原始信息失败"));
        return;
    }

    QString xQuestion = QStringLiteral("确认要删除SKU为[%1]的记录?").arg(xSelectSKUCode);
    if(QMessageBox::question(this, QStringLiteral("询问"), xQuestion, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if(SqlManager::getInstance()->delProductItem(xSelectSKUCode))
        {
            m_pMainWidget->updatePage();
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QString xTip = QStringLiteral("erp demo developed by sryan.\n\n\n\n   mail:sryan@qq.com");
    QMessageBox::information(this, QStringLiteral("关于"), xTip);
}

void MainWindow::onActionEditCategory()
{
    CategoryDlg dlg;
    dlg.exec();
}

void MainWindow::onTableMenu(const QPoint& _refMouse)
{
    m_xTableMenu.exec(QCursor::pos());
}
