#ifndef GFUNCTIONS_H
#define GFUNCTIONS_H
/////////////////////////////////////////////////////
#include <QString>
#include <QTime>

const char* getSKUNumber(const char* _pszSelect1, const char* _pszSelect2, const char* _pszSelect3);
void internalSleep(int _nMs);
const QString& getAppPath();
bool fileExists(const QString& _refPath);

#endif // GFUNCTIONS_H

