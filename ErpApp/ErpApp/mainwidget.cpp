#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include "SqlManager.h"
#include <QDebug>
#include <QDateTime>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_nTableColCount = 5;
    m_nTableRowCount = 15;
    m_nPage = 0;

    createWidgets();
}

MainWidget::~MainWidget()
{
    delete ui;
}


void MainWidget::createWidgets()
{
    QVBoxLayout* pMainLo = new QVBoxLayout;
    m_pTable = new QTableWidget(this);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pMainLo->addWidget(m_pTable);
    setLayout(pMainLo);

    //  initialize tablewidget
    m_pTable->setColumnCount(m_nTableColCount);
    m_pTable->setRowCount(m_nTableRowCount);
    QStringList xHeader;
    xHeader << QStringLiteral("货物编号")
               << QStringLiteral("货物名称")
                  << QStringLiteral("进价")
                  << QStringLiteral("实际重量")
                  << QStringLiteral("新增时间");
    m_pTable->setHorizontalHeaderLabels(xHeader);

    showPage(m_nPage, m_nTableRowCount);
}


void MainWidget::showPage(int _nPage, int _nPerPage)
{
    ProductItemList xItems;

    if(SqlManager::getInstance()->getProductItems(xItems, _nPage, m_nTableRowCount))
    {
        qDebug() << "productList " << xItems.size();

        m_nPage = _nPage;
        ProductItem xEmptyItem;

        for(int i = 0; i < m_nTableRowCount; ++i)
        {
            if(i < xItems.size())
            {
                setRowContent(i, xItems[i], false);
            }
            else
            {
                setRowContent(i, xEmptyItem, true);
            }
        }
    }
    else
    {
        qDebug() << "getProductItems failed";
    }
}

void MainWidget::updatePage()
{
    showPage(m_nPage, m_nTableRowCount);
}

void MainWidget::setRowContent(int _nRow, ProductItem& _refItem, bool _bEmpty)
{
    QTableWidgetItem* pItem = NULL;

    //  create rows
    for(int i = 0; i < m_nTableRowCount; ++i)
    {
        pItem = m_pTable->item(_nRow, i);
        if(NULL == pItem)
        {
            pItem = new QTableWidgetItem();
            m_pTable->setItem(_nRow, i, pItem);
        }
    }

    //  sku
    pItem = m_pTable->item(_nRow, 0);
    if(!_bEmpty)
    {
        pItem->setText(_refItem.xSKUCode);
    }
    else
    {
        pItem->setText("");
    }

    //  product name
    pItem = m_pTable->item(_nRow, 1);
    if(!_bEmpty)
    {
        pItem->setText(_refItem.xProductName);
    }
    else
    {
        pItem->setText("");
    }

    //  buy price
    pItem = m_pTable->item(_nRow, 2);
    if(!_bEmpty)
    {
        pItem->setText(QString::number(_refItem.fBuyMoney));
    }
    else
    {
        pItem->setText("");
    }

    //  real weight
    pItem = m_pTable->item(_nRow, 3);
    if(!_bEmpty)
    {
        pItem->setText(QString::number(_refItem.fRealWeight));
    }
    else
    {
        pItem->setText("");
    }

    //  time
    pItem = m_pTable->item(_nRow, 4);
    if(!_bEmpty)
    {
        QDateTime time = QDateTime::fromTime_t(_refItem.nInsertTime);
        qDebug() << time << _refItem.nInsertTime;
        pItem->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
    }
    else
    {
        pItem->setText("");
    }
}
