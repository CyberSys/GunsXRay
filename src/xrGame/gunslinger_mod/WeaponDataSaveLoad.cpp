#include "StdAfx.h"
#include "WeaponDataSaveLoad.h"
#include "HudItemUtils.h"
#include "WeaponAdditionalBuffer.h"
#include "WeaponEvents.h"
#include "EngineFriends.h"

#include "Weapon.h"

namespace GunslingerMod
{
void LoadAdditionalDataFromReader(CWeapon* wpn, IReader* packet)
{
    if (!WpnCanShoot(wpn))
        return;

    WpnBuf* buf = GetBuffer(wpn);
    if (buf == nullptr)
    {
        buf = new WpnBuf(wpn);
    }

    u8 tmp_bool = 0;
    EngineFriendWrapper::ReadFromReader(packet, &tmp_bool, sizeof(tmp_bool));
    buf->SetLaserEnabledStatus(tmp_bool);

    EngineFriendWrapper::ReadFromReader(packet, &tmp_bool, sizeof(tmp_bool));
    buf->SwitchTorch(tmp_bool);

    EngineFriendWrapper::ReadFromReader(packet, &tmp_bool, sizeof(tmp_bool));
    SetWeaponMisfireStatus(wpn, tmp_bool);

    EngineFriendWrapper::ReadFromReader(packet, &tmp_bool, sizeof(tmp_bool));
    buf->SetExplosed(tmp_bool);

    lens_zoom_params lens_params = buf->GetLensParams();
    EngineFriendWrapper::ReadFromReader(packet, &lens_params.target_position, sizeof(lens_params.target_position));
    lens_params.real_position = lens_params.target_position;
    buf->SetLensParams(lens_params);

    float tmp_single;
    EngineFriendWrapper::ReadFromReader(packet, &tmp_single, sizeof(tmp_single));
    buf->SetOffsetDir(tmp_single);

    // ReloadNightBrightnessParams и подобные ему вызывать здесь нельзя - у нас еще не прогрузились и не установились
    // апгрейды, прицела c нужным индексом может тупо не быть!
    s32 tmp_int;
    EngineFriendWrapper::ReadFromReader(packet, &tmp_int, sizeof(tmp_int));
    buf->SetNightBrightnessSavedStep(tmp_int);

    u16 ammos_in_mag = 0;
    EngineFriendWrapper::ReadFromReader(packet, &ammos_in_mag, sizeof(ammos_in_mag));
    buf->ammos.resize(ammos_in_mag);

    u16 cnt_total = 0;
    while (cnt_total < ammos_in_mag)
    {
        u16 cnt = 0;
        u8 ammotype = 0;
        EngineFriendWrapper::ReadFromReader(packet, &cnt, sizeof(cnt));
        EngineFriendWrapper::ReadFromReader(packet, &ammotype, sizeof(ammotype));
        for (u16 i = cnt_total; i < cnt_total + cnt - 1; ++i)
        {
            buf->ammos[i] = ammotype;
        }
        cnt_total = cnt_total + cnt;
    }

    if (IsExplosed(wpn))
    {
        OnWeaponExplode_AfterAnim(wpn, 0);
    }
}
} // namespace GunslingerMod
