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

inline bool ObjectArrayLooksValid()
{
	auto* Array = UObject::GObjects;

	if (!IsReadable(Array, sizeof(*Array)))
		return false;

	if (Array->NumElements <= 0 || Array->NumElements > Array->MaxElements)
		return false;

	if (!IsReadable(Array->Objects, sizeof(FUObjectItem)))
		return false;

	for (int Index = 0; Index < 16 && Index < Array->NumElements; Index++)
	{
		UObject* Object = Array->Objects[Index].Object;

		if (Object && !IsReadable(Object, sizeof(UObject)))
			return false;
	}

	return true;
}

inline void LogObjectArray()
{
	auto* Array = UObject::GObjects;

	char Line[200];
	sprintf_s(Line, "GObjects at 0x%llX", static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(Array)));
	Log(Line);

	if (!IsReadable(Array, sizeof(*Array)))
	{
		Log("GObjects memory is not readable");
		return;
	}

	sprintf_s(Line, "GObjects items 0x%llX, max %d, num %d", static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(Array->Objects)), Array->MaxElements, Array->NumElements);
	Log(Line);
}
