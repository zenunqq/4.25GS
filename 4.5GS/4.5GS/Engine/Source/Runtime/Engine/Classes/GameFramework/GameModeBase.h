#pragma once

inline void (*HandleStartingNewPlayerOriginal)(AFortGameModeAthena*, AFortPlayerControllerAthena*) = nullptr;

APawn* SpawnDefaultPawnForHook(AGameModeBase* GameMode, AController* NewPlayer, AActor* StartSpot);
void HandleStartingNewPlayerHook(AFortGameModeAthena* GameMode, AFortPlayerControllerAthena* NewPlayer);
