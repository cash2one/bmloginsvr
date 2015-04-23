#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QStringList>
#include <QList>

struct ProductItem
{
    int nID;

    QString xSKUCode;
    QString xFstCategory;
    QString xSecCategory;
    QString xTrdCategory;
    int nSeq;
    QString xProductName;
    float fEstimateWeight;
    float fRealWeight;
    QString xNote;
    float fBuyMoney;
    QStringList xBuyDetails;
    QStringList xAttribDetails;

    int nInsertTime;

    ProductItem()
    {
        nID = 0;
        nInsertTime = 0;
        nSeq = 0;
        fEstimateWeight = 0;
        fRealWeight = 0;
        fBuyMoney = 0;
    }
};

typedef QList<ProductItem> ProductItemList;

class SqlManager
{
protected:
    SqlManager();

public:
    ~SqlManager();

    static SqlManager* getInstance()
    {
        static SqlManager s_ins;
        return &s_ins;
    }

    static bool readQueryValues(QSqlQuery& _refQuery, ProductItem& _refItem);
    static bool readQueryValuesAttrib(QSqlQuery& _refQuery, ProductItem& _refItem);

public:
    bool connect(const char* _pszSqlPath);
    bool connect(const QString& _refPath);
    void close();

    bool newProductItem(ProductItem& _refItem);
    bool delProductItem(QString& _refSKUCode);
    int getItemCount(const QString& _refSKUCode);

    bool getProductItems(ProductItemList& _refItems, int _nPage, int _nPerPage);
    bool getProductItem(ProductItem& _refItem, QString& _refSKUCode);

    int getMaxItemSeq(const QString& _refFstCategory,
                      const QString& _refSecCategory,
                      const QString& _refTrdCategory);

protected:
    QSqlDatabase m_xSql;
};

#endif // SQLMANAGER_H
