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

string inttohex(u64 n, u32 cnt)
{
    char b[20];
    string fmtstr = string("%0") + inttostr(cnt) + "x";
    xr_sprintf(b, fmtstr.c_str(), n);
    return string(b);
}

string ptrtohex(void* n)
{
    char b[20];
    string fmtstr = string("%p");
    xr_sprintf(b, fmtstr.c_str(), n);
    return string(b);
}

float strtofloatdef(pcstr str, float def)
{
    float outval = def;
    if (sscanf(str, "%f", &outval) < 1)
    {
        return def;
    }
    return outval;
}

float strtofloatdef(string str, float def) { return strtofloatdef(str.c_str(), def); }

string leftstr(const string& str, u32 n) { return str.substr(0, n); }
string leftstr(pcstr str, u32 n) { return leftstr(string(str), n); }

string rightstr(const string& str, u32 n)
{
    if (str.length() < n)
        n = str.length();
    return str.substr(str.length() - n, n);
}
string rightstr(pcstr str, u32 n) { return rightstr(string(str), n); }
u32 length(string str) { return str.length(); }
u32 length(pcstr str) { return string(str).length(); }

string trim_right(const string& str)
{
    size_t trimmed_cnt = length(str);

    for (; trimmed_cnt > 0; --trimmed_cnt)
    {
        if (str[trimmed_cnt - 1] == ' ')
            trimmed_cnt--;
        else
            break;
    }

    if (trimmed_cnt == 0)
        return "";
    else
        return leftstr(str, trimmed_cnt);
}

string trim_left(const string& str)
{
    size_t trimmed_cnt = length(str);

    for (size_t i = 0; (i < length(str)) && (trimmed_cnt > 0); ++i)
    {
        if (str[i] == ' ')
            trimmed_cnt--;
        else
            break;
    }

    if (trimmed_cnt == 0)
        return "";
    else
        return rightstr(str, trimmed_cnt);
}
string trim(const string& str) { return trim_right(trim_left(str)); }
string trim(pcstr str) { return trim(string(str)); }
} // namespace GunslingerMod
