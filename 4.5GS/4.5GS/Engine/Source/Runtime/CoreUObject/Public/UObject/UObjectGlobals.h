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

inline bool IsFlatObjectArray(uintptr_t Address)
{
	auto* Candidate = reinterpret_cast<TUObjectArray*>(Address);

	if (Candidate->NumElements < 1000 || Candidate->NumElements > 4000000)
		return false;

	if (Candidate->MaxElements < Candidate->NumElements || Candidate->MaxElements > 8000000)
		return false;

	if (!IsReadable(Candidate->Objects, sizeof(FUObjectItem) * 64))
		return false;

	int ValidObjects = 0;

	for (int Index = 0; Index < 64; Index++)
	{
		UObject* Object = Candidate->Objects[Index].Object;

		if (!Object)
			continue;

		if (!IsReadable(Object, sizeof(UObject)) || Object->Index != Index)
			return false;

		ValidObjects++;
	}

	return ValidObjects >= 32;
}

inline TUObjectArray* ScanForObjectArray()
{
	const uintptr_t Base = ImageBase();
	const auto* Dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(Base);
	const auto* Nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(Base + Dos->e_lfanew);
	const IMAGE_SECTION_HEADER* Section = IMAGE_FIRST_SECTION(Nt);

	for (WORD Index = 0; Index < Nt->FileHeader.NumberOfSections; Index++, Section++)
	{
		if (!(Section->Characteristics & IMAGE_SCN_MEM_WRITE))
			continue;

		const uintptr_t Start = Base + Section->VirtualAddress;
		const uintptr_t End = Start + Section->Misc.VirtualSize;

		for (uintptr_t Address = Start; Address + sizeof(TUObjectArray) <= End; Address += sizeof(void*))
		{
			if (IsFlatObjectArray(Address))
				return reinterpret_cast<TUObjectArray*>(Address);
		}
	}

	return nullptr;
}
