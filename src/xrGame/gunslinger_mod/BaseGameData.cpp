#include "StdAfx.h"
#include "BaseGameData.h"
#include "EngineFriends.h"

namespace GunslingerMod
{
TimeTicks GetGameTickCount() { return EngineFriendWrapper::GetGameTickCount(); }

float random() { return EngineFriendWrapper::random(); }
} // namespace GunslingerMod
