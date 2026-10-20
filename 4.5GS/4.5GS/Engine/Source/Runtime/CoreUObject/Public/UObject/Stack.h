#pragma once

struct FFrame
{
	char Padding[0x10];
	UFunction* Node;
	UObject* Object;
	uint8* Code;
	uint8* Locals;
};
