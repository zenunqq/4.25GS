#pragma once

using GiveAbilityFn = FGameplayAbilitySpecHandle* (*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec);
using InternalTryActivateAbilityFn = bool (*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, const FPredictionKey*, UGameplayAbility**, void*, const FGameplayEventData*);

inline GiveAbilityFn GiveAbilityNative = Resolve<GiveAbilityFn>(Offsets::GiveAbility);
inline InternalTryActivateAbilityFn InternalTryActivateAbilityNative = Resolve<InternalTryActivateAbilityFn>(Offsets::InternalTryActivateAbility);

void GrantAbility(UAbilitySystemComponent* AbilitySystem, UGameplayAbility* Ability);
void GrantDefaultAbilities(UAbilitySystemComponent* AbilitySystem);
FGameplayAbilitySpec* FindAbilitySpec(UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle& Handle);
void InternalServerTryActivateAbilityHook(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey* PredictionKey, const FGameplayEventData* TriggerEventData);
