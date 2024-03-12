#include <tchar.h>
#include <iostream>
#include <Windows.h>
#include "kmbox.hpp"  
#include "global.h"
#include "mem.hpp"
#include <thread>

#include<array>
#include "AimAssist.hpp"
_com kmboxx;
int PortNumber = 3;

std::array<std::uint8_t, 256 * 2 / 8> key_state_bitmap;
std::array<std::uint8_t, 256 / 8> key_state_recent_bitmap;
bool is_key_down(std::uint8_t const vk) {
	return key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2;
}
void keydown_Loop() {
	int pid = 0;
	int win = 0;
	SIZE_T pcPIDs;
	VMMDLL_PidList(dma.hVMM, nullptr, &pcPIDs);
	DWORD* pPIDs = (DWORD*)new char[pcPIDs * 4];
	VMMDLL_PidList(dma.hVMM, pPIDs, &pcPIDs);
	PVMMDLL_MAP_EAT KeyState;
	VMMDLL_MemFree(&KeyState);
	PVMMDLL_MAP_MODULEENTRY mod;
	VMMDLL_MemFree(&mod);
	for (int i = 0; i < pcPIDs; i++) 
	{
		VMMDLL_PROCESS_INFORMATION ProcessInformation = { 0 };
		ProcessInformation.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
		ProcessInformation.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;
		SIZE_T pcbProcessInformation = sizeof(VMMDLL_PROCESS_INFORMATION);
		VMMDLL_ProcessGetInformation(dma.hVMM, pPIDs[i], &ProcessInformation, &pcbProcessInformation);
		if (strcmp(ProcessInformation.szName, "csrss.exe") == 0) {
			if (VMMDLL_Map_GetModuleFromNameU(dma.hVMM, pPIDs[i], (LPSTR)"win32ksgd.sys", &mod, VMMDLL_FLAG_NOCACHE)) {
				pid = pPIDs[i];
			}
		}
	}
	if (pid == 0) {
		win = 10;
		for (int i = 0; i < pcPIDs; i++)
		{
			VMMDLL_PROCESS_INFORMATION ProcessInformation = { 0 };
			ProcessInformation.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
			ProcessInformation.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;
			SIZE_T pcbProcessInformation = sizeof(VMMDLL_PROCESS_INFORMATION);
			VMMDLL_ProcessGetInformation(dma.hVMM, pPIDs[i], &ProcessInformation, &pcbProcessInformation);
			if (strcmp(ProcessInformation.szName, "csrss.exe") == 0) {
				if (VMMDLL_Map_GetEATU(dma.hVMM, pPIDs[i], (LPSTR)"win32kbase.sys", &KeyState)) {
					pid = pPIDs[i];
				}
			}
		}

	}
	else {
		win = 11;
	}
	DWORD64 base = 0;
	if (win == 10) {
		int kk = 1000;
		while (base == 0) {
			if (strcmp(KeyState->pMap[kk].uszFunction, "gafAsyncKeyState") == 0) {
				base = KeyState->pMap[kk].vaFunction;
				break;
			}
			kk++;
		}
	}
	if (win == 11) {
		DWORD64 gSessionGlobalSlots, ketaddress, inketaddress;
		VMMDLL_MemReadEx(dma.hVMM, pid, mod->vaBase + 0x3110, (PBYTE)&gSessionGlobalSlots, sizeof(gSessionGlobalSlots), 0, VMMDLL_FLAG_NOCACHE);
		VMMDLL_MemReadEx(dma.hVMM, pid, gSessionGlobalSlots, (PBYTE)&ketaddress, sizeof(ketaddress), 0, VMMDLL_FLAG_NOCACHE);
		VMMDLL_MemReadEx(dma.hVMM, pid, ketaddress, (PBYTE)&inketaddress, sizeof(inketaddress), 0, VMMDLL_FLAG_NOCACHE);
		base = inketaddress + 0x3690;
	}


	while (1) {
		auto prev_key_state_bitmap = key_state_bitmap;
		VMMDLL_MemReadEx(dma.hVMM, pid, base, (PBYTE)&key_state_bitmap, sizeof(key_state_bitmap), 0, VMMDLL_FLAG_NOCACHE);
		for (auto vk = 0u; vk < 256; ++vk) {
			if ((key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) &&
				!(prev_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
				key_state_recent_bitmap[vk / 8] |= 1 << vk % 8;
		}

		if (is_key_down(0x1)) {
			leftkey = 1;
		}
		else {
			leftkey = 0;

		}
		if (is_key_down(0x2)) {
			rightkey = 1;
		}
		else {
			rightkey = 0;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void MoveMouseTo(int targetX, int targetY, float speed) {


	int x1 = int(targetX - (screenWidth / 2));
	int y1 = int(targetY - (screenHeight / 2));
	aim_sensitive = 1.f + (speed / 100.f);
	x1 = x1 - int(float(x1) / aim_sensitive);
	y1 = y1 - int(float(y1) / aim_sensitive);
	char buff[1024];
	
	sprintf_s(buff, "km.move(%d,%d)\r\n", x1, y1);

	kmboxx.write(buff);

	Sleep(10);
 
	kmboxx.read(buff, sizeof(buff));


}

void Aimbot() {

	while (1) {

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		int targetii = -1;
		float DistanceToSight = 0;
		float MaxAimDistance = 100000;
		for (auto kk = 0; kk < allplayer.sum; kk++)
		{
			entitylist temp2 = allplayer;
			int i = temp2.i[kk];
			if ((static_cast<int>(distancess[i])) > AimAssist::i().disss->x) continue;
			DistanceToSight = sqrt(pow(head[i][1].x - ((float)screenWidth / 2), 2) + pow(head[i][1].y - ((float)screenHeight / 2), 2));
			if (drawed[i] == false)continue;
			if (DistanceToSight >= 0 && DistanceToSight < AimAssist::i().fov->x)
			{
				if (DistanceToSight < MaxAimDistance)
				{
					MaxAimDistance = DistanceToSight;
					if (head[i][1].x + head[i][1].y > screenWidth + screenHeight)
					{
						
					}
					else
					{
						targetii = i;
					}
				}

			}

		}

		if (rightkey == 1 && targetii != -1) {

			MoveMouseTo(head[targetii][1].x, head[targetii][1].y, AimAssist::i().reactTime->x);

		}
		



	}

}