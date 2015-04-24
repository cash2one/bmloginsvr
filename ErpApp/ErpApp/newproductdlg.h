#ifndef NEWPRODUCTDLG_H
#define NEWPRODUCTDLG_H

#include <QDialog>

namespace Ui {
class NewProductDlg;
}

class QStandardItemModel;
struct ProductItem;

class NewProductDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewProductDlg(QWidget *parent = 0);
    ~NewProductDlg();

public:
    void initWidgets();
    void setSKUCode(const QString& _refSKUCode);
    void setCategory(const QString& _refFstCategory,
                     const QString& _refSecCategory,
                     const QString& _refTrdCategory)
    {
        m_xFstCategory = _refFstCategory;
        m_xSecCategory = _refSecCategory;
        m_xTrdCategory = _refTrdCategory;
    }
    void setCategorySeq(int _nSeq)
    {
        m_nCategorySeq = _nSeq;
    }
    bool getContentModified()
    {
        return m_bContentModified;
    }

    void updatePage(ProductItem& _refItem);
    void updateMode();
    void resetTableModelAttrib();
    void resetTableModelBuy();

private slots:
    void on_pushButton_newBuyItem_clicked();

    void on_pushButton_removeBuyItem_clicked();

    void on_pushButton_newAttribItem_clicked();

    void on_pushButton_removeAttribItem_clicked();

    void on_textEdit_Note_textChanged();

    void on_pushButton_imagePath_clicked();

    void on_pushButton_newProduct_clicked();

private:
    Ui::NewProductDlg *ui;

    QString m_xSKUCode;
    QString m_xFstCategory;
    QString m_xSecCategory;
    QString m_xTrdCategory;
    int m_nCategorySeq;

    //  table model
    QStandardItemModel* m_pTableModelBuy;
    QStandardItemModel* m_pTableModelAttrib;

    bool m_bNewItemMode;
    bool m_bContentModified;
};

#endif // NEWPRODUCTDLG_H
