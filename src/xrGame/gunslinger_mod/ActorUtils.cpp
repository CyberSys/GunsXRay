#include "StdAfx.h"
#include "ActorUtils.h"

namespace GunslingerMod
{
CActor* GetActor()
{
    CActor* res = nullptr;
    if (g_pGameLevel != nullptr)
    {
        IGameObject* o = g_pGameLevel->CurrentEntity();
        if (o != nullptr)
            res = dynamic_cast<CActor*>(o);
    }
    return res;
}
int GetActorActiveSlot()
{
    int res = -1;
    CActor* act = GetActor();
    if (act != nullptr)
    {
        res = static_cast<int>(act->inventory().GetActiveSlot());
    }
    return res;
}

CHudItemObject* GetActorActiveItem()
{
    CHudItemObject* res = nullptr;
    CActor* act = GetActor();
    if (act != nullptr)
    {
        CInventoryItem* itm = ItemInSlot(act, GetActorActiveSlot());
        if (itm != nullptr)
        {
            res = static_cast<CHudItemObject*>(itm);
        }
    }
    return res;
}

CInventoryItem* ItemInSlot(CActor* act, int slot) { return act->inventory().ItemFromSlot(slot); }
} // namespace GunslingerMod
