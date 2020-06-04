#include "StdAfx.h"
#include "gunsl_config.h"
#include "EngineFriends.h"

namespace GunslingerMod
{
//данные консольных команд
//булевские флаги
static u32 _console_bool_flags = 0;

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

bool IsLensEnabled() { return (_mask_lens_enabled & _console_bool_flags) != 0; }
} // namespace GunslingerMod
