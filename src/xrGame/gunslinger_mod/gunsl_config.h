#pragma once

namespace GunslingerMod
{
bool game_ini_line_exist(pcstr section, pcstr key);
bool game_ini_r_bool(pcstr section, pcstr key);
bool game_ini_r_bool_def(pcstr section, pcstr key, bool def);
pcstr game_ini_read_string(pcstr section, pcstr key);
} // namespace GunslingerMod
