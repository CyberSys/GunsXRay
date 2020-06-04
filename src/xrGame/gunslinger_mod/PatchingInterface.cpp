#include "StdAfx.h"
#include "PatchingInterface.h"
#include "WeaponDataSaveLoad.h"

namespace GunslingerMod
{
void PatchingInterface::CWeapon_class::load(CWeapon* wpn, IReader* packet)
{
    LoadAdditionalDataFromReader(wpn, packet);
}
} // namespace GunslingerMod
