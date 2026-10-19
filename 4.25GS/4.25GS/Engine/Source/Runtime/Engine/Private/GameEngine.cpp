#include "pch.h"

void DisableEngineFunctions()
{
	for (uintptr_t Offset : Offsets::Disabled)
		PatchReturn(Offset);
}

void LoadStartupMap()
{
	auto* World = CurrentWorld();

	GetDefaultObject<UKismetSystemLibrary>()->ExecuteConsoleCommand(World, Config::MapCommand, nullptr);

	World = CurrentWorld();
	World->OwningGameInstance->LocalPlayers.Remove(0);
}
