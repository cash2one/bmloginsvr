#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QTableWidget;
struct ProductItem;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

public:
    bool showPage(int _nPage);
    int getPage()   {return m_nPage;}
    void updatePage();
    void setRowContent(int _nRow, ProductItem& _refItem, bool _bEmpty);

    void NextPage();
    void PrevPage();

    QString getCurrentSelectSKUCode();
    void setSelectFilters(const QString& _refFilter);
    void setSelectOrder(const QString& _refOrder);
    void setOrderInsertTimeDesc(bool _bDesc)
    {
        m_bOrderInsertTimeDesc = _bDesc;
    }

    QString getSqlAddExpr();

private:
    void createWidgets();

public slots:
    void onHeaderClicked(int _nHeader);

private:
    Ui::MainWidget *ui;

    QTableWidget* m_pTable;
    int m_nTableRowCount;
    int m_nTableColCount;
    int m_nPage;

    //  sql filters
    QString m_xFilterItem;
    QString m_xOderItem;

    bool m_bOrderInsertTimeDesc;
};

#endif // MAINWIDGET_H
