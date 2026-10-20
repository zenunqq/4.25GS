#pragma once

using StaticFindObjectFn = UObject* (*)(UClass*, UObject*, const wchar_t*, bool);
using StaticLoadObjectFn = UObject* (*)(UClass*, UObject*, const wchar_t*, const wchar_t*, uint32_t, UObject*, bool);

inline StaticFindObjectFn StaticFindObjectNative = Resolve<StaticFindObjectFn>(Offsets::StaticFindObject);
inline StaticLoadObjectFn StaticLoadObjectNative = Resolve<StaticLoadObjectFn>(Offsets::StaticLoadObject);

template <typename T>
inline T* FindObject(const std::string& Path, UClass* Class = UObject::StaticClass())
{
	const std::wstring Wide(Path.begin(), Path.end());
	return reinterpret_cast<T*>(StaticFindObjectNative(Class, nullptr, Wide.c_str(), false));
}

template <typename T>
inline T* LoadObject(const std::string& Path, UClass* Class = T::StaticClass(), UObject* Outer = nullptr)
{
	const std::wstring Wide(Path.begin(), Path.end());
	return reinterpret_cast<T*>(StaticLoadObjectNative(Class, Outer, Wide.c_str(), nullptr, 0, nullptr, false));
}
