#pragma once

class CSE_Abstract;
class IGameObject;
class CGameObject;

namespace GunslingerMod
{
CScriptGameObject* game_object_GetScriptGameObject(CGameObject* obj);
CSE_Abstract* get_server_object_by_id(u16 id);
void alife_release(CSE_Abstract* srv_obj);

bool ElectronicsProblemsDec();
bool ElectronicsProblemsInc();
float TargetElectronicsProblemsCnt();
float CurrentElectronicsProblemsCnt();

void ResetElectronicsProblems();
bool ElectronicsProblemsImmediateApply();

void ReadFromReader(IReader* r, void* buf, size_t bytes_count);
u32 ReaderLength(IReader* r);
u32 ReaderElapsed(IReader* r);
void IWriter__w_u32(IWriter* w, u32 value);
void IWriter__w_stringZ(IWriter* w, pcstr value);

u16 GetCObjectID(IGameObject* o);
} // namespace GunslingerMod
