#pragma once
#include "BaseDefs.h"

namespace GunslingerMod
{
TimeTicks GetGameTickCount();

void LogText(const string& text, bool IsError = false);
void LogText(pcstr text, bool IsError = false);

float random();
} // namespace GunslingerMod
