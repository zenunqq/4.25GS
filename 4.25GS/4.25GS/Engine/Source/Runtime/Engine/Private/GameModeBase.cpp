#include "pch.h"

APawn* SpawnDefaultPawnForHook(AGameModeBase* GameMode, AController* NewPlayer, AActor* StartSpot)
{
	static auto* PawnClass = FindObject<UClass>(Config::PawnClass);

	if (!PawnClass || !StartSpot)
		return nullptr;

	auto* Pawn = SpawnActor<APawn>(StartSpot->K2_GetActorLocation(), StartSpot->K2_GetActorRotation(), PawnClass);

	auto* Controller = reinterpret_cast<AFortPlayerController*>(NewPlayer);
	auto* State = reinterpret_cast<AFortPlayerStateAthena*>(NewPlayer->PlayerState);

	GrantDefaultAbilities(State->AbilitySystemComponent);
	GiveStartingItems(Controller, reinterpret_cast<AFortGameModeAthena*>(GameMode));
	GiveDefaultPickaxe(Controller);
	ApplyDefaultCharacterParts(State);
	RefreshInventory(Controller);

	return Pawn;
}

void HandleStartingNewPlayerHook(AFortGameModeAthena* GameMode, AFortPlayerControllerAthena* NewPlayer)
{
	auto* State = reinterpret_cast<AFortPlayerStateAthena*>(NewPlayer->PlayerState);

	NewPlayer->bHasServerFinishedLoading = true;
	NewPlayer->OnRep_bHasServerFinishedLoading();

	State->bHasStartedPlaying = true;
	State->OnRep_bHasStartedPlaying();

	NewPlayer->OverriddenBackpackSize = Config::BackpackSize;

	HandleStartingNewPlayerOriginal(GameMode, NewPlayer);
}
