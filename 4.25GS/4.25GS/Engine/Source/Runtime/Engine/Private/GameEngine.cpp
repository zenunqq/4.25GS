#include "pch.h"

void DisableEngineFunctions()
{
	for (uintptr_t Offset : Offsets::Disabled)
		PatchReturn(Offset);
}

void LoadStartupMap()
{
	auto* World = CurrentWorld();
	Log(World ? "World found" : "World missing");

	GetDefaultObject<UKismetSystemLibrary>()->ExecuteConsoleCommand(World, Config::MapCommand, nullptr);

	Log("Map command sent");

	World = CurrentWorld();
	World->OwningGameInstance->LocalPlayers.Remove(0);
}
