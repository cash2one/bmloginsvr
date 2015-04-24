#ifndef FINDDLG_H
#define FINDDLG_H

#include <QDialog>

namespace Ui {
class FindDlg;
}

class QComboBox;

class FindDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FindDlg(QWidget *parent = 0);
    ~FindDlg();

public:
    void createWidgets();

    QString& getSkuNumber()
    {
        return m_xSKUNumber;
    }
    QString& getQuerySKU()
    {
        return m_xQuerySKU;
    }

    void loadComboContent(QComboBox* _pWidget);
    void calcSKUCode();
    void clearSKUCode();

public slots:
    void onSelectChanged(int _index);

private slots:
    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::FindDlg *ui;

    QString m_xSKUNumber;
    QString m_xQuerySKU;
};

#endif // FINDDLG_H
