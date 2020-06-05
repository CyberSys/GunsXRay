#include "StdAfx.h"
#include "PatchingInterface.h"
#include "WeaponDataSaveLoad.h"
#include "WeaponEvents.h"

namespace GunslingerMod
{
void PatchingInterface::CWeapon_class::load(CWeapon* wpn, IReader* packet)
{
    LoadAdditionalDataFromReader(wpn, packet);
}

void PatchingInterface::CWeapon_class::net_Spawn_middle(CWeapon* wpn) { CreateWpnBufIfNeeded(wpn); }
} // namespace GunslingerMod
