#include "StdAfx.h"
#include "WeaponEvents.h"
#include "HudItemUtils.h"
#include "WeaponAdditionalBuffer.h"

namespace GunslingerMod
{
void CreateWpnBufIfNeeded(CWeapon* wpn)
{
    if (WpnCanShoot(wpn))
    {
        //буфер может уже быть создан в load'e - проверим это
        if (GetBuffer(wpn) == nullptr)
        {
            new WpnBuf(wpn);
        }
    }
}

void OnWeaponExplode_AfterAnim(CWeapon* wpn, s32 param)
{
    // TODO:port
}
} // namespace GunslingerMod
