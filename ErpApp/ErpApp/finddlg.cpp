#include "finddlg.h"
#include "ui_finddlg.h"
#include "CategoryManager.h"
#include <qdebug.h>
#include <QIntValidator>
#include <QMessageBox>

FindDlg::FindDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDlg)
{
    ui->setupUi(this);

    createWidgets();
}

FindDlg::~FindDlg()
{
    delete ui;
}


void FindDlg::createWidgets()
{
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectChanged(int)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectChanged(int)));
    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectChanged(int)));

    ui->lineEdit_2->setValidator(new QIntValidator(0, 999));

    loadComboContent(NULL);
}

void FindDlg::loadComboContent(QComboBox *_pWidget)
{
    //  初始化
    QStringList xParents;
    QStringList xCategorys;

    if(_pWidget == NULL)
    {
        ui->comboBox->clear();

        if(CategoryManager::getInstance()->getCategoryByParent(xParents, xCategorys))
        {
            //  1
            for(int i = 0; i < xCategorys.size(); ++i)
            {
                QString& _refStrItem = xCategorys[i];

                if(i % 2 == 0)
                {
                    ui->comboBox->addItem(_refStrItem, xCategorys[i + 1]);
                }
            }

            loadComboContent(ui->comboBox);
        }
        else
        {
            qDebug() << "failed to getCategoryByParent";
        }
    }
    else if(_pWidget == ui->comboBox)
    {
        ui->comboBox_2->clear();
        ui->comboBox_3->clear();

        //  根据1读取2
        QString xText1 = ui->comboBox->currentText();
        if(xText1.isEmpty())
        {
            return;
        }

        xParents << xText1;
        if(CategoryManager::getInstance()->getCategoryByParent(xParents, xCategorys))
        {
            //  1
            for(int i = 0; i < xCategorys.size(); ++i)
            {
                QString& _refStrItem = xCategorys[i];

                if(i % 2 == 0)
                {
                    ui->comboBox_2->addItem(_refStrItem, xCategorys[i + 1]);
                }
            }

            loadComboContent(ui->comboBox_2);
        }
    }
    else if(_pWidget == ui->comboBox_2)
    {
        ui->comboBox_3->clear();

        //  根据2读取3
        QString xText1 = ui->comboBox->currentText();
        if(xText1.isEmpty())
        {
            return;
        }
        QString xText2 = ui->comboBox_2->currentText();
        if(xText2.isEmpty())
        {
            return;
        }

        xParents << xText1 << xText2;

        if(CategoryManager::getInstance()->getCategoryByParent(xParents, xCategorys))
        {
            //  1
            for(int i = 0; i < xCategorys.size(); ++i)
            {
                QString& _refStrItem = xCategorys[i];

                if(i % 2 == 0)
                {

                    ui->comboBox_3->addItem(_refStrItem, xCategorys[i + 1]);
                }
            }
        }
    }
    else if(_pWidget == ui->comboBox_3)
    {

    }
}

void FindDlg::onSelectChanged(int _index)
{
    QObject* pObj = sender();

    if(pObj->isWidgetType())
    {
        QComboBox* pWidget = static_cast<QComboBox*>(pObj);
        loadComboContent(pWidget);
        calcSKUCode();
    }
}

void FindDlg::calcSKUCode()
{
    m_xSKUNumber.clear();

    if(ui->comboBox->count() == 0 ||
            ui->comboBox_2->count() == 0 ||
            ui->comboBox_3->count() == 0)
    {
        ui->lineEdit_3->clear();
        return;
    }

    QString xFstCategory = ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
    QString xSecCategory = ui->comboBox_2->itemData(ui->comboBox_2->currentIndex()).toString();
    QString xTrdCategory = ui->comboBox_3->itemData(ui->comboBox_3->currentIndex()).toString();

    if(xFstCategory.isEmpty() ||
            xSecCategory.isEmpty() ||
            xTrdCategory.isEmpty())
    {
        ui->lineEdit_3->clear();
        return;
    }

    bool bTransOk = false;
    int nSKUSeq = ui->lineEdit_2->text().toInt(&bTransOk);
    if(!bTransOk)
    {
        nSKUSeq = 0;
    }
    char szNumber[10] = {0};
    sprintf(szNumber, "%03d", nSKUSeq);
    m_xSKUNumber = xFstCategory + xSecCategory + xTrdCategory + szNumber;

    ui->lineEdit_3->setText(m_xSKUNumber);

    qDebug() << __FUNCTION__ << xFstCategory << xSecCategory << xTrdCategory << nSKUSeq;
}

void FindDlg::clearSKUCode()
{
    m_xSKUNumber.clear();
    ui->lineEdit_3->clear();
}

void FindDlg::on_lineEdit_2_textChanged(const QString &arg1)
{
    calcSKUCode();
}

void FindDlg::on_pushButton_clicked()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        //  find by sku
        QString xText = ui->lineEdit->text();
        if(xText.isEmpty())
        {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("查找的SKU不能为空"));
            return;
        }

        m_xQuerySKU = xText;
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        if(m_xSKUNumber.isEmpty())
        {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("查找的SKU不能为空"));
            return;
        }

        m_xQuerySKU = m_xSKUNumber;
    }

    accept();
}
