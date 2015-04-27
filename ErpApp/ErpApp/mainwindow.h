#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

class QPushButton;
class QLabel;
class QAction;
class MainWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createWidgets();
    void createMenu();

public slots:
    void onActionNewProduct();
    void onActionPrevPage();
    void onActionNextPage();
    void onActionModifyProduct();
    void onActionFind();
    void onActionShowAll();
    void onActionDelete();
    void onActionEditCategory();

    void onTableMenu(const QPoint& _refMouse);

private slots:
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    QPushButton* m_pBtnPrevPage;
    QPushButton* m_pBtnNextPage;
    QLabel* m_pLblPageInfo;

    MainWidget* m_pMainWidget;
    QMenu m_xTableMenu;
};

#endif // MAINWINDOW_H
