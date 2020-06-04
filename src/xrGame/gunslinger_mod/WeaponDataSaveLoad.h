#pragma once

class CWeapon;
class IReader;

namespace GunslingerMod
{
void LoadAdditionalDataFromReader(CWeapon* wpn, IReader* packet);
} // namespace GunslingerMod
