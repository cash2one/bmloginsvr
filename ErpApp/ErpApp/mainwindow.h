#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void onActionNewProduct();
    void onActionPrevPage();
    void onActionNextPage();
    void onActionModifyProduct();
    void onActionFind();
    void onActionShowAll();
    void onActionDelete();
    void onActionEditCategory();

private slots:
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    QPushButton* m_pBtnPrevPage;
    QPushButton* m_pBtnNextPage;
    QLabel* m_pLblPageInfo;

    MainWidget* m_pMainWidget;
};

#endif // MAINWINDOW_H
