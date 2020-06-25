#include "StdAfx.h"
#include "BaseGameData.h"
#include "EngineFriends.h"

namespace GunslingerMod
{
TimeTicks GetGameTickCount() { return EngineFriendWrapper::GetGameTickCount(); }

TimeTicks GetTimeDeltaSafe(TimeTicks starttime)
{
    TimeTicks curtime = GetGameTickCount();
    TimeTicks result = curtime - starttime;
    //обработаем переполнение
    if (result > curtime)
    {
        result = (TimeTicks)(-1) - starttime + curtime;
    }
    return result;
}
TimeTicks GetTimeDeltaSafe(TimeTicks starttime, TimeTicks endtime)
{
    TimeTicks result = endtime - starttime;
    //обработаем переполнение
    if (result > endtime)
    {
        result = (TimeTicks)(-1) - starttime + endtime;
    };
    return result;
}

bool GetNextSubStr(string& data, string& buf, char separator)
{
    u32 p = 0;
    for (size_t i = 0; i < length(data); ++i)
    {
        if (data[i] == separator)
        {
            p = i + 1;
            break;
        };
    }

    if (p > 0)
    {
        buf = leftstr(data, p - 1);
        buf = trim(buf);
        data = rightstr(data, length(data) - p);
        data = trim(data);
        return true;
    }
    else
    {
        if (length(trim(data)) > 0)
        {
            buf = trim(data);
            data = "";
            return true;
        }
        else
            return false;
    }
}

void LogText(const string& text, bool IsError) { LogText(text.c_str(), IsError); }
void LogText(pcstr text, bool IsError)
{
    string paramText = string("GNSLWP: ") + text;
    if (IsError)
    {
        paramText = string("! ") + paramText;
    }
    else
    {
        paramText = string("~ ") + paramText;
    }
    EngineFriendWrapper::LogText(paramText.c_str());
}

float random() { return EngineFriendWrapper::random(); }
} // namespace GunslingerMod
