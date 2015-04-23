#include "gfunctions.h"
#include <stdio.h>
#include <QApplication>
#include <QFileInfo>
#include <unistd.h>
#include <string>

using std::string;

////////////////////////////////////////////////////////
const char* getSKUNumber(const char *_pszSelect1, const char *_pszSelect2, const char *_pszSelect3)
{
    static char s_szStringBuffer[256] = {0};

    const char* szCategory1 = _pszSelect1;
    const char* szCategory2 = _pszSelect2;
    const char* szCategory3 = _pszSelect3;

    int nProductIndex = 1;

    sprintf(s_szStringBuffer, "%s%s%s%d", szCategory1, szCategory2, szCategory3, nProductIndex);
    return s_szStringBuffer;
}

void internalSleep(int _nMs)
{
    QTime t;
    t.start();

    while(t.elapsed() < _nMs)
    {
        QCoreApplication::processEvents();
    }
}

const QString& getAppPath()
{
    static QString s_xAppPath;

    if(s_xAppPath.isEmpty())
    {
        s_xAppPath = QApplication::applicationFilePath();
        QFileInfo fi(s_xAppPath);
        s_xAppPath = fi.absolutePath();
    }

    return s_xAppPath;
}

bool fileExists(const QString &_refPath)
{
    string xPath = _refPath.toStdString();
    if(0 != access(xPath.c_str(), 0))
    {
        return false;
    }
    return true;
}
