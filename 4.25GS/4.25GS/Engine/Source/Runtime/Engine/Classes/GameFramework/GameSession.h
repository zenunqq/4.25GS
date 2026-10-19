#pragma once

inline void (*KickPlayerOriginal)(AGameSession*, AController*) = nullptr;

void KickPlayerHook(AGameSession* Session, AController* Controller);
