#pragma once

class IReader;

namespace GunslingerMod
{
enum ESavedGameParseStatus
{
    GUNS_UNCOMPATIBLE_SAVE = 0,
    GUNS_FULLY_COMPATIBLE_SAVE = 1,
    GUNS_ORIGINAL_GAME_SAVE = 2,
    GUNS_ANOTHER_MOD_SAVE = 3,
};

ESavedGameParseStatus CSavedGameWrapper__valid_saved_game_override(IReader* reader);
void ConstructGunsHeader(IWriter* packet);
} // namespace GunslingerMod
