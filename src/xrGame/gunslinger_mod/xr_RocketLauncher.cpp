#include "StdAfx.h"
#include "xr_RocketLauncher.h"
#include "EngineFriends.h"
#include "Misc.h"

#include "CustomRocket.h"

namespace GunslingerMod
{
u32 GetRocketsCount(CRocketLauncher* rl) { return EngineFriendWrapper::GetRocketsCount(rl); }

CCustomRocket* GetRocket(CRocketLauncher* rl, u32 index) { return EngineFriendWrapper::GetRocket(rl, index); }

void UnloadRockets(CRocketLauncher* rl)
{
    u32 cnt = GetRocketsCount(rl);
    if (cnt == 0)
        return;

    for (u32 i = 0; i < cnt; ++i)
    {
        CCustomRocket* r = GetRocket(rl, i);
        alife_release(get_server_object_by_id(GetCObjectID(r)));
    }
    EngineFriendWrapper::DropRockets(rl);
}

void DetachRocket(CRocketLauncher* rl, u16 rocket_id, bool bLaunch)
{
    EngineFriendWrapper ::DetachRocket(rl, rocket_id, bLaunch);
}

void CWeaponMagazinedWGrenade__LaunchGrenade(CWeaponMagazinedWGrenade* wpn) { EngineFriendWrapper::LaunchGrenade(wpn); }

void virtual_CCustomRocket__Contact(CCustomRocket* r, Fvector3* pos, Fvector3* normal) {}
} // namespace GunslingerMod
