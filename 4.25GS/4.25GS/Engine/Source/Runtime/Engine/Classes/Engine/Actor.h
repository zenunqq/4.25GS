#pragma once

FQuat RotatorToQuat(const FRotator& Rotation);

template <typename T>
inline T* GetDefaultObject()
{
	return reinterpret_cast<T*>(T::StaticClass()->DefaultObject);
}

template <typename T>
inline T* SpawnActor(FVector Location, FRotator Rotation = FRotator{ 0, 0, 0 }, UClass* Class = T::StaticClass(), FVector Scale = FVector{ 1, 1, 1 })
{
	FTransform Transform{};
	Transform.Rotation = RotatorToQuat(Rotation);
	Transform.Scale3D = Scale;
	Transform.Translation = Location;

	auto* Statics = GetDefaultObject<UGameplayStatics>();
	auto* Actor = Statics->BeginSpawningActorFromClass(UWorld::GetWorld(), Class, Transform, false, nullptr);

	if (Actor)
		Statics->FinishSpawningActor(Actor, Transform);

	return reinterpret_cast<T*>(Actor);
}

int GetNetModeActorHook();
