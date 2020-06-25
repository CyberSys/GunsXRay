#include "StdAfx.h"
#include "WeaponAdditionalBuffer.h"

#include "HudItemUtils.h"
#include "gunsl_config.h"
#include "BaseGameData.h"
#include "ActorUtils.h"
#include "ControllerMonster.h"

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

    // ВНИМАНИЕ! Метод load выполняется раньше применения апгрейдов, а нас могут вызвать именно из него при загрузке
    // игры! Из-за этого все вносимые апгрейдами изменения (вроде допприцелов или типов патронов) будут еще невалидны!
    // Поэтому использовать что-то, что может поменяться при установке апгрейдов, в этом конструкторе НЕЛЬЗЯ!

    _my_wpn = wpn;
    _lock_remain_time = 0;
    _current_anim = "";
    _is_weapon_explosed = false;

    _SetWpnBufPtr(wpn, this);

    _laser_enabled = (random() > 0.5);
    _laser_installed = false;

    _torch_installed = false;

    _actor_camera_speed = game_ini_r_single_def(GetSection(wpn), "actor_camera_speed_factor", 1.0) * GetCamSpeedDef();

    _is_alter_zoom_now = false;
    _is_alter_zoom_last = false;

    Fvector3 tmpvec;
    tmpvec.set(-1, -1, 0);

    tmpvec = game_ini_read_vector3_def(GetSection(wpn), "collimator_breaking_params", &tmpvec);
    _collimator_breaking.start_condition = tmpvec.x;
    _collimator_breaking.end_condition = tmpvec.y;
    _collimator_breaking.start_probability = tmpvec.z;
    _collimator_problems_level = game_ini_r_single_def(GetSection(wpn), "collimator_problems_level", 0);

    _need_permanent_lensrender = game_ini_r_bool_def(GetHUDSection(wpn), "permanent_lens_render", false);
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

bool WpnBuf::Update()
{
    // TODO: Port
    return true;
}

bool WpnBuf::IsExplosed() { return _is_weapon_explosed; }
void WpnBuf::SetExplosed(bool status) { _is_weapon_explosed = status; }

string WpnBuf::GetCurAnim()
{
    if (_current_anim != "")
        return _current_anim;
    else
        return GetActualCurrentAnim(_my_wpn);
}

bool WpnBuf::IsLaserInstalled() { return _laser_installed; }

void WpnBuf::InstallLaser(pcstr params_section)
{
    // TODO: port
}

void WpnBuf::SetLaserEnabledStatus(bool status) { _laser_enabled = status; }

void WpnBuf::InstallTorch(pcstr params_section)
{
    // TODO: port
}

void WpnBuf::SwitchTorch(bool status, bool forced)
{
    // TODO: port
}

bool WpnBuf::IsTorchInstalled() { return _torch_installed; }

void WpnBuf::SetCameraSpeed(float s) { _actor_camera_speed = s; }
float WpnBuf::GetCameraSpeed() { return _actor_camera_speed; }

bool WpnBuf::IsAlterZoomMode() { return _is_alter_zoom_now; }
void WpnBuf::SetAlterZoomMode(bool status) { _is_alter_zoom_now = status; }

bool WpnBuf::IsLastZoomAlter() { return _is_alter_zoom_last; }

float WpnBuf::GetAlterZoomDirectSwitchMixupFactor() { return _alter_zoom_direct_switch_mixup_factor; }

conditional_breaking_params WpnBuf::GetCollimatorBreakingParams() { return _collimator_breaking; }

bool WpnBuf::NeedPermanentLensRendering() { return _need_permanent_lensrender; }
void WpnBuf::SetPermanentLensRenderingStatus(bool status) { _need_permanent_lensrender = status; }
lens_zoom_params WpnBuf::GetLensParams() { return _lens_zoom_params; };
void WpnBuf::SetLensParams(lens_zoom_params params) { _lens_zoom_params = params; }
void WpnBuf::SetOffsetDir(float val) { _lens_offset.dir = val; }

void WpnBuf::SetNightBrightnessSavedStep(s32 val) { _lens_night_brightness_saved_step = val; }

float WpnBuf::GetCollimatorProblemsLevel()
{
    return ModifyFloatUpgradedValue(_my_wpn, "collimator_problems_level", _collimator_problems_level);
}

bool WpnBuf::IsActionProcessing()
{
    return (((GetOwner(_my_wpn) != nullptr) && (_lock_remain_time > 0)) ||
        ((GetOwner(_my_wpn) != nullptr) && (GetOwner(_my_wpn) == static_cast<IGameObject*>(GetActor())) &&
            (IsActorSuicideNow() || IsActorPlanningSuicide())));
}

bool IsActionProcessing(CHudItemObject* wpn)
{
    WpnBuf* buf = GetBuffer(wpn);
    if (buf != nullptr)
    {
        return buf->IsActionProcessing();
    }
    else
    {
        return false;
    }
}

string GetCurAnim(CHudItemObject* wpn)
{
    WpnBuf* buf = GetBuffer(wpn);
    if (buf != nullptr)
        return buf->GetCurAnim();
    else
        return "";
}
} // namespace GunslingerMod
