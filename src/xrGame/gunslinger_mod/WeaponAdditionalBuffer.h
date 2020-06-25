#pragma once

#include "BaseDefs.h"

class CWeapon;
class CHudItemObject;

namespace GunslingerMod
{
struct conditional_breaking_params
{
    float start_condition; //при каком состоянии начнутся проблемы
    float end_condition; //при каком состоянии отрубится вообще
    float start_probability; //вероятность проблем в стартовом состоянии
};

struct lens_offset_params
{
    float dir;
    float max_value;
    float start_condition;
    float end_condition;
};

struct lens_zoom_params
{
    float delta;
    float target_position;
    float speed;
    float factor_min;
    float factor_max;
    float gyro_period;
    float real_position;
    TimeTicks last_gyro_snd_time;
};

struct stepped_params
{
    float max_value;
    float min_value;
    float cur_value;
    float cur_step;
    s32 steps;
    float jitter;
};

using TAnimationEffectorCb = void(CHudItemObject* wpn, s32 param);
using TAnimationEffector = TAnimationEffectorCb*;

class WpnBuf
{
private:
    bool _is_weapon_explosed;

    CWeapon* _my_wpn;

    TimeTicks _lock_remain_time;
    string _current_anim;

    bool _laser_enabled;
    bool _laser_installed;

    bool _torch_installed;

    float _actor_camera_speed;

    bool _is_alter_zoom_now;
    bool _is_alter_zoom_last;
    float _alter_zoom_direct_switch_mixup_factor;

    conditional_breaking_params _collimator_breaking;
    float _collimator_problems_level;

    bool _need_permanent_lensrender;
    lens_zoom_params _lens_zoom_params;

    //параметры смещения при поломке оружия - полярная с.к!
    lens_offset_params _lens_offset;
    stepped_params _lens_night_brightness;
    s32 _lens_night_brightness_saved_step;

    static void _SetWpnBufPtr(CWeapon* wpn, WpnBuf* what_write);

public:
    xr_vector<u8> ammos;

    WpnBuf(CWeapon* wpn);
    ~WpnBuf();

    bool Update();

    bool IsExplosed();
    void SetExplosed(bool status);

    string GetCurAnim();

    bool IsLaserInstalled();

    void InstallLaser(pcstr params_section);

    void SetLaserEnabledStatus(bool status);

    void InstallTorch(pcstr params_section);
    void SwitchTorch(bool status, bool forced = false);
    bool IsTorchInstalled();

    void SetCameraSpeed(float s);
    float GetCameraSpeed();

    bool IsAlterZoomMode();
    void SetAlterZoomMode(bool status);

    // Показывает, был ли последний вход в прицеливание "альтернативным" - надо для корректного выхода из
    // альтернативного прицеливания
    bool IsLastZoomAlter();

    float GetAlterZoomDirectSwitchMixupFactor();

    conditional_breaking_params GetCollimatorBreakingParams();

    bool NeedPermanentLensRendering();
    void SetPermanentLensRenderingStatus(bool status);
    lens_zoom_params GetLensParams();
    void SetLensParams(lens_zoom_params params);

    void SetOffsetDir(float val);

    void SetNightBrightnessSavedStep(s32 val);

    float GetCollimatorProblemsLevel();

    bool IsActionProcessing();
};

WpnBuf* GetBuffer(CHudItemObject* wpn);
bool IsExplosed(CHudItemObject* wpn);
bool IsActionProcessing(CHudItemObject* wpn);
string GetCurAnim(CHudItemObject* wpn);
} // namespace GunslingerMod
