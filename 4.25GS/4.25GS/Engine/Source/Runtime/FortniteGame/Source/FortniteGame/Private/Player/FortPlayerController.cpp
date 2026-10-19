#include "pch.h"

void ServerExecuteInventoryItemHook(AFortPlayerController* Controller, FGuid ItemGuid)
{
	auto* Pawn = reinterpret_cast<AFortPlayerPawnAthena*>(Controller->Pawn);

	if (!Pawn)
		return;

	auto* Definition = FindItemDefinition(Controller, ItemGuid);

	if (!Definition)
		return;

	Pawn->EquipWeaponDefinition(reinterpret_cast<UFortWeaponItemDefinition*>(Definition), ItemGuid);
}
