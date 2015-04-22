#include "gfunctions.h"
#include <stdio.h>

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
