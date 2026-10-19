#include "pch.h"

bool HasWarmupSpots()
{
	TArray<AActor*> Spots;
	GetDefaultObject<UGameplayStatics>()->GetAllActorsOfClass(CurrentWorld(), AFortPlayerStartWarmup::StaticClass(), &Spots);

	const int Count = Spots.Num();
	Spots.Free();

	return Count > 0;
}

void ApplyPlaylist()
{
	auto* Playlist = FindObject<UFortPlaylistAthena>(Config::Playlist);

	if (!Playlist)
		return;

	auto* State = CurrentGameState();
	State->CurrentPlaylistData = Playlist;
	State->OnRep_CurrentPlaylistData();
}

bool ReadyToStartMatchHook(AFortGameModeAthena* GameMode)
{
	static bool bPrepared = false;

	if (!HasWarmupSpots())
		return false;

	if (!bPrepared)
	{
		bPrepared = true;
		Log("Warmup spots found");

		ApplyPlaylist();
		Log("Playlist applied");

		if (StartListening(CurrentWorld()))
			Log("Listening");
		else
			Log("Failed to start listening");

		Log("Listen setup finished");

		GameMode->WarmupRequiredPlayerCount = Config::WarmupRequiredPlayers;

		auto* State = CurrentGameState();
		State->PlayersLeft--;
		State->OnRep_PlayersLeft();

		GameMode->bWorldIsReady = true;
	}

	return ReadyToStartMatchOriginal(GameMode);
}
