#pragma once

#include <cstdint>
#include <string>

namespace GunslingerMod
{
using TimeTicks = std::uint32_t;
using string = std::string;

string inttostr(s32 n);
string inttostr(u32 n);
string inttohex(u32 n, u32 cnt);
} // namespace GunslingerMod
