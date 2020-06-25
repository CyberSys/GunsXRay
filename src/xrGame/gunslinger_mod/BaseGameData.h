#pragma once
#include "BaseDefs.h"

namespace GunslingerMod
{
TimeTicks GetGameTickCount();

TimeTicks GetTimeDeltaSafe(TimeTicks starttime);
TimeTicks GetTimeDeltaSafe(TimeTicks starttime, TimeTicks endtime);

bool GetNextSubStr(string& data, string& buf, char separator = 0);

void LogText(const string& text, bool IsError = false);
void LogText(pcstr text, bool IsError = false);

float random();
} // namespace GunslingerMod
