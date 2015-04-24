#include "newproductdlg.h"
#include "ui_newproductdlg.h"
#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <QMessageBox>
#include <QFileDialog>
#include <qdebug.h>
#include "gfunctions.h"
#include <QFile>
#include <string>
#include "direct.h"
#include <QFileInfo>
#include <QPixmap>
#include <stdio.h>
#include <QDoubleValidator>
#include "SqlManager.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

using std::string;

NewProductDlg::NewProductDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProductDlg)
{
    m_bNewItemMode = true;
    m_nCategorySeq = 0;
    m_bContentModified = false;

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

    QDoubleValidator* pValidator = new QDoubleValidator(0.0, 99999999.0, 2, this);
    ui->lineEdit_realWeight->setValidator(pValidator);
    pValidator = new QDoubleValidator(0.0, 99999999.0, 2, this);
    ui->lineEdit_estimateWeight->setValidator(pValidator);

    ui->tabWidget->setCurrentIndex(0);
}

void NewProductDlg::setSKUCode(const QString &_refSKUCode)
{
    m_xSKUCode = _refSKUCode;
    ui->lineEdit_SKUCode->setText(m_xSKUCode);

    //  检查是否已存在，存在的话修改窗体，否则新增的窗体
    int nCount = SqlManager::getInstance()->getItemCount(_refSKUCode);
    if(1 == nCount)
    {
        m_bNewItemMode = false;
    }

    //  从数据库读取数据
    if(!m_bNewItemMode)
    {
        ProductItem item;
        item.xSKUCode = m_xSKUCode;
        if(SqlManager::getInstance()->getProductItem(item, m_xSKUCode))
        {
            updatePage(item);
        }
    }

    updateMode();
}

void NewProductDlg::resetTableModelAttrib()
{
    m_pTableModelAttrib->clear();
    QStandardItem* pTableItem = new QStandardItem(QStringLiteral("产品属性"));
    m_pTableModelAttrib->setHorizontalHeaderItem(0, pTableItem);
    m_pTableModelAttrib->setColumnCount(1);

    QTableView* pTableView = ui->tableView_attrib;
    pTableView->setColumnWidth(0, 250);
}

void NewProductDlg::resetTableModelBuy()
{
    m_pTableModelBuy->clear();
    QStandardItem* pTableItem = new QStandardItem(QStringLiteral("采购网址"));
    m_pTableModelBuy->setHorizontalHeaderItem(0, pTableItem);
    pTableItem = new QStandardItem(QStringLiteral("采购价格"));
    m_pTableModelBuy->setHorizontalHeaderItem(1, pTableItem);
    m_pTableModelBuy->setColumnCount(2);

    QTableView* pTableView = ui->tableView_buy;
    pTableView->setColumnWidth(0, 210);
}

void NewProductDlg::updatePage(ProductItem &_refItem)
{
    ui->lineEdit_productName->setText(_refItem.xProductName);
    ui->lineEdit_estimateWeight->setText(QString("%1").arg(_refItem.fEstimateWeight));
    ui->lineEdit_realWeight->setText(QString("%1").arg(_refItem.fRealWeight));
    ui->lineEdit_SKUCode->setText(_refItem.xSKUCode);
    ui->textEdit_Note->setText(_refItem.xNote);

    //  属性
    resetTableModelAttrib();
    for(int i = 0; i < _refItem.xAttribDetails.size(); ++i)
    {
        QString& refStrItem = _refItem.xAttribDetails[i];
        if(refStrItem.isEmpty())
        {
            continue;
        }

        int nCurrentRowCount = m_pTableModelAttrib->rowCount();
        m_pTableModelAttrib->setItem(nCurrentRowCount, 0, new QStandardItem(refStrItem));
    }

    //  采购网站和价格
    resetTableModelBuy();
    for(int i = 0; i < _refItem.xBuyDetails.size(); ++i)
    {
        QString& refStrItem = _refItem.xBuyDetails[i];
        if(refStrItem.isEmpty())
        {
            continue;
        }

        //  解析json字符串
        QJsonParseError eError;
        QJsonDocument xJsDoc = QJsonDocument::fromJson(refStrItem.toUtf8(), &eError);

        if(eError.error == QJsonParseError::NoError)
        {
            if(xJsDoc.isObject())
            {
                QJsonObject xJsObj = xJsDoc.object();

                if(xJsObj.contains("url") &&
                        xJsObj.contains("price"))
                {
                    QString xUrl = xJsObj.take("url").toString();
                    QString xPrice = xJsObj.take("price").toString();

                    int nCurrentRowCount = m_pTableModelBuy->rowCount();
                    m_pTableModelBuy->setItem(nCurrentRowCount, 0, new QStandardItem(xUrl));
                    m_pTableModelBuy->setItem(nCurrentRowCount, 1, new QStandardItem(xPrice));
                }
            }
        }
    }
}

void NewProductDlg::updateMode()
{
    if(m_bNewItemMode)
    {
        setWindowTitle(QStringLiteral("添加新产品"));
    }
    else
    {
        setWindowTitle(QStringLiteral("修改产品信息"));
    }
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

void NewProductDlg::on_pushButton_imagePath_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择缩略图文件"),
                                                    "",
                                                    "imagefiles(*.jpg *.jpeg *.png *.bmp);;jpgfile(*.jpg);;jpegfile(*.jpeg);;pngfile(*.png);;bmpfile(*.bmp)");
    QFileInfo fi(fileName);
    QString xFileSuffix = fi.suffix();

    if(!fileName.isEmpty())
    {
        qDebug() << "select a new image file";
        ui->lineEdit_imagePath->setText(fileName);

        //  将图片复制入db文件夹
        QString xDestDir = getAppPath() + "/db/" + m_xSKUCode;
        if(!fileExists(xDestDir))
        {
            string xDir = xDestDir.toStdString();
            if(0 != mkdir(xDir.c_str()))
            {
                QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("创建文件缩略图文件夹失败"));
                return;
            }
        }

        QString xDestFileDir = xDestDir + "/snap." + xFileSuffix;

        //  先删除文件
        if(fileExists(xDestFileDir))
        {
            string xCStrDestFileDir = xDestFileDir.toStdString();
            if(0 != remove(xCStrDestFileDir.c_str()))
            {
                QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("移除原始缩略图失败"));
                return;
            }
        }

        if(!QFile::copy(fileName, xDestFileDir))
        {
            QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("复制文件缩略图失败"));
            return;
        }

        ui->label_thumb->setPixmap(QPixmap(xDestFileDir));
    }
}

void NewProductDlg::on_pushButton_newProduct_clicked()
{
    ProductItem item;
    //  base info
    item.xSKUCode = m_xSKUCode;
    item.xFstCategory = m_xFstCategory;
    item.xSecCategory = m_xSecCategory;
    item.xTrdCategory = m_xTrdCategory;
    item.xProductName = ui->lineEdit_productName->text();
    item.fEstimateWeight = ui->lineEdit_estimateWeight->text().toFloat();
    item.fRealWeight = ui->lineEdit_realWeight->text().toFloat();
    item.nInsertTime = QDateTime::currentDateTime().toTime_t();
    item.nSeq = m_nCategorySeq;

    //  attrib
    for(int i = 0; i < m_pTableModelAttrib->rowCount(); ++i)
    {
        QStandardItem* pItem = m_pTableModelAttrib->item(i);
        if(NULL != pItem)
        {
            QString xValue = pItem->text();
            if(!xValue.isEmpty())
            {
                item.xAttribDetails.push_back(xValue);
            }
        }
    }

    //  buy url
    for(int i = 0; i < m_pTableModelBuy->rowCount(); ++i)
    {
        QString xUrl;
        QString xPrice;
        QStandardItem* pItem = m_pTableModelBuy->item(i);
        if(NULL != pItem)
        {
            xUrl = pItem->text();
        }
        pItem = m_pTableModelBuy->item(i, 1);
        if(NULL != pItem)
        {
            xPrice = pItem->text();
        }

        if(!xUrl.isEmpty() ||
                !xPrice.isEmpty())
        {
            QJsonObject xJsObj;
            xJsObj.insert("url", xUrl);
            xJsObj.insert("price", xPrice);

            QJsonDocument doc;
            doc.setObject(xJsObj);
            QByteArray xByteArray = doc.toJson(QJsonDocument::Compact);
            QString xJsData(xByteArray);

            item.xBuyDetails.push_back(xJsData);
        }
    }

    if(item.xProductName.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("货物名称不能为空"));
        return;
    }

    if(m_bNewItemMode)
    {
        //  新建商品
        if(SqlManager::getInstance()->newProductItem(item))
        {
            m_bNewItemMode = false;
            m_bContentModified = true;
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("新增项目成功"));
            updateMode();
        }
        else
        {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无法创建项目"));
        }
    }
    else
    {
        //  更新商品
        if(SqlManager::getInstance()->updateProductItem(item))
        {
            m_bContentModified = true;
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("修改项目成功"));
        }
        else
        {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无法修改项目"));
        }
    }
}
