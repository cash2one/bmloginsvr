#ifndef CATEGORYNEWNODEDLG_H
#define CATEGORYNEWNODEDLG_H

#include <QDialog>

namespace Ui {
class CategoryNewNodeDlg;
}

class CategoryNewNodeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryNewNodeDlg(QWidget *parent = 0);
    ~CategoryNewNodeDlg();

public:
    QString& getName()
    {
        return m_xName;
    }
    QString& getCode()
    {
        return m_xCode;
    }
    int getMode()
    {
        return m_nMode;
    }
    void setMode(int _nMode);
    void setDefaultName(const QString& _refName);
    void setDefaultCode(const QString& _refCode);

private slots:
    void on_pushButton_clicked();

private:
    Ui::CategoryNewNodeDlg *ui;

    QString m_xName;
    QString m_xCode;
    int m_nMode;
};

#endif // CATEGORYNEWNODEDLG_H
