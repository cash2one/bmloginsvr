#include "CategoryManager.h"
#include <QFile>
#include <QTextStream>
#include "gfunctions.h"

CategoryManager::CategoryManager()
{

}

CategoryManager::~CategoryManager()
{
    close();
}

bool CategoryManager::load(const QString &_refPath)
{
    bool bNewFile = false;

    if(!fileExists(_refPath))
    {
        //  new xml file
        QDomDocument doc;
        QFile xFile(_refPath);
        if(!xFile.open(QFile::ReadWrite))
        {
            return false;
        }

        QDomProcessingInstruction xIns = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(xIns);

        QDomElement xRootEle = doc.createElement("root");
        doc.appendChild(xRootEle);

        QTextStream xStream(&xFile);
        doc.save(xStream, 3);
        xFile.close();

        bNewFile = true;
    }

    QFile xFile(_refPath);
    if(!xFile.open(QFile::ReadOnly))
    {
        return false;
    }

    m_xDoc.setContent(&xFile);
    m_xFilePath = _refPath;
    xFile.close();

    if(bNewFile)
    {
        newTestFile();
    }

    return true;
}

void CategoryManager::close()
{

}

void CategoryManager::save()
{
    if(m_xFilePath.isEmpty())
    {
        return;
    }

    QFile xFile(m_xFilePath);
    if(!xFile.open(QFile::ReadWrite))
    {
        return;
    }
    xFile.resize(0);

    QTextStream xStream(&xFile);
    m_xDoc.save(xStream, 3);
    xFile.close();
}

QDomElement CategoryManager::getElementByParent(const QStringList &_refParents)
{
    //  find parent node
    QDomElement xRootElement = m_xDoc.documentElement();
    QDomElement xDestElement = xRootElement;

    //  find first parent
    for(int i = 0; i < _refParents.size(); ++i)
    {
        QString refStrItem = _refParents[i];
        bool bFind = false;
        QDomElement xElement = xDestElement.firstChildElement();

        while(!xElement.isNull())
        {
            if(xElement.attribute("name") == refStrItem)
            {
                bFind = true;
                xDestElement = xElement;
                break;
            }

            xElement = xElement.nextSiblingElement();
        }

        if(!bFind)
        {
            return QDomElement();
        }
    }

    return xDestElement;
}

bool CategoryManager::addNode(const QStringList &_refParents, const QString &_refName, const QString &_refCode)
{
    //  find parent node
    QDomElement xDestElement = getElementByParent(_refParents);

    if(xDestElement.isNull())
    {
        return false;
    }

    QDomElement xNewItem = m_xDoc.createElement(QString("category_%1").arg(_refParents.size() + 1));
    xNewItem.setAttribute("name", _refName);
    xNewItem.setAttribute("code", _refCode);

    xDestElement.appendChild(xNewItem);

    return true;
}

bool CategoryManager::removeNode(const QStringList &_refParents)
{
    if(_refParents.size() == 0)
    {
        return false;
    }

    QDomElement xDestElement = getElementByParent(_refParents);
    if(xDestElement.isNull())
    {
        return false;
    }

    QDomNode xParentElement = xDestElement.parentNode();
    xParentElement.removeChild(xDestElement);

    return true;
}

bool CategoryManager::modifyNode(const QStringList &_refParents, const QString &_refName, const QString &_refCode)
{
    if(_refParents.size() == 0)
    {
        return false;
    }

    QDomElement xDestElement = getElementByParent(_refParents);
    if(xDestElement.isNull())
    {
        return false;
    }

    xDestElement.setAttribute("name", _refName);
    xDestElement.setAttribute("code", _refCode);

    return true;
}

bool CategoryManager::getCategoryByParent(const QStringList &_refParents, QStringList &_refOut)
{
    QDomElement xDestElement = getElementByParent(_refParents);
    if(xDestElement.isNull())
    {
        LOGDEBUG << "null dom element";
        return false;
    }

    //  遍历所有孩子节点
    QDomElement xChildElement = xDestElement.firstChildElement();
    QString xName;
    QString xCode;

    _refOut.clear();

    while(!xChildElement.isNull())
    {
        xName = xChildElement.attribute("name");
        xCode = xChildElement.attribute("code");
        _refOut << xName
                   << xCode;

        xChildElement = xChildElement.nextSiblingElement();
    }

    return true;
}

void CategoryManager::newTestFile()
{
    QStringList xParents;
    addNode(xParents, QStringLiteral("男装"), "MZ");
    addNode(xParents, QStringLiteral("女装"), "WZ");
    addNode(xParents, QStringLiteral("童装"), "TZ");

    xParents << QStringLiteral("男装");
    addNode(xParents, QStringLiteral("衣服"), "YF");
    addNode(xParents, QStringLiteral("裤子"), "KZ");
    addNode(xParents, QStringLiteral("鞋子"), "XZ");

    xParents.clear();
    xParents << QStringLiteral("女装");
    addNode(xParents, QStringLiteral("衣服"), "YF");
    addNode(xParents, QStringLiteral("裤子"), "KZ");
    addNode(xParents, QStringLiteral("鞋子"), "XZ");

    xParents.clear();
    xParents << QStringLiteral("童装");
    addNode(xParents, QStringLiteral("衣服"), "YF");
    addNode(xParents, QStringLiteral("裤子"), "KZ");
    addNode(xParents, QStringLiteral("鞋子"), "XZ");

    xParents.clear();
    xParents << QStringLiteral("男装")
             << QStringLiteral("衣服");
    addNode(xParents, QStringLiteral("衬衫"), "CS");
    addNode(xParents, QStringLiteral("外套"), "WT");
    addNode(xParents, QStringLiteral("棉服"), "MF");

    xParents.clear();
    xParents << QStringLiteral("女装")
             << QStringLiteral("衣服");
    addNode(xParents, QStringLiteral("衬衫"), "CS");
    addNode(xParents, QStringLiteral("外套"), "WT");
    addNode(xParents, QStringLiteral("棉服"), "MF");

    save();
}
