#include "SqlManager.h"
#include "gfunctions.h"
#include <string>
#include <QDebug>
#include <QDateTime>
#include <QSqlError>

using std::string;

SqlManager::SqlManager()
{

}

SqlManager::~SqlManager()
{
    close();
}

bool SqlManager::connect(const QString &_refPath)
{
    string xPath = _refPath.toStdString();
    return connect(xPath.c_str());
}

void SqlManager::close()
{
    if(m_xSql.isOpen())
    {
        m_xSql.close();
    }
}

bool SqlManager::connect(const char *_pszSqlPath)
{
    //  首先检测文件是否存在
    bool bSqlExists = fileExists(_pszSqlPath);

    m_xSql = QSqlDatabase::addDatabase("QSQLITE");
    m_xSql.setDatabaseName(_pszSqlPath);

    if(!m_xSql.open())
    {
        return false;
    }

    if(!bSqlExists)
    {
        //  新的文件，创建表
        QSqlQuery xSqlNewTable;
        QString xNewTableExpr = "create table product(id integer primary key,"\
                "skucode varchar(20),"\
                "fstcategory varchar(20),"\
                "seccategory varchar(20),"\
                "trdcategory varchar(20),"\
                "seq integer,"\
                "productname varchar(100),"\
                "estimateweight float,"\
                "realweight float,"\
                "buymoney float,"\
                "note varchar(1024),"\
                "inserttime integer"\
                ")";
        if(!xSqlNewTable.exec(xNewTableExpr))
        {
            qDebug() << "create table error:" << xNewTableExpr;
            return false;
        }
    }

    return true;
}

bool SqlManager::newProductItem(ProductItem &_refItem)
{
    int nCount = getItemCount(_refItem.xSKUCode);
    if(0 != nCount)
    {
        qDebug() << "SKUCode " << _refItem.xSKUCode << " count:" << QString::number(nCount);
        return false;
    }

    //  insert a record
    QString xSqlExpr = "insert into product (skucode, fstcategory, seccategory, trdcategory, seq, productname, estimateweight, realweight, buymoney, note, inserttime) values(";
    QString xFullExpr = xSqlExpr + "'" + _refItem.xSKUCode + "',"
            + "'" + _refItem.xFstCategory + "',"
            + "'" + _refItem.xSecCategory + "',"
            + "'" + _refItem.xTrdCategory + "',"
            + QString::number(_refItem.nSeq) + ","
            + "'" + _refItem.xProductName + "',"
            + QString::number(_refItem.fEstimateWeight) + ","
            + QString::number(_refItem.fRealWeight) + ","
            + QString::number(_refItem.fBuyMoney) + ","
            + "'" + _refItem.xNote + "', "
            + QString::number(QDateTime::currentDateTime().toTime_t())
            + ")"
            ;

    QSqlQuery xQuery;
    if(!xQuery.exec(xFullExpr))
    {
        qDebug() << "sql expression error:" << xFullExpr << xQuery.lastError();
        return false;
    }

    qDebug() << "new product item inserttime:" << _refItem.nInsertTime;
    return true;
}

bool SqlManager::delProductItem(QString &_refSKUCode)
{
    return true;
}

int SqlManager::getItemCount(const QString &_refSKUCode)
{
    QString xExpr = "select id from product where skucode='" + _refSKUCode + "'";
    QSqlQuery xQuery;

    if(!xQuery.exec(xExpr))
    {
        qDebug() << "sql expression error:" << xExpr;
        return -1;
    }

    int nCount = 0;
    while(xQuery.next())
    {
        ++nCount;
    }

    return nCount;
}

bool SqlManager::getProductItems(ProductItemList& _refItems, int _nPage, int _nPerPage)
{
    int nOffset = _nPerPage * _nPage;
    QString xExpr = "select * from product limit %1 offset %2";
    xExpr = xExpr.arg(_nPerPage).arg(nOffset);

    QSqlQuery xQuery;
    if(!xQuery.exec(xExpr))
    {
        qDebug() << "sql expression error:" << xExpr << xQuery.lastError();
        return false;
    }

    while(xQuery.next())
    {
        ProductItem item;
        item.xAttribDetails.clear();
        item.xBuyDetails.clear();
        readQueryValues(xQuery, item);
        _refItems.push_back(item);
    }

    return true;
}

bool SqlManager::getProductItem(ProductItem& _refItem, QString &_refSKUCode)
{
    QString xExpr = "select * from product where skucode='" + _refSKUCode + "'";

    QSqlQuery xQuery;
    if(!xQuery.exec(xExpr))
    {
        qDebug() << "sql expression error:" << xExpr << xQuery.lastError();
        return false;
    }

    if(xQuery.size() == 0)
    {
        return false;
    }

    if(!xQuery.next())
    {
        return false;
    }

    if(!readQueryValues(xQuery, _refItem))
    {
        return false;
    }

    return true;
}

bool SqlManager::readQueryValues(QSqlQuery &_refQuery, ProductItem &_refItem)
{
    _refItem.nID = _refQuery.value(0).toInt();
    _refItem.xSKUCode = _refQuery.value(1).toString();
    _refItem.xFstCategory = _refQuery.value(2).toString();
    _refItem.xSecCategory = _refQuery.value(3).toString();
    _refItem.xTrdCategory = _refQuery.value(4).toString();
    _refItem.nSeq = _refQuery.value(5).toInt();
    _refItem.xProductName = _refQuery.value(6).toString();
    _refItem.fEstimateWeight = _refQuery.value(7).toFloat();
    _refItem.fRealWeight = _refQuery.value(8).toFloat();
    _refItem.fBuyMoney = _refQuery.value(9).toFloat();
    _refItem.xNote = _refQuery.value(10).toString();
    _refItem.nInsertTime = _refQuery.value(11).toInt();

    return true;
}

int SqlManager::getMaxItemSeq(const QString &_refFstCategory, const QString &_refSecCategory, const QString &_refTrdCategory)
{
    if(_refFstCategory.isEmpty() ||
            _refSecCategory.isEmpty() ||
            _refTrdCategory.isEmpty())
    {
        return -1;
    }

    QString xExpr = "select max(seq) from product where fstcategory='"\
            + _refFstCategory + "' and "\
            "seccategory='" + _refSecCategory + "' and "\
            "trdcategory='" + _refTrdCategory + "'";
    QSqlQuery xQuery;
    if(!xQuery.exec(xExpr))
    {
        qDebug() << "sql expression error:" << xExpr << xQuery.lastError();
        return -1;
    }

    xQuery.next();
    return xQuery.value(0).toInt();
}
