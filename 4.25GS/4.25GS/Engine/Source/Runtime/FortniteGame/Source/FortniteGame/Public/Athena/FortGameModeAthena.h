#pragma once

inline bool (*ReadyToStartMatchOriginal)(AFortGameModeAthena*) = nullptr;

bool HasWarmupSpots();
void ApplyPlaylist();
bool ReadyToStartMatchHook(AFortGameModeAthena* GameMode);
