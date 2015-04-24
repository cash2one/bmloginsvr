#include "categorydlg.h"
#include "ui_categorydlg.h"
#include "CategoryManager.h"
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

CategoryDlg::CategoryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDlg)
{
    m_pRootTreeWidget = NULL;
    ui->setupUi(this);

    createWidgets();
}

CategoryDlg::~CategoryDlg()
{
    delete ui;
}

void CategoryDlg::createWidgets()
{
    ui->treeWidget->setHeaderHidden(true);

    createFromLocalData();
}

void CategoryDlg::createFromLocalData()
{
    //  root item
    m_pRootTreeWidget = new QTreeWidgetItem(ui->treeWidget, QStringList(QStringLiteral("根目录")));

    /*QStringList xCategorys;
    QStringList xOutL0;
    if(!CategoryManager::getInstance()->getCategoryByParent(xCategorys, xOutL0))
    {
        qDebug() << "create tree failed" << __FUNCTION__ << __LINE__;
        return;
    }*/

    //  直接使用原始xml数据加载
    QDomDocument& refDoc = CategoryManager::getInstance()->getDocument();
    QDomElement xRootElement = refDoc.documentElement();

    QDomElement xFirstElement = xRootElement.firstChildElement();
    createNode(m_pRootTreeWidget, xFirstElement);

    ui->treeWidget->expandAll();
}


//  递归生成树
void CategoryDlg::createNode(QTreeWidgetItem *_pParentNode, QDomElement &_refDoc)
{
    if(_refDoc.isNull())
    {
        return;
    }

    QDomElement xElement = _refDoc;
    QString xName;
    QString xCode;

    while(!xElement.isNull())
    {
        //  create this node
        xName = xElement.attribute("name");
        xCode = xElement.attribute("code");
        if(!xName.isEmpty() &&
                !xCode.isEmpty())
        {
            QTreeWidgetItem* pItem = new QTreeWidgetItem(_pParentNode, QStringList(xName));
            _pParentNode->addChild(pItem);
            m_xUserdataMap.insert(pItem, xCode);

            if(xElement.hasChildNodes())
            {
                QDomElement xChildElement = xElement.firstChildElement();
                createNode(pItem, xChildElement);
            }
        }

        xElement = xElement.nextSiblingElement();
    }
}
