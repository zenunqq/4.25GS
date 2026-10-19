#include "pch.h"

bool StartListening(UWorld* World)
{
	auto* Beacon = SpawnActor<AFortOnlineBeaconHost>(FVector{});

	if (!Beacon)
		return false;

	Beacon->ListenPort = Config::Port - 1;

	InitHostNative(Beacon);
	PauseBeaconRequestsNative(Beacon, false);

	World->NetDriver = Beacon->NetDriver;

	auto* Driver = World->NetDriver;

	if (!Driver)
		return false;

	Driver->World = World;
	Driver->NetDriverName = GetDefaultObject<UKismetStringLibrary>()->Conv_StringToName(Config::NetDriverName);

	CurrentGameMode()->GameSession->MaxPlayers = Config::MaxPlayers;

	FString Error;
	FURL Url;
	Url.Port = Config::Port;

	InitListenNative(Driver, World, Url, true, Error);
	SetWorldNative(Driver, World);

	World->LevelCollections[0].NetDriver = Driver;
	World->LevelCollections[1].NetDriver = Driver;

	return true;
}

void TickFlushHook(UNetDriver* NetDriver)
{
	if (auto* Replication = NetDriver->ReplicationDriver)
		reinterpret_cast<void (*)(UObject*)>(Replication->Vft[VTable::ReplicationDriverReplicate])(Replication);

	if (GetAsyncKeyState(VK_F6) & 1)
		GetDefaultObject<UKismetSystemLibrary>()->ExecuteConsoleCommand(CurrentWorld(), Config::AircraftCommand, nullptr);

	TickFlushOriginal(NetDriver);
}
