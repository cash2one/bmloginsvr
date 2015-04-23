#pragma execution_character_set("utf-8")

#include "newskudlg.h"
#include "ui_newskudlg.h"
#include "gfunctions.h"
#include <QMessageBox>
#include <qdebug.h>
#include "SqlManager.h"

NewSKUDlg::NewSKUDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSKUDlg)
{
    ui->setupUi(this);

    m_bExecuteNextStep = false;
    m_nCategorySeq = 0;

    createWidgets();
}

NewSKUDlg::~NewSKUDlg()
{
    delete ui;
}


void NewSKUDlg::createWidgets()
{
    setWindowTitle(QStringLiteral("第一步：生成SKU码"));

    ui->comboBox->addItem(QStringLiteral("11"));
    ui->comboBox_2->addItem(QStringLiteral("11"));
    ui->comboBox_3->addItem(QStringLiteral("11"));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onNextStep()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectChanged(int)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectChanged(int)));
    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectChanged(int)));

    onSelectChanged(0);
}


//  slots
void NewSKUDlg::onNextStep()
{
    qDebug() << __FUNCTION__;

    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择正确的类别"));
    }
    else
    {
        m_bExecuteNextStep = true;
        m_xSKUNumber = ui->lineEdit->text();
        m_xCategory.clear();
        QString xCategory = ui->comboBox->currentText();
        m_xCategory << xCategory;
        xCategory = ui->comboBox_2->currentText();
        m_xCategory << xCategory;
        xCategory = ui->comboBox_3->currentText();
        m_xCategory << xCategory;
        accept();
    }
}

void NewSKUDlg::onSelectChanged(int _index)
{
    if(ui->comboBox->count() == 0 ||
            ui->comboBox_2->count() == 0 ||
            ui->comboBox_3->count() == 0)
    {
        return;
    }

    QString xFstCategory = ui->comboBox->currentText();
    QString xSecCategory = ui->comboBox_2->currentText();
    QString xTrdCategory = ui->comboBox_3->currentText();

    /*const char* pszSelect1 = xFstCategory.toStdString().c_str();
    const char* pszSelect2 = xSecCategory.toStdString().c_str();
    const char* pszSelect3 = xTrdCategory.toStdString().c_str();*/

    if(xFstCategory.isEmpty() ||
            xSecCategory.isEmpty() ||
            xTrdCategory.isEmpty())
    {
        return;
    }

    //m_xSKUNumber = getSKUNumber(pszSelect1, pszSelect2, pszSelect3);
    //ui->lineEdit->setText(m_xSKUNumber);
    int nSKUSeq = SqlManager::getInstance()->getMaxItemSeq(
                xFstCategory,
                xSecCategory,
                xTrdCategory);
    if(nSKUSeq < 0)
    {
        //  error
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("获取货物序号失败"));
        return;
    }

    ++nSKUSeq;
    char szNumber[10] = {0};
    sprintf(szNumber, "%03d", nSKUSeq);
    m_xSKUNumber = xFstCategory + xSecCategory + xTrdCategory + szNumber;
    m_nCategorySeq = nSKUSeq;

    ui->lineEdit->setText(m_xSKUNumber);

    qDebug() << __FUNCTION__ << xFstCategory << xSecCategory << xTrdCategory << nSKUSeq;
}
