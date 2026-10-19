#include "pch.h"

FQuat RotatorToQuat(const FRotator& Rotation)
{
	constexpr double Pi = 3.14159265358979323846;
	constexpr double HalfRadians = Pi / 360.0;

	const double SP = std::sin(Rotation.Pitch * HalfRadians);
	const double CP = std::cos(Rotation.Pitch * HalfRadians);
	const double SY = std::sin(Rotation.Yaw * HalfRadians);
	const double CY = std::cos(Rotation.Yaw * HalfRadians);
	const double SR = std::sin(Rotation.Roll * HalfRadians);
	const double CR = std::cos(Rotation.Roll * HalfRadians);

	FQuat Result{};
	Result.X = static_cast<float>(CR * SP * SY - SR * CP * CY);
	Result.Y = static_cast<float>(-CR * SP * CY - SR * CP * SY);
	Result.Z = static_cast<float>(CR * CP * SY - SR * SP * CY);
	Result.W = static_cast<float>(CR * CP * CY + SR * SP * SY);
	return Result;
}

int GetNetModeActorHook()
{
	return Config::NetModeDedicatedServer;
}
