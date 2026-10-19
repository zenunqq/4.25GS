#pragma once

inline UWorld* CurrentWorld()
{
	return UWorld::GetWorld();
}

inline AFortGameModeAthena* CurrentGameMode()
{
	return reinterpret_cast<AFortGameModeAthena*>(CurrentWorld()->AuthorityGameMode);
}

inline AFortGameStateAthena* CurrentGameState()
{
	return reinterpret_cast<AFortGameStateAthena*>(CurrentWorld()->GameState);
}

int GetNetModeWorldHook();
