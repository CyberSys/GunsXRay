#pragma once

#include "BaseDefs.h"

class CWeapon;
class CHudItemObject;

namespace GunslingerMod
{
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

class WpnBuf
{
private:
    bool _is_weapon_explosed;

    CWeapon* _my_wpn;

    bool _laser_enabled;

    bool _is_alter_zoom_now;

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

    bool IsExplosed();
    void SetExplosed(bool status);

    void SetLaserEnabledStatus(bool status);

    void SwitchTorch(bool status, bool forced = false);

    bool IsAlterZoomMode();
    void SetAlterZoomMode(bool status);

    lens_zoom_params GetLensParams();
    void SetLensParams(lens_zoom_params params);
    void SetOffsetDir(float val);

    void SetNightBrightnessSavedStep(s32 val);
};

WpnBuf* GetBuffer(CHudItemObject* wpn);
bool IsExplosed(CHudItemObject* wpn);
} // namespace GunslingerMod
