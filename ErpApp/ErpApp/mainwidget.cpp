#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include "SqlManager.h"
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_nTableColCount = 5;
    m_nTableRowCount = 3;
    m_nPage = 0;
    m_bOrderInsertTimeDesc = false;

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

    connect(m_pTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));

    showPage(m_nPage);
}


bool MainWidget::showPage(int _nPage)
{
    ProductItemList xItems;

    QString xFilterWords = QString(" %1 %2 ").arg(m_xOderItem, m_xFilterItem);
    QString xPageFilter = QString("limit %1 offset %2").arg(m_nTableRowCount).arg(_nPage * m_nTableRowCount);
    xFilterWords += xPageFilter;

    bool bRet = false;

    if(SqlManager::getInstance()->getProductItems(xItems, xFilterWords))
    {
        qDebug() << "productList " << xItems.size();

        if(xItems.size() > 0)
        {
            m_nPage = _nPage;
            bRet = true;
        }

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
        return false;
    }

    return bRet;
}

QString MainWidget::getSqlAddExpr()
{


    return "";
}

void MainWidget::updatePage()
{
    showPage(m_nPage);
}

void MainWidget::setRowContent(int _nRow, ProductItem& _refItem, bool _bEmpty)
{
    QTableWidgetItem* pItem = NULL;

    //  create cols
    for(int i = 0; i < m_nTableColCount; ++i)
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

QString MainWidget::getCurrentSelectSKUCode()
{
    int nSelectRow = m_pTable->currentRow();
    QTableWidgetItem* pItem = m_pTable->item(nSelectRow, 0);

    if(NULL == pItem)
    {
        return QString("");
    }
    else
    {
        return pItem->text();
    }
}

void MainWidget::setSelectFilters(const QString &_refFilter)
{
    m_xFilterItem = _refFilter;

    m_nPage = 0;
    updatePage();
}

void MainWidget::setSelectOrder(const QString &_refOrder)
{
    m_xOderItem = _refOrder;

    m_nPage = 0;
    updatePage();
}

void MainWidget::PrevPage()
{
    int nPage = m_nPage;
    --nPage;

    if(nPage < 0)
    {
        nPage = 0;
    }

    if(nPage != m_nPage)
    {
        if(!showPage(nPage))
        {
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("已到达第一页"));
            return;
        }
    }
    else
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("已到达第一页"));
        return;
    }
}

void MainWidget::NextPage()
{
    int nPage = m_nPage;
    ++nPage;

    /*if(!showPage(nPage))
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("已到达最后一页"));
        return;
    }*/
    //  判断是否到达最后一页
    int nCount = SqlManager::getInstance()->getRowCount(m_xFilterItem);
    if(-1 == nCount)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("获取行数失败"));
        return;
    }

    qDebug() << "next page item count:" << nCount;

    int nFirstShowIndex = nPage * m_nTableRowCount;
    if(nFirstShowIndex >= nCount)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("已到达最后一页"));
        return;
    }

    showPage(nPage);
}

void MainWidget::onHeaderClicked(int _nHeader)
{
    if(_nHeader == 4)
    {
        QString xOrder = " order by inserttime ";
        if(m_bOrderInsertTimeDesc)
        {
            xOrder += "desc ";
        }

        setSelectOrder(xOrder);

        m_bOrderInsertTimeDesc = !m_bOrderInsertTimeDesc;
    }
}
