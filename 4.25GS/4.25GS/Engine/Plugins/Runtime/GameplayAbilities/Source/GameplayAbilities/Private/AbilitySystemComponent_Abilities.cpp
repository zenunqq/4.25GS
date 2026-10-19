#include "pch.h"

void GrantAbility(UAbilitySystemComponent* AbilitySystem, UGameplayAbility* Ability)
{
	if (!AbilitySystem || !Ability)
		return;

	FGameplayAbilitySpecHandle Handle;
	Handle.GenerateNewHandle();

	FGameplayAbilitySpec Spec{ -1, -1, -1 };
	Spec.Handle = Handle;
	Spec.Ability = Ability;
	Spec.Level = 1;
	Spec.InputID = 1;

	GiveAbilityNative(AbilitySystem, &Handle, Spec);
}

void GrantDefaultAbilities(UAbilitySystemComponent* AbilitySystem)
{
	static auto* AbilitySet = FindObject<UFortAbilitySet>(Config::DefaultAbilitySet);
	static auto* RangedDamage = FindObject<UClass>(Config::RangedDamageAbility);

	if (AbilitySet)
	{
		for (int Index = 0; Index < AbilitySet->GameplayAbilities.Num(); Index++)
		{
			UClass* AbilityClass = AbilitySet->GameplayAbilities[Index].Get();

			if (AbilityClass)
				GrantAbility(AbilitySystem, reinterpret_cast<UGameplayAbility*>(AbilityClass->DefaultObject));
		}
	}

	if (RangedDamage)
		GrantAbility(AbilitySystem, reinterpret_cast<UGameplayAbility*>(RangedDamage->DefaultObject));
}

FGameplayAbilitySpec* FindAbilitySpec(UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle& Handle)
{
	auto& Specs = AbilitySystem->ActivatableAbilities.Items;

	for (int Index = 0; Index < Specs.Num(); Index++)
	{
		if (Specs[Index].Handle.Handle == Handle.Handle)
			return &Specs[Index];
	}

	return nullptr;
}

void InternalServerTryActivateAbilityHook(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey* PredictionKey, const FGameplayEventData* TriggerEventData)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpec(AbilitySystem, Handle);

	if (!Spec || !Spec->Ability)
	{
		AbilitySystem->ClientActivateAbilityFailed(Handle, PredictionKey->Current);
		return;
	}

	Spec->InputPressed = true;

	UGameplayAbility* Instanced = nullptr;

	if (!InternalTryActivateAbilityNative(AbilitySystem, Handle, PredictionKey, &Instanced, nullptr, TriggerEventData))
	{
		AbilitySystem->ClientActivateAbilityFailed(Handle, PredictionKey->Current);
		Spec->InputPressed = false;
	}
}
