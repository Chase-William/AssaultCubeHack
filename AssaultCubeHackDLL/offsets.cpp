#include "pch.h"
#include "offsets.h"

void Offsets::Init()
{
	uintptr_t base = (uintptr_t)GetModuleHandleA("ac_client.exe");
	Offsets::Player::health = base + 0x17E0A8 + 0xEC;
}
