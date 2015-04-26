#ifndef CATEGORYDLG_H
#define CATEGORYDLG_H

#include <QDialog>
#include <QMap>
#include <QtXml/QDomElement>

class QTreeWidgetItem;
typedef QMap<QTreeWidgetItem*, QString> TreeWidgetUserDataMap;

namespace Ui {
class CategoryDlg;
}

class CategoryDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryDlg(QWidget *parent = 0);
    ~CategoryDlg();

public:
    void createWidgets();
    void createFromLocalData();
    void createNode(QTreeWidgetItem* _pParentNode, QDomElement& _refDoc);

    bool canAddNode(QTreeWidgetItem* _pParentNode, const QString& _refCode);

    bool addNode(QTreeWidgetItem* _pParentNode, const QString& _refName, const QString& _refCode);
    bool modifyNode(QTreeWidgetItem* _pNode, const QString& _refName, const QString& _refCode);

public slots:
    void onTreeNodeClicked(QTreeWidgetItem* _pNode, int _nCol);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::CategoryDlg *ui;
    TreeWidgetUserDataMap m_xUserdataMap;
    QTreeWidgetItem* m_pRootTreeWidget;
};

#endif // CATEGORYDLG_H
