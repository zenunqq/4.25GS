#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

#include "Engine/Source/ThirdParty/MinHook/Include/MinHook.h"
#include "Engine/Source/ThirdParty/SDK/SDK.hpp"

using namespace SDK;
using namespace Params;

#include "Config.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"
#include "Engine/Plugins/HookingLibrary/HookingLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Stack.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameEngine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h"

#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Items/FortInventory.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Player/FortPlayerController.h"
#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Athena/FortPlayerControllerAthena.h"
#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Athena/FortGameModeAthena.h"
