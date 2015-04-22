#ifndef NEWSKUDLG_H
#define NEWSKUDLG_H

#include <QDialog>

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

private:
    void createWidgets();

private:
    Ui::NewSKUDlg *ui;

    bool m_bExecuteNextStep;
    QString m_xSKUNumber;
};

#endif // NEWSKUDLG_H
