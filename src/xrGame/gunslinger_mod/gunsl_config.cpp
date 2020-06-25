#include "StdAfx.h"
#include "gunsl_config.h"
#include "EngineFriends.h"
#include "BaseDefs.h"
#include "BaseGameData.h"

namespace GunslingerMod
{
const pcstr GUNSL_BASE_SECTION = "gunslinger_base";

float dof_def_speed_out;
float dof_def_timeoffset;

pcstr _mod_ver;
pcstr _save_ver;
pcstr _addon_name;

//данные консольных команд
//булевские флаги
static u32 _console_bool_flags = 0;

static float _max_actor_cam_speed = 1;

//маски для флагов
static const u32 _mask_dyndof = 0x1;
static const u32 _mask_constzoomdof = 0x2;
static const u32 _mask_laserdotcorrection = 0x4;
static const u32 _mask_npclasers = 0x8;
static const u32 _mask_realballistics = 0x10;
static const u32 _mask_forcelense = 0x20;
static const u32 _mask_unlocksnd = 0x40;
static const u32 _mask_lens_enabled = 0x80;

bool game_ini_line_exist(pcstr section, pcstr key)
{
    return EngineFriendWrapper::ini_line_exist(EngineFriendWrapper::GetGameIni(), section, key);
}

bool game_ini_r_bool(pcstr section, pcstr key)
{
    return EngineFriendWrapper::ini_r_bool(EngineFriendWrapper::GetGameIni(), section, key);
}

float game_ini_r_single(pcstr section, pcstr key)
{
    return EngineFriendWrapper::ini_r_float(EngineFriendWrapper::GetGameIni(), section, key);
}

s32 game_ini_r_int(pcstr section, pcstr key)
{
    return EngineFriendWrapper::ini_r_int(EngineFriendWrapper::GetGameIni(), section, key);
}

pcstr game_ini_read_string(pcstr section, pcstr key)
{
    return EngineFriendWrapper::ini_r_string(EngineFriendWrapper::GetGameIni(), section, key);
}

bool game_ini_r_bool_def(pcstr section, pcstr key, bool def)
{
    if (game_ini_line_exist(section, key))
    {
        return game_ini_r_bool(section, key);
    }
    else
    {
        return def;
    }
}

float game_ini_r_single_def(pcstr section, pcstr key, float def)
{
    if (game_ini_line_exist(section, key))
    {
        return game_ini_r_single(section, key);
    }
    else
    {
        return def;
    }
}

s32 game_ini_r_int_def(pcstr section, pcstr key, s32 def)
{
    if (game_ini_line_exist(section, key))
    {
        return game_ini_r_int(section, key);
    }
    else
    {
        return def;
    }
}

Fvector3 game_ini_read_vector3_def(pcstr section, pcstr key, Fvector3* def)
{
    if (game_ini_line_exist(section, key))
    {
        Fvector3 result;

        string tmp = game_ini_read_string(section, key);
        string coord;

        GetNextSubStr(tmp, coord, ',');
        result.x = strtofloatdef(coord, 0);

        GetNextSubStr(tmp, coord, ',');
        result.y = strtofloatdef(coord, 0);

        GetNextSubStr(tmp, coord, ',');
        result.z = strtofloatdef(coord, 0);

        return result;
    }
    else
    {
        return *def;
    }
}

bool gunsl_config_Init()
{
    //для выхода из анимы
    dof_def_speed_out = game_ini_r_single_def(GUNSL_BASE_SECTION, "default_dof_speed_out", 1);
    //время выключени¤ дофа
    dof_def_timeoffset = game_ini_r_single_def(GUNSL_BASE_SECTION, "default_dof_time_offset", -0.4);

    _max_actor_cam_speed = game_ini_r_single_def(GUNSL_BASE_SECTION, "default_actor_camera_speed", 10);

    _mod_ver = game_ini_read_string(GUNSL_BASE_SECTION, "version");
    _save_ver = game_ini_read_string(GUNSL_BASE_SECTION, "save_version");
    _addon_name = game_ini_read_string(GUNSL_BASE_SECTION, "addon_name");

    return true;
}

float GetDefaultDOFSpeed_Out() { return dof_def_speed_out; }
float GetDefaultDOFTimeOffset() { return dof_def_timeoffset; }

float GetCamSpeedDef() { return _max_actor_cam_speed; }

bool IsLensEnabled() { return (_mask_lens_enabled & _console_bool_flags) != 0; }

pcstr GetModVer() { return _mod_ver; }

pcstr GetSaveVer() { return _save_ver; }

pcstr GetAddonName() { return _addon_name; }
} // namespace GunslingerMod
