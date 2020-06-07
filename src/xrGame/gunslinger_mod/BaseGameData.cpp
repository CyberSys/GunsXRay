#include "StdAfx.h"
#include "BaseGameData.h"
#include "EngineFriends.h"

namespace GunslingerMod
{
TimeTicks GetGameTickCount() { return EngineFriendWrapper::GetGameTickCount(); }
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
