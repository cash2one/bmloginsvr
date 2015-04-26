#include "categorydlg.h"
#include "ui_categorydlg.h"
#include "CategoryManager.h"
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include "categorynewnodedlg.h"
#include <QMessageBox>
#include "gfunctions.h"

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
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onTreeNodeClicked(QTreeWidgetItem*,int)));

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

bool CategoryDlg::canAddNode(QTreeWidgetItem *_pParentNode, const QString &_refCode)
{
    QStringList xParentCategory;
    QTreeWidgetItem* pParentNode = _pParentNode;

    while(pParentNode->parent())
    {
        TreeWidgetUserDataMap::iterator fndIter = m_xUserdataMap.find(pParentNode);
        if(fndIter != m_xUserdataMap.end())
        {
            xParentCategory.push_front(fndIter.value());
        }
    }

    QStringList xOut;
    if(!CategoryManager::getInstance()->getCategoryByParent(xParentCategory, xOut))
    {
        return false;
    }

    //  检查是否有重复的code
    for(int i = 0; i < xOut.size(); ++i)
    {
        if(_refCode == xOut[i])
        {
            return false;
        }
    }

    return true;
}

bool CategoryDlg::modifyNode(QTreeWidgetItem *_pNode, const QString &_refName, const QString &_refCode)
{
    if(_refName.isEmpty() ||
            _refCode.isEmpty())
    {
        qDebug() << __FUNCTION__ << __LINE__ << " empty name or code";
        return false;
    }

    //  获取节点userdata
    QString xPrevCode;
    QString xPrevName = _pNode->text(0);
    TreeWidgetUserDataMap::iterator fndIter = m_xUserdataMap.find(_pNode);

    if(fndIter != m_xUserdataMap.end())
    {
        xPrevCode = fndIter.value();
    }
    else
    {
        LOGDEBUG << " failed to modify userdata";
        return false;
    }

    bool bCheckName = false;
    bool bCheckCode = false;

    if(_refName != xPrevName)
    {
        bCheckName = true;
    }
    if(_refCode != xPrevCode)
    {
        bCheckCode = true;
    }

    //  先写入数据
    QStringList xParentCategory;
    QTreeWidgetItem* pParentNode = _pNode->parent();

    while(pParentNode)
    {
        //  use code
        /*TreeWidgetUserDataMap::iterator fndIter = m_xUserdataMap.find(pParentNode);
        if(fndIter != m_xUserdataMap.end())
        {
            xParentCategory.push_front(fndIter.value());
        }*/
        //  use name
        if(pParentNode != m_pRootTreeWidget)
        {
            xParentCategory.push_front(pParentNode->text(0));
        }

        pParentNode = pParentNode->parent();
    }

    QStringList xOut;
    if(!CategoryManager::getInstance()->getCategoryByParent(xParentCategory, xOut))
    {
        LOGDEBUG << " can't getCategoryByParent" << xParentCategory;
        return false;
    }

    //  检查是否有重复的code和name
    for(int i = 0; i < xOut.size(); ++i)
    {
        if(bCheckCode &&
                i % 2 == 1 &&
                _refCode == xOut[i])
        {
            LOGDEBUG << " same code";
            return false;
        }

        if(bCheckName &&
                i % 2 == 0 &&
                _refName == xOut[i])
        {
            LOGDEBUG << " same name";
            return false;
        }
    }

    //  修改xml数据 首先加入最后一项
    xParentCategory << _pNode->text(0);
    if(!CategoryManager::getInstance()->modifyNode(xParentCategory, _refName, _refCode))
    {
        LOGDEBUG << " modify dom document failed.";
        return false;
    }

    CategoryManager::getInstance()->save();

    //  修改节点userdata
    fndIter = m_xUserdataMap.find(_pNode);
    if(fndIter != m_xUserdataMap.end())
    {
        fndIter.value() = _refCode;
    }
    else
    {
        LOGDEBUG << " failed to modify userdata";
        return false;
    }

    return true;
}

bool CategoryDlg::addNode(QTreeWidgetItem *_pParentNode, const QString &_refName, const QString &_refCode)
{
    if(_refName.isEmpty() ||
            _refCode.isEmpty())
    {
        qDebug() << __FUNCTION__ << __LINE__ << " empty name or code";
        return false;
    }

    //  先写入数据
    QStringList xParentCategory;
    QTreeWidgetItem* pParentNode = _pParentNode;

    int nParentLevel = 0;
    while(pParentNode)
    {
        //  use code
        /*TreeWidgetUserDataMap::iterator fndIter = m_xUserdataMap.find(pParentNode);
        if(fndIter != m_xUserdataMap.end())
        {
            xParentCategory.push_front(fndIter.value());
        }*/
        //  use name
        if(pParentNode != m_pRootTreeWidget)
        {
            xParentCategory.push_front(pParentNode->text(0));
        }

        pParentNode = pParentNode->parent();
        ++nParentLevel;
    }

    if(nParentLevel > 3)
    {
        LOGDEBUG << " parent level > 3 : " << nParentLevel;
        return false;
    }

    QStringList xOut;
    if(!CategoryManager::getInstance()->getCategoryByParent(xParentCategory, xOut))
    {
        LOGDEBUG << " can't getCategoryByParent" << xParentCategory;
        return false;
    }

    //  检查是否有重复的code和name
    for(int i = 0; i < xOut.size(); ++i)
    {
        if(i % 2 == 1 &&
                _refCode == xOut[i])
        {
            LOGDEBUG << " same code";
            return false;
        }

        if(i % 2 == 0 &&
                _refName == xOut[i])
        {
            LOGDEBUG << " same name";
            return false;
        }
    }

    if(!CategoryManager::getInstance()->addNode(xParentCategory, _refName, _refCode))
    {
        LOGDEBUG << " addnode to dom document failed.";
        return false;
    }

    QTreeWidgetItem* pItem = new QTreeWidgetItem(_pParentNode, QStringList(_refName));
    _pParentNode->addChild(pItem);
    m_xUserdataMap.insert(pItem, _refCode);

    CategoryManager::getInstance()->save();
    return true;
}


//  slots
void CategoryDlg::onTreeNodeClicked(QTreeWidgetItem *_pNode, int _nCol)
{
    TreeWidgetUserDataMap::iterator fndIter = m_xUserdataMap.find(_pNode);
    if(fndIter != m_xUserdataMap.end())
    {
        QString xInfo = QStringLiteral("名称:%1 代号:%2").arg(_pNode->text(_nCol)).arg(fndIter.value());
        ui->label_nodeInfo->setText(xInfo);
    }
    else
    {
        ui->label_nodeInfo->setText(QStringLiteral("根节点"));
    }
}

void CategoryDlg::on_pushButton_clicked()
{
    QTreeWidgetItem* pNode = ui->treeWidget->currentItem();
    if(NULL == pNode)
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择要添加的父节点后再进行添加节点操作"));
        return;
    }

    //  三级节点无法添加子节点
    QTreeWidgetItem* pParentNode = pNode->parent();

    int nParentLevel = 0;
    while(pParentNode)
    {
        pParentNode = pParentNode->parent();
        ++nParentLevel;
    }

    if(nParentLevel >= 3)
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("第三级目录无法添加子节点"));
        return;
    }

    CategoryNewNodeDlg dlg;
    if(QDialog::Accepted == dlg.exec())
    {
        if(!addNode(pNode, dlg.getName(), dlg.getCode()))
        {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("添加节点失败。请确认输入正确，可能是由于当前父节点下有同代号的子节点"));
        }
    }
}

void CategoryDlg::on_pushButton_3_clicked()
{
    QTreeWidgetItem* pNode = ui->treeWidget->currentItem();
    if(NULL == pNode)
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择要修改的节点进行修改"));
        return;
    }

    if(m_pRootTreeWidget == pNode)
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无法修改根节点数据"));
        return;
    }

    CategoryNewNodeDlg dlg;
    dlg.setMode(1);

    QString xDefaultCode;
    QString xDefaultName = pNode->text(0);

    TreeWidgetUserDataMap::iterator fndIter = m_xUserdataMap.find(pNode);
    if(fndIter != m_xUserdataMap.end())
    {
        xDefaultCode = fndIter.value();
    }
    else
    {
        LOGDEBUG << " failed to get userdata";
        return;
    }

    dlg.setDefaultCode(xDefaultCode);
    dlg.setDefaultName(xDefaultName);

    if(QDialog::Accepted == dlg.exec())
    {
        if(dlg.getName() != xDefaultName ||
                dlg.getCode() != xDefaultCode)
        {
            if(!modifyNode(pNode, dlg.getName(), dlg.getCode()))
            {
                QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("修改节点失败。请确认输入正确，可能是由于当前父节点下有同代号的子节点"));
            }
            else
            {
                QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("修改成功"));
                onTreeNodeClicked(pNode, 0);
            }
        }
    }
}
