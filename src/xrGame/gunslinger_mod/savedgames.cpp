#include "StdAfx.h"
#include "savedgames.h"
#include "BaseDefs.h"
#include "BaseGameData.h"
#include "Misc.h"
#include "gunsl_config.h"

namespace GunslingerMod
{
const u32 GUNS_SAVE_VER = 1;
const u32 GUNS_MASK = 0x20200101;

const u16 GUNS_KEY_ID_MOD_NAME = 0x0001;
const u16 GUNS_KEY_ID_MOD_VER = 0x0002;
const u16 GUNS_KEY_ID_AUTH_LOW = 0x0003;
const u16 GUNS_KEY_ID_AUTH_HIGH = 0x0004;
const u16 GUNS_KEY_ID_SAVE_VER = 0x0005;

const u32 GUNS_KEY_TYPE_U32 = 0x00010000;
const u32 GUNS_KEY_TYPE_STRINGZ = 0x00020000;
const u32 GUNS_KEY_TYPE_TERMINATOR = 0x00030000;

u32 GetTransformedGunsSaveVer() { return (0xFFFFFFFF - GUNS_SAVE_VER) ^ GUNS_MASK; }

void GetAuthVal(u32* phigh, u32* plow)
{
    u64 auth = EngineFriendWrapper::AuthGet();
    *plow = auth;
    *phigh = auth >> 32;
}

void WriteGunsStringZData(IWriter* w, u16 key_id, pcstr data)
{
    IWriter__w_u32(w, GUNS_KEY_TYPE_STRINGZ ^ key_id);
    IWriter__w_stringZ(w, data);
}

void WriteGunsU32Data(IWriter* w, u16 key_id, u32 data)
{
    IWriter__w_u32(w, GUNS_KEY_TYPE_U32 ^ key_id);
    IWriter__w_u32(w, data);
}

void WriteGunsDataTerminator(IWriter* w) { IWriter__w_u32(w, GUNS_KEY_TYPE_TERMINATOR); }

bool ReadNextGunsU32Data(IReader* reader, u16& id, u32& value)
{
    if (ReaderElapsed(reader) < 8)
    {
        return false;
    }

    u32 tmp = 0;
    ReadFromReader(reader, &tmp, sizeof(tmp));
    u32 keytype = tmp & 0xFFFF0000;
    if (keytype != GUNS_KEY_TYPE_U32)
    {
        return false;
    }

    id = tmp & 0xFFFF;

    ReadFromReader(reader, &value, sizeof(value));
    return true;
}

bool ReadNextGunsStringZData(IReader* reader, u16& id, string& value)
{
    if (ReaderElapsed(reader) < 5)
        return false;

    u32 tmp = 0;
    ReadFromReader(reader, &tmp, sizeof(tmp));
    u32 keytype = tmp & 0xFFFF0000;
    if (keytype != GUNS_KEY_TYPE_STRINGZ)
        return false;

    id = tmp & 0xFFFF;

    string str = "";
    char c = 0;
    bool flag = true;
    while (flag)
    {
        if (c != 0)
        {
            str = str + c;
        }
        ReadFromReader(reader, &c, sizeof(c));
        flag = (c != 0) && (ReaderElapsed(reader) > 0);
    }

    if (c == 0)
    {
        value = str;
        return true;
    }

    return false;
}

bool ReadGunsTerminatorData(IReader* reader)
{
    if (ReaderElapsed(reader) < 8)
        return false;

    u32 tmp = 0;
    ReadFromReader(reader, &tmp, sizeof(tmp));
    u32 keytype = tmp & 0xFFFF0000;
    if (keytype != GUNS_KEY_TYPE_TERMINATOR)
        return false;

    return true;
}

ESavedGameParseStatus CSavedGameWrapper__valid_saved_game_override_internal(
    IReader* reader, string& version, string& addon_name, string& save_ver, u32& d1, u32& d2)
{
    const u32 CURRENT_ALIFE_VERSION = EngineFriendWrapper::GetCurrentALifeVersion();

    u32 ver_hdr = 0;
    u32 alife_ver = 0;
    u16 data_id = 0;

    version = "";
    save_ver = "";
    addon_name = "";
    d1 = 0;
    d2 = 0;

    ESavedGameParseStatus result = GUNS_UNCOMPATIBLE_SAVE;

    if (ReaderLength(reader) < 8)
        return result;

    ReadFromReader(reader, &ver_hdr, sizeof(ver_hdr));
    if (ver_hdr == 0xFFFFFFFF)
    {
        ReadFromReader(reader, &alife_ver, sizeof(alife_ver));
        if (alife_ver >= CURRENT_ALIFE_VERSION)
        {
            result = GUNS_ORIGINAL_GAME_SAVE;
        }
    }
    else if (ver_hdr == GetTransformedGunsSaveVer())
    {
        if (!ReadNextGunsU32Data(reader, data_id, d1))
            return result;
        if (!ReadNextGunsU32Data(reader, data_id, d2))
            return result;

        if (!ReadNextGunsStringZData(reader, data_id, addon_name) || (data_id != GUNS_KEY_ID_MOD_NAME))
            return result;
        if (!ReadNextGunsStringZData(reader, data_id, save_ver) || (data_id != GUNS_KEY_ID_SAVE_VER))
            return result;
        if (!ReadNextGunsStringZData(reader, data_id, version) || (data_id != GUNS_KEY_ID_MOD_VER))
            return result;
        if (!ReadGunsTerminatorData(reader))
            return result;

        if ((addon_name == GetAddonName()) && (save_ver == GetSaveVer()))
        {
            result = GUNS_FULLY_COMPATIBLE_SAVE;
        }
        else
        {
            result = GUNS_ANOTHER_MOD_SAVE;
        };

        // parse original ALIFE_VERSION
        if (ReaderElapsed(reader) < 4)
        {
            result = GUNS_UNCOMPATIBLE_SAVE;
            return result;
        }
        ReadFromReader(reader, &alife_ver, sizeof(alife_ver));
        if (alife_ver < CURRENT_ALIFE_VERSION)
        {
            result = GUNS_UNCOMPATIBLE_SAVE;
        }
    }
    return result;
}

ESavedGameParseStatus CSavedGameWrapper__valid_saved_game_override(IReader* reader)
{
    string version, addon_name, save_ver;
    u32 d1, d2;

    ESavedGameParseStatus result =
        CSavedGameWrapper__valid_saved_game_override_internal(reader, version, addon_name, save_ver, d1, d2);

    LogText(string("Saved game status: ") + inttostr(static_cast<u32>(result)));
    if ((result == GUNS_FULLY_COMPATIBLE_SAVE) || (result == GUNS_ANOTHER_MOD_SAVE))
    {
        LogText(string("Info: ") + addon_name + "(" + GetAddonName() + ")" + ", " + version + "(" + GetModVer() + ")" +
            ", " + save_ver + "(" + GetSaveVer() + ")" + ", " + inttohex(d2, 8) + inttohex(d1, 8));
    }
    return result;
}

void ConstructGunsHeader(IWriter* packet)
{
    u32 l, h;
    GetAuthVal(&h, &l);

    //Сначала - наш хидер вместо вырезанного (-1)
    IWriter__w_u32(packet, GetTransformedGunsSaveVer());

    //Теперь - данные мода
    WriteGunsU32Data(packet, GUNS_KEY_ID_AUTH_LOW, l);
    WriteGunsU32Data(packet, GUNS_KEY_ID_AUTH_HIGH, h);
    WriteGunsStringZData(packet, GUNS_KEY_ID_MOD_NAME, GetAddonName());
    WriteGunsStringZData(packet, GUNS_KEY_ID_SAVE_VER, GetSaveVer());
    WriteGunsStringZData(packet, GUNS_KEY_ID_MOD_VER, GetModVer());

    //Завершение данных ганса
    WriteGunsDataTerminator(packet);
}
} // namespace GunslingerMod
