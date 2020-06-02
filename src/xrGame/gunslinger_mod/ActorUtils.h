#pragma once

#include "Level.h"
#include "Actor.h"
#include "hud_item_object.h"
namespace GunslingerMod
{
CActor* GetActor();
CHudItemObject* GetActorActiveItem();
CInventoryItem* ItemInSlot(CActor* act, int slot);
int GetActorActiveSlot();
} // namespace GunslingerMod
