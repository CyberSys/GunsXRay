#include "StdAfx.h"
#include "WeaponAdditionalBuffer.h"

#include "HudItemUtils.h"
#include "gunsl_config.h"
#include "BaseGameData.h"

#include "Weapon.h"

namespace GunslingerMod
{
WpnBuf* GetBuffer(CHudItemObject* wpn)
{
    WpnBuf* result = nullptr;
    if (WpnCanShoot(wpn))
    {
        result = static_cast<WpnBuf*>(EngineFriendWrapper::GetBuffer(static_cast<CWeapon*>(wpn)));
    }
    return result;
}

bool IsExplosed(CHudItemObject* wpn)
{
    WpnBuf* buf = GetBuffer(wpn);
    if (buf != nullptr)
        return buf->IsExplosed();
    return false;
}

void WpnBuf::_SetWpnBufPtr(CWeapon* wpn, WpnBuf* what_write) { EngineFriendWrapper::SetBuffer(wpn, what_write); }

WpnBuf::WpnBuf(CWeapon* wpn)
{
    pcstr sect = GetSection(wpn);

    _my_wpn = wpn;
    _is_weapon_explosed = false;
    _SetWpnBufPtr(wpn, this);

    _is_alter_zoom_now = false;

    _lens_zoom_params.factor_min = game_ini_r_single_def(sect, "min_lens_factor", 1);
    _lens_zoom_params.factor_max = game_ini_r_single_def(sect, "max_lens_factor", 1);
    _lens_zoom_params.speed = game_ini_r_single_def(sect, "lens_speed", 0);
    _lens_zoom_params.gyro_period = game_ini_r_single_def(sect, "lens_gyro_sound_period", 0);
    _lens_zoom_params.delta = 1 / game_ini_r_single_def(sect, "lens_factor_levels_count", 5);
    _lens_zoom_params.target_position = 1;

    _lens_zoom_params.last_gyro_snd_time = GetGameTickCount();
    _lens_zoom_params.real_position = 0;

    //параметры сбивающегося прицела
    _lens_offset.max_value = game_ini_r_single_def(GetSection(wpn), "lens_offset_max_val", 0.05);
    _lens_offset.start_condition = game_ini_r_single_def(GetSection(wpn), "lens_offset_start_condition", 0.5);
    _lens_offset.end_condition = game_ini_r_single_def(GetSection(wpn), "lens_offset_end_condition", 0.1);
    SetOffsetDir(random()); //смещение линзы (перезапишется в load в случае загрузки сейва)

    _lens_night_brightness.max_value = 1.0;
    _lens_night_brightness.min_value = 0.0;
    _lens_night_brightness.cur_value = 0.5;
    _lens_night_brightness.steps = 2;
    _lens_night_brightness.cur_step = 1;
    _lens_night_brightness.jitter = 0.1;
    _lens_night_brightness_saved_step = -1;
}

WpnBuf::~WpnBuf() { _SetWpnBufPtr(_my_wpn, nullptr); }

bool WpnBuf::IsExplosed() { return _is_weapon_explosed; }
void WpnBuf::SetExplosed(bool status) { _is_weapon_explosed = status; }

void WpnBuf::SetLaserEnabledStatus(bool status) { _laser_enabled = status; }

void WpnBuf::SwitchTorch(bool status, bool forced)
{
    // TODO: port
}

bool WpnBuf::IsAlterZoomMode() { return _is_alter_zoom_now; }
void WpnBuf::SetAlterZoomMode(bool status) { _is_alter_zoom_now = status; }

lens_zoom_params WpnBuf::GetLensParams() { return _lens_zoom_params; };
void WpnBuf::SetLensParams(lens_zoom_params params) { _lens_zoom_params = params; }
void WpnBuf::SetOffsetDir(float val) { _lens_offset.dir = val; }

void WpnBuf::SetNightBrightnessSavedStep(s32 val) { _lens_night_brightness_saved_step = val; }
} // namespace GunslingerMod
