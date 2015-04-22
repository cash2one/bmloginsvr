#pragma execution_character_set("utf-8")

#include "newskudlg.h"
#include "ui_newskudlg.h"
#include "gfunctions.h"
#include <QMessageBox>
#include <qdebug.h>

NewSKUDlg::NewSKUDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSKUDlg)
{
    ui->setupUi(this);

    m_bExecuteNextStep = false;

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
        accept();
    }
}

void NewSKUDlg::onSelectChanged(int _index)
{
    qDebug() << __FUNCTION__;

    if(ui->comboBox->count() == 0 ||
            ui->comboBox_2->count() == 0 ||
            ui->comboBox_3->count() == 0)
    {
        return;
    }

    const char* pszSelect1 = ui->comboBox->currentText().toStdString().c_str();
    const char* pszSelect2 = ui->comboBox_2->currentText().toStdString().c_str();
    const char* pszSelect3 = ui->comboBox_3->currentText().toStdString().c_str();

    m_xSKUNumber = getSKUNumber(pszSelect1, pszSelect2, pszSelect3);
    ui->lineEdit->setText(m_xSKUNumber);
}
