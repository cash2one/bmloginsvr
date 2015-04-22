#include "newproductdlg.h"
#include "ui_newproductdlg.h"
#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <QMessageBox>

NewProductDlg::NewProductDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProductDlg)
{
    ui->setupUi(this);
    initWidgets();
}

NewProductDlg::~NewProductDlg()
{
    delete ui;
}


void NewProductDlg::initWidgets()
{
    m_pTableModelBuy = new QStandardItemModel(this);
    QStandardItem* pTableItem = new QStandardItem(QStringLiteral("采购网址"));
    m_pTableModelBuy->setHorizontalHeaderItem(0, pTableItem);
    pTableItem = new QStandardItem(QStringLiteral("采购价格"));
    m_pTableModelBuy->setHorizontalHeaderItem(1, pTableItem);

    QTableView* pTableView = ui->tableView_buy;
    pTableView->setModel(m_pTableModelBuy);
    pTableView->setColumnWidth(0, 210);

    m_pTableModelAttrib = new QStandardItemModel(this);
    pTableItem = new QStandardItem(QStringLiteral("产品属性"));
    m_pTableModelAttrib->setHorizontalHeaderItem(0, pTableItem);

    pTableView = ui->tableView_attrib;
    pTableView->setModel(m_pTableModelAttrib);
    pTableView->setColumnWidth(0, 250);

    ui->tabWidget->setCurrentIndex(0);
}

void NewProductDlg::setSKUCode(const QString &_refSKUCode)
{
    m_xSKUCode = _refSKUCode;
    ui->lineEdit_SKUCode->setText(m_xSKUCode);
}

void NewProductDlg::on_pushButton_newBuyItem_clicked()
{
    int nCurrentRowCount = m_pTableModelBuy->rowCount();
    m_pTableModelBuy->setItem(nCurrentRowCount, 0, new QStandardItem(""));
    m_pTableModelBuy->setItem(nCurrentRowCount, 0, new QStandardItem(""));
}

void NewProductDlg::on_pushButton_removeBuyItem_clicked()
{
    QTableView* pTableView = ui->tableView_buy;
    QItemSelectionModel *selections = pTableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();

    if(selected.isEmpty())
    {
        return;
    }

    QModelIndex xCurrentSelectIndex = pTableView->currentIndex();
    m_pTableModelBuy->removeRow(xCurrentSelectIndex.row());
}

void NewProductDlg::on_pushButton_newAttribItem_clicked()
{
    int nCurrentRowCount = m_pTableModelAttrib->rowCount();
    m_pTableModelAttrib->setItem(nCurrentRowCount, 0, new QStandardItem(""));
}

void NewProductDlg::on_pushButton_removeAttribItem_clicked()
{
    QTableView* pTableView = ui->tableView_attrib;
    QItemSelectionModel *selections = pTableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();

    if(selected.isEmpty())
    {
        return;
    }

    QModelIndex xCurrentSelectIndex = pTableView->currentIndex();
    m_pTableModelAttrib->removeRow(xCurrentSelectIndex.row());
}

void NewProductDlg::on_textEdit_Note_textChanged()
{
    if(ui->textEdit_Note->toPlainText().length() > 512)
    {
        QString xLeft = ui->textEdit_Note->toPlainText().left(512);
        ui->textEdit_Note->setText(xLeft);

        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("最大字符限制512"));
    }
}
