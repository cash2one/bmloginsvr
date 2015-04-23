#ifndef NEWSKUDLG_H
#define NEWSKUDLG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class NewSKUDlg;
}

class NewSKUDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewSKUDlg(QWidget *parent = 0);
    ~NewSKUDlg();

public slots:
    void onNextStep();
    void onSelectChanged(int _index);

public:
    bool canExecuteNextStep()
    {
        return m_bExecuteNextStep;
    }
    QString& getSkuNumber()
    {
        return m_xSKUNumber;
    }
    QStringList& getCategory()
    {
        return m_xCategory;
    }
    int getCategorySeq()
    {
        return m_nCategorySeq;
    }

private:
    void createWidgets();

private:
    Ui::NewSKUDlg *ui;

    bool m_bExecuteNextStep;
    QString m_xSKUNumber;
    QStringList m_xCategory;
    int m_nCategorySeq;
};

#endif // NEWSKUDLG_H
