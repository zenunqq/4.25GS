#pragma once

inline uintptr_t ImageBase()
{
	return reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
}

template <typename Fn>
inline Fn Resolve(uintptr_t Offset)
{
	return reinterpret_cast<Fn>(ImageBase() + Offset);
}

inline void Log(const char* Message)
{
	std::cout << Message << std::endl;
}
