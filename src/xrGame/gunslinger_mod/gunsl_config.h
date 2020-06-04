#pragma once

namespace GunslingerMod
{
//------------------------------общие функции работы с игровыми конфигами---------------------------------
bool game_ini_line_exist(pcstr section, pcstr key);
bool game_ini_r_bool(pcstr section, pcstr key);
bool game_ini_r_bool_def(pcstr section, pcstr key, bool def);
float game_ini_r_single(pcstr section, pcstr key);
float game_ini_r_single_def(pcstr section, pcstr key, float def);
pcstr game_ini_read_string(pcstr section, pcstr key);

//---------------------------специфические функции конфигурации ганса--------------------------------------
bool IsLensEnabled();
} // namespace GunslingerMod
