#ifndef NEWPRODUCTDLG_H
#define NEWPRODUCTDLG_H

#include <QDialog>

namespace Ui {
class NewProductDlg;
}

class QStandardItemModel;

class NewProductDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewProductDlg(QWidget *parent = 0);
    ~NewProductDlg();

public:
    void initWidgets();
    void setSKUCode(const QString& _refSKUCode);

private slots:
    void on_pushButton_newBuyItem_clicked();

    void on_pushButton_removeBuyItem_clicked();

    void on_pushButton_newAttribItem_clicked();

    void on_pushButton_removeAttribItem_clicked();

    void on_textEdit_Note_textChanged();

private:
    Ui::NewProductDlg *ui;

    QString m_xSKUCode;

    //  table model
    QStandardItemModel* m_pTableModelBuy;
    QStandardItemModel* m_pTableModelAttrib;
};

#endif // NEWPRODUCTDLG_H
