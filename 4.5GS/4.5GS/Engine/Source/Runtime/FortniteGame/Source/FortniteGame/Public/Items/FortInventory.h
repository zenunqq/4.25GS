#pragma once

using MarkArrayDirtyFn = void (*)(FFastArraySerializer*);

inline MarkArrayDirtyFn MarkArrayDirtyNative = Resolve<MarkArrayDirtyFn>(Offsets::MarkArrayDirty);

UFortWorldItem* GiveItem(AFortPlayerController* Controller, UFortItemDefinition* Definition, int Count, int LoadedAmmo = 0);
UFortItemDefinition* FindItemDefinition(AFortPlayerController* Controller, const FGuid& ItemGuid);
void RefreshInventory(AFortPlayerController* Controller);
void GiveStartingItems(AFortPlayerController* Controller, AFortGameModeAthena* GameMode);
void GiveDefaultPickaxe(AFortPlayerController* Controller);
