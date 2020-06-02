#include "StdAfx.h"
#include "gunsl_config.h"
#include "xrCore/xr_ini.h"

namespace GunslingerMod
{
const CInifile* GetGameIni() { return pSettings; }

bool game_ini_line_exist(pcstr section, pcstr key) { return GetGameIni()->line_exist(section, key); }

bool game_ini_r_bool(pcstr section, pcstr key) { return GetGameIni()->r_bool(section, key); }

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

pcstr game_ini_read_string(pcstr section, pcstr key) { return GetGameIni()->r_string(section, key); }
} // namespace GunslingerMod
