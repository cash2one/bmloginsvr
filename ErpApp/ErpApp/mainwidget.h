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
    void showPage(int _nPage, int _nPerPage);
    void updatePage();
    void setRowContent(int _nRow, ProductItem& _refItem, bool _bEmpty);

private:
    void createWidgets();

private:
    Ui::MainWidget *ui;

    QTableWidget* m_pTable;
    int m_nTableRowCount;
    int m_nTableColCount;
    int m_nPage;
};

#endif // MAINWIDGET_H
