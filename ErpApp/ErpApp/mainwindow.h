#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class QLabel;
class QAction;

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

private:
    Ui::MainWindow *ui;

    QPushButton* m_pBtnPrevPage;
    QPushButton* m_pBtnNextPage;
    QLabel* m_pLblPageInfo;
};

#endif // MAINWINDOW_H
