#include "pch.h"

UFortWorldItem* GiveItem(AFortPlayerController* Controller, UFortItemDefinition* Definition, int Count, int LoadedAmmo)
{
	if (!Controller || !Controller->WorldInventory || !Definition)
		return nullptr;

	auto* Item = reinterpret_cast<UFortWorldItem*>(Definition->CreateTemporaryItemInstanceBP(Count, 1));

	if (!Item)
		return nullptr;

	Item->ItemEntry.Count = Count;
	Item->ItemEntry.LoadedAmmo = LoadedAmmo;
	Item->SetOwningControllerForTemporaryItem(Controller);

	auto& Inventory = Controller->WorldInventory->Inventory;
	Inventory.ItemInstances.Add(Item);

	auto& Entry = Inventory.ReplicatedEntries.Add(Item->ItemEntry);
	Entry.LoadedAmmo = LoadedAmmo;

	return Item;
}

UFortItemDefinition* FindItemDefinition(AFortPlayerController* Controller, const FGuid& ItemGuid)
{
	if (!Controller || !Controller->WorldInventory)
		return nullptr;

	auto& Instances = Controller->WorldInventory->Inventory.ItemInstances;

	for (int Index = 0; Index < Instances.Num(); Index++)
	{
		if (Instances[Index]->ItemEntry.ItemGuid == ItemGuid)
			return Instances[Index]->ItemEntry.ItemDefinition;
	}

	return nullptr;
}

void RefreshInventory(AFortPlayerController* Controller)
{
	auto* Inventory = Controller->WorldInventory;

	Controller->HandleWorldInventoryLocalUpdate();

	Inventory->HandleInventoryLocalUpdate();
	Inventory->bRequiresLocalUpdate = true;
	Inventory->ForceNetUpdate();

	MarkArrayDirtyNative(&Inventory->Inventory);
}

void GiveStartingItems(AFortPlayerController* Controller, AFortGameModeAthena* GameMode)
{
	for (int Index = 0; Index < GameMode->StartingItems.Num(); Index++)
	{
		auto& Starting = GameMode->StartingItems[Index];

		if (Starting.Item)
			GiveItem(Controller, Starting.Item, Starting.Count);
	}
}

void GiveDefaultPickaxe(AFortPlayerController* Controller)
{
	static auto* Pickaxe = FindObject<UAthenaPickaxeItemDefinition>(Config::DefaultPickaxe);

	if (Pickaxe)
		GiveItem(Controller, Pickaxe->WeaponDefinition, 1);
}
