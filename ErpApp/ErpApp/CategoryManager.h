#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H

#include <QString>
#include "gfunctions.h"
#include <QtXml/QDomDocument>
#include <QStringList>

class CategoryManager
{
public:
    static CategoryManager* getInstance()
    {
        static CategoryManager s_ins;
        return &s_ins;
    }

protected:
    CategoryManager();

public:
    ~CategoryManager();

public:
    bool load(const QString& _refPath);
    void close();
    void save();

    QDomElement getElementByParent(const QStringList& _refParents);
    bool addNode(const QStringList& _refParents, const QString& _refName, const QString& _refCode);
    bool removeNode(const QStringList& _refParents);
    bool modifyNode(const QStringList& _refParents, const QString& _refName, const QString& _refCode);
    bool getCategoryByParent(const QStringList& _refParents, QStringList& _refOut);

    QDomDocument& getDocument()
    {
        return m_xDoc;
    }

private:
    void newTestFile();

protected:
    QDomDocument m_xDoc;
    QString m_xFilePath;
};

#endif // CATEGORYMANAGER_H
