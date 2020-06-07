#include "StdAfx.h"

#include "BaseDefs.h"

namespace GunslingerMod
{
string inttostr(s32 n)
{
    char b[20];
    xr_sprintf(b, "%d", n);
    return string(b);
}

string inttostr(u32 n)
{
    char b[20];
    xr_sprintf(b, "%d", n);
    return string(b);
}

string inttohex(u32 n, u32 cnt)
{
    char b[20];
    string fmtstr = string("%0") + inttostr(cnt) + "x";
    xr_sprintf(b, fmtstr.c_str(), n);
    return string(b);
}
} // namespace GunslingerMod
