#include "pch.h"

void ApplyDefaultCharacterParts(AFortPlayerStateAthena* PlayerState)
{
	static auto* Head = FindObject<UCustomCharacterPart>(Config::DefaultHead);
	static auto* Body = FindObject<UCustomCharacterPart>(Config::DefaultBody);

	if (!PlayerState || !Head || !Body)
		return;

	PlayerState->CharacterParts[0] = Head;
	PlayerState->CharacterParts[1] = Body;
	PlayerState->OnRep_CharacterParts();
}
