#pragma once

using InitHostFn = bool (*)(AOnlineBeacon*);
using PauseBeaconRequestsFn = void (*)(AOnlineBeacon*, bool);
using InitListenFn = bool (*)(UNetDriver*, void*, FURL&, bool, FString&);
using SetWorldFn = void* (*)(UNetDriver*, UWorld*);
using TickFlushFn = void (*)(UNetDriver*);

inline InitHostFn InitHostNative = Resolve<InitHostFn>(Offsets::InitHost);
inline PauseBeaconRequestsFn PauseBeaconRequestsNative = Resolve<PauseBeaconRequestsFn>(Offsets::PauseBeaconRequests);
inline InitListenFn InitListenNative = Resolve<InitListenFn>(Offsets::InitListen);
inline SetWorldFn SetWorldNative = Resolve<SetWorldFn>(Offsets::SetWorld);
inline TickFlushFn TickFlushOriginal = nullptr;

bool StartListening(UWorld* World);
void TickFlushHook(UNetDriver* NetDriver);
