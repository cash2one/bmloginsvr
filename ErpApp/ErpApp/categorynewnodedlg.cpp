#include "categorynewnodedlg.h"
#include "ui_categorynewnodedlg.h"
#include <QMessageBox>

CategoryNewNodeDlg::CategoryNewNodeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryNewNodeDlg)
{
    ui->setupUi(this);
    m_nMode = 0;
}

CategoryNewNodeDlg::~CategoryNewNodeDlg()
{
    delete ui;
}

void CategoryNewNodeDlg::setMode(int _nMode)
{
    m_nMode = _nMode;

    if(1 == m_nMode)
    {
        ui->pushButton->setText(QStringLiteral("修改"));
        setWindowTitle("修改节点数据");
    }
}

void CategoryNewNodeDlg::setDefaultCode(const QString &_refCode)
{
    ui->lineEdit_2->setText(_refCode);
}

void CategoryNewNodeDlg::setDefaultName(const QString &_refName)
{
    ui->lineEdit->setText(_refName);
}

void CategoryNewNodeDlg::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请输入名称"));
        return;
    }
    if(ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请输入代号"));
        return;
    }

    m_xCode = ui->lineEdit_2->text();
    m_xName = ui->lineEdit->text();

    accept();
}
