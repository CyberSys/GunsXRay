#pragma once

#include <cstdint>
#include <string>

namespace GunslingerMod
{
using TimeTicks = std::uint32_t;
using string = std::string;

string inttostr(s32 n);
string inttostr(u32 n);
string inttohex(u64 n, u32 cnt);
string ptrtohex(void* n);

float strtofloatdef(pcstr str, float def);
float strtofloatdef(string str, float def);

string leftstr(const string& str, u32 n);
string leftstr(pcstr str, u32 n);
string rightstr(const string& str, u32 n);
string rightstr(pcstr str, u32 n);
u32 length(string str);
u32 length(pcstr str);

string trim_right(const string& str);
string trim_left(const string& str);
string trim(pcstr str);
string trim(const string& str);
} // namespace GunslingerMod
