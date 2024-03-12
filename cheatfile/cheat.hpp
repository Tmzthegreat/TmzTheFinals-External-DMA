#pragma once

#include "render.hpp"
#include "mem.hpp"
#include "struct.hpp"
#include "offset.hpp"
#include <string>
#include <chrono>
#include <thread>
#include "global.h"
#include "aimbot.hpp"
#include "decrypt.hpp"
#include <emmintrin.h>
#include <softintrin.h>
#include <codecvt>
#include <intrin.h>
#include "../menu/Menu.hpp"
#include "../ESP.hpp"
#include "../Module.hpp"
ESP& espInstance = Singleton<ESP>::i();
std::chrono::steady_clock::time_point lastDrawTime = std::chrono::steady_clock::now();

Transform view;

Vec2 screenpos, headpos;
uint64_t UWorld = NULL, UWorld2 = NULL;
DOUBLE viewMatrix[4][4];




uint64_t decryptUworld6(uintptr_t ImageBase) {

	int v11; // eax
	char v12; // dl
	__m128i si128; // xmm1
	__m128i v14; // xmm2
	__m128i v15; // xmm3
	unsigned __int64 v16; // rax
	{
		__int64 xmmword_6FA9C50 = dma.readMemory<__int64>(ImageBase + 0x6FA9C50);
		__int64 xmmword_6FA9C60 = dma.readMemory<__int64>(ImageBase + 0x6FA9C60);
		__int64 xmmword_6FA9C70 = dma.readMemory<__int64>(ImageBase + 0x6FA9C70);
		__m128i* unk_955F450 = reinterpret_cast<__m128i*>(ImageBase + 0x955F450);
		__m128i* unk_955F060 = reinterpret_cast<__m128i*>(ImageBase + 0x955F060);

		v11 = __ROL4__(
			16777619
			* __ROL4__(
				16777619 * __ROL4__(16777619 * __ROL4__((unsigned int)unk_955F450, 26) + 746532130, 17)
				+ ((unsigned __int64)unk_955F450 >> 32)
				+ 746532130,
				24)
			+ 746532130,
			15);
		printf("V11 :: %llx   ", v11);
		v12 = (-109 * v11 + 34) ^ ((unsigned int)(16777619 * v11 + 746532130) >> 16);
		si128 = _mm_load_si128((const __m128i*) & xmmword_6FA9C50);
		v14 = _mm_load_si128((const __m128i*) & xmmword_6FA9C60);
		v15 = _mm_load_si128((const __m128i*) & xmmword_6FA9C70);
		__m128i temp = dma.readMemory<__m128i>((ULONG64)((char*)unk_955F060 + 48 * (v12 & 7) + 1032));
		__m128i result128i = _mm_xor_si128(
			_mm_shuffle_epi8(
				_mm_xor_si128(temp, si128),
				v14),
			v15);
		v16 = _mm_cvtsi128_si64(result128i);
		__m128i temp2 = dma.readMemory<__m128i>((ULONG64)((char*)unk_955F060 + 48 * ((v12 + 1) & 7) + 1032));
		unsigned __int64 part2 = _mm_cvtsi128_si64(_mm_xor_si128(
			_mm_shuffle_epi8(
				_mm_xor_si128(temp2, si128),
				v14),
			v15));
		unsigned __int64 result = v16 + (part2 ^ (0x100000001B3i64
			* __ROL8__(0x100000001B3i64 * __ROL8__(v16, 39) - 0x4AA4698CF2D5601Ei64, 35)
			- 0x4AA4698CF2D5601Ei64));


		return result;
	}

}

void getAllDataloop()
{
	uint64_t modbase1 = dma.getModbase();
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		UWorld2f = decryptUworld6(modbase1);
		std::cout << "uworld::  " << std::hex << UWorld2f << std::endl;
		VMMDLL_ConfigSet(dma.hVMM, VMMDLL_OPT_REFRESH_FREQ_TLB_PARTIAL, 1);
		dma.SClear(dma.hS, VMMDLL_FLAG_NOCACHE);
		dma.SPrepare(dma.hS, UWorld2f + offsets::GameState, sizeof(uint64_t));
		dma.SPrepare(dma.hS, GameStatef + offsets::PlayerArray, sizeof(uint64_t));
		dma.SPrepare(dma.hS, GameStatef + offsets::PlayerCount, sizeof(int));

		//self
		dma.SPrepare(dma.hS, UWorld2f + offsets::OwningGameInstance, sizeof(uint64_t));
		dma.SPrepare(dma.hS, owning + offsets::LocalPlayers, sizeof(uint64_t));
		dma.SPrepare(dma.hS, LocalPlayers + 0x0, sizeof(uint64_t));
		dma.SPrepare(dma.hS, self + offsets::localplayercontroller, sizeof(uint64_t));
		dma.SPrepare(dma.hS, localcontroller + offsets::ackpawn, sizeof(uint64_t));
		dma.SPrepare(dma.hS, ackpawn + offsets::USquadComponent, sizeof(uint64_t));
		dma.SPrepare(dma.hS, localteam + offsets::GetSquadIndex, sizeof(int));
		dma.ExecuteMemoryReads(dma.hS);
		GameStatef = dma.SRead<uint64_t>(dma.hS, UWorld2f + offsets::GameState);
		playerarrayf = dma.SRead<uint64_t>(dma.hS, GameStatef + offsets::PlayerArray);
		playerCount = dma.SRead<int>(dma.hS, GameStatef + offsets::PlayerCount);
		owning = dma.SRead<uint64_t>(dma.hS, UWorld2f + offsets::OwningGameInstance);
		LocalPlayers = dma.SRead<uint64_t>(dma.hS, owning + offsets::LocalPlayers);
		self = dma.SRead<uint64_t>(dma.hS, LocalPlayers + 0x0);
		localcontroller = dma.SRead<uint64_t>(dma.hS, self + offsets::localplayercontroller);
		ackpawn = dma.SRead<uint64_t>(dma.hS, localcontroller + offsets::ackpawn);
		localteam = dma.SRead<uint64_t>(dma.hS, ackpawn + offsets::USquadComponent);
		localteamindex = dma.SRead<int>(dma.hS, localteam + offsets::GetSquadIndex);
		entitylist start;
		for (int j = 0; j < playerCount; j++) {
			start.i[start.sum++] = j;
		}
		allplayer = start;
	}

}


auto ProjectWorldToScreen(FVector WorldLocation) -> FVector
{
	FVector Screenlocation = FVector(0, 0, 0);
	FVector CameraLocation, CameraRotation;
	CameraLocation = ViewInfo.Locations;
	CameraRotation = ViewInfo.Rotations;
	
	FVector vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]),
		vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]),
		vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
	FVector vDelta = WorldLocation - CameraLocation;
	FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f) vTransformed.z = 1.f;
		
	float FovAngle = ViewInfo.FOVs;
	//cout << ViewInfo.FOV << endl;
	float ScreenCenterX = screenWidth / 2.0f;
	float ScreenCenterY = screenHeight / 2.0f;
	//std::cout << ScreenCenterX << ScreenCenterY << std::endl;
	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterY / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterY / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}



void printViewInfo(const FMinimalViewInfo& ViewInfo) {
	//std::cout << "ViewInfo:" << std::endl;
	std::cout << "  Locations: "
		<< "X=" << ViewInfo.Locations.x
		<< ", Y=" << ViewInfo.Locations.y
		<< ", Z=" << ViewInfo.Locations.z << std::endl;
	std::cout << "  Rotations: "
		<< "X=" << ViewInfo.Rotations.x
		<< ", Y=" << ViewInfo.Rotations.y
		<< ", Z=" << ViewInfo.Rotations.z << std::endl;
	std::cout << "  FOVs: " << ViewInfo.FOVs << std::endl;
}

void PrintMatrix(const MAT4X4& matrix) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << std::setw(10) << matrix.c[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
void getMatrix() {
	while (TRUE) {


		dma.SClear(dma.hS3, VMMDLL_FLAG_NOCACHE);
		uint64_t modbase1 = dma.getModbase();

		dma.SPrepare(dma.hS3, localcontroller + offsets::PlayerCameraManager, sizeof(uint64_t));
		dma.SPrepare(dma.hS3, viewview + offsets::viewoffset, sizeof(FMinimalViewInfo));
		dma.ExecuteMemoryReads(dma.hS3);
		viewview = dma.SRead<uint64_t>(dma.hS3, localcontroller + offsets::PlayerCameraManager);
		ViewInfo = dma.SRead<FMinimalViewInfo>(dma.hS3, viewview + offsets::viewoffset);
	}

}

std::string get_name_from_fstring(int i)
{
	playername[i] = dma.SRead<uint64_t>(dma.hS2, actorpointer[i] + offsets::playername);
	num_chars[i] = dma.SRead<uint32_t>(dma.hS2, actorpointer[i] + offsets::playernamecount);
	std::wstring wideString;
	for (uint32_t j = 0; j < num_chars[i]; ++j)
	{
		wchar_t c = dma.SRead<wchar_t>(dma.hS2, playername[i] + j * sizeof(wchar_t));
		wideString.push_back(c);
	}
	name[i] = std::string(wideString.begin(), wideString.end());
	return name[i];
}



FVector GetEntityBone(int i, int id) {
	old_bone[i][id] = dma.SRead<FTransform>(dma.hS2, Mash[i] + (id * sizeof(FTransform)));
	if (old_bone[i][id].rot.x != 0)bone2[i][id] = old_bone[i][id];
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone2[i][id].ToMatrixWithScale(), ComponentToWorld[i].ToMatrixWithScale());
	return FVector(Matrix._41, Matrix._42, Matrix._43);
}



void draw_skeleton(int i) {
	ImColor color = ImColor(255, 255, 0, 255); 
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	std::map<int, Vec3> screenBonePositions;

	Vec3 minScreenPos(100000.0f, 100000.0f, 100000.0f);
	Vec3 maxScreenPos(-100000.0f, -100000.0f, -100000.0f);
	ImColor rainbowColor = GetRainbowColor(0.0005, rainbow, 255.0f);

	for (int id : boneIds) {
		boneTo[i] = GetEntityBone(i, id);
		FVector temp;
		temp = ProjectWorldToScreen(boneTo[i]);
		Vec3 screenPos = { temp.x, temp.y, temp.z };
		

		
		if (screenPos.x != 0) {
			minScreenPos.x = fmin(screenPos.x, minScreenPos.x);
			minScreenPos.y = fmin(screenPos.y, minScreenPos.y);
			maxScreenPos.x = fmax(screenPos.x, maxScreenPos.x);
			maxScreenPos.y = fmax(screenPos.y, maxScreenPos.y);
			if (id == 7) {
			head[i][1] = screenPos; // 
			std::cout << "head locaton :: " << head[i][1].x << std::endl;
			distancess[i] = boneTo[i].Distance(ViewInfo.Locations);
			}
			screenBonePositions[id] = screenPos;
		}
		
	}

	//The code below is for viewmartix, if you do not want to get view from camera 
	
	//for (int id : boneIds) {
	//	boneTo[i] = GetEntityBone(i, id);

	//	Vec3 WPos = { boneTo[i].x, boneTo[i].y, boneTo[i].z };
	//	Vec3 screenPos;

	//	if (view.bone2s(WPos, &screenPos)) {
	//		if (screenPos.x != 0) {
	//	
	//			minScreenPos.x = fmin(screenPos.x, minScreenPos.x);
	//			minScreenPos.y = fmin(screenPos.y, minScreenPos.y);
	//			maxScreenPos.x = fmax(screenPos.x, maxScreenPos.x);
	//			maxScreenPos.y = fmax(screenPos.y, maxScreenPos.y);
	//			if (id == 7) {
	//				head[i][1] = screenPos; // 
	//				distancess[i] = boneTo[i].Distance(ViewInfo.Locations);
	//			}
	//			screenBonePositions[id] = screenPos;
	//		}
	//	}
	//}
	if (espInstance.isToggled()) {
		if (health[i] >= 1 && teampointer2[i] != localteamindex) {
			ImVec2 boxPos(minScreenPos.x, minScreenPos.y);
			ImVec2 boxSize(maxScreenPos.x - minScreenPos.x, maxScreenPos.y - minScreenPos.y);
			int padding = 5;
			paddedBoxPos[i] = ImVec2(boxPos.x - padding, boxPos.y - padding);
			paddedBoxSize[i] = ImVec2(boxSize.x + 2 * padding, boxSize.y + 2 * padding);

			if (*ESP::i().isHealthBar) {
				int healthBarWidth = 5;
				ImVec2 healthBarPos = ImVec2(paddedBoxPos[i].x - healthBarWidth, paddedBoxPos[i].y);
				ImVec2 healthBarSize = ImVec2(healthBarWidth, paddedBoxSize[i].y);
				DrawHealthBar(i, maxhealth[i], health[i], healthBarPos, healthBarSize, false);

			}
			if (*(ESP::i().is2DBox)) {
				float thickness = 1.5f;
				AddColoredRect(drawList, paddedBoxPos[i], paddedBoxSize[i], ESP::i().col2DBox, thickness);
			}

			if (*(ESP::i().isPlayername)) {
				ImColor textColor = ImColor(255, 0, 255);

				playerName[i] = get_name_from_fstring(i);
				textSize[i] = ImGui::CalcTextSize(playerName[i].c_str());
				textPos[i] = ImVec2(paddedBoxPos[i].x + (paddedBoxSize[i].x - textSize[i].x) / 2, paddedBoxPos[i].y - textSize[i].y);
				AddColoredText(drawList, textPos[i], ESP::i().colPlayername, playerName[i].c_str());
			}

			if (*(ESP::i().isPlayerinfo)) {
				std::string teamText2 = "T: " + std::to_string(teampointer2[i]);
				std::string distanceText = " | D: " + std::to_string(static_cast<int>(distancess[i]));
				std::string displayText = teamText2 + distanceText;
				ImVec2 textSize2 = ImGui::CalcTextSize(displayText.c_str());
				ImVec2 textPos2 = ImVec2(paddedBoxPos[i].x + (paddedBoxSize[i].x - textSize2.x) / 2, paddedBoxPos[i].y + paddedBoxSize[i].y);
				ImColor textColor2 = ImColor(255, 255, 0); 
				AddColoredText(drawList, textPos2, ESP::i().colPlayerinfo, displayText.c_str());
			}


			if (*ESP::i().isSkeleton) {
				for (const auto& connection : boneConnections) {
					auto fromIt = screenBonePositions.find(connection.fromId);
					auto toIt = screenBonePositions.find(connection.toId);

					if (fromIt != screenBonePositions.end() && toIt != screenBonePositions.end()) {
						AddColoredLine(drawList, ImVec2(fromIt->second.x, fromIt->second.y), ImVec2(toIt->second.x, toIt->second.y), ESP::i().colSkeleton, 1.0f);
					}
				}
			}

		}
	
	}


}



void cheat()
{

	ShowFPS();
	if (*(AimAssist::i().relativeFOV)) {
		drawFov(AimAssist::i().fov->x);
	}
	dma.SClear(dma.hS2, VMMDLL_FLAG_NOCACHE);
	if (playerCount > 0)
	{
		for (auto kk = 0; kk < allplayer.sum; kk++) {

			entitylist temp2 = allplayer;
			int i = temp2.i[kk];

			dma.SPrepare(dma.hS2, playerarrayf + i * 8, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, actorpointer[i] + offsets::playername, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, actorpointer[i] + offsets::playernamecount, sizeof(uint32_t));
			for (uint32_t j = 0; j < num_chars[i]; ++j)
			{
				dma.SPrepare(dma.hS2, playername[i] + j * sizeof(wchar_t), sizeof(wchar_t));
			}

			dma.SPrepare(dma.hS2, actorpointer[i] + offsets::PawnPrivate, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, pawnpointer[i] + offsets::USquadComponent, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, teampointer1[i] + offsets::GetSquadIndex, sizeof(int));
			dma.SPrepare(dma.hS2, pawnpointer[i] + offsets::healthcomponent, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, h1[i] + offsets::currenthealth, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, h1[i] + offsets::maxhealth, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, pawnpointer[i] + offsets::USkeletalMeshComponent, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, bone[i] + offsets::MeshArray, sizeof(uint64_t));
			dma.SPrepare(dma.hS2, bone[i] + 0x668, sizeof(float));
			dma.SPrepare(dma.hS2, bone[i] + offsets::ComponentToWorld, sizeof(FTransform));

			for (int id : boneIds) {
				dma.SPrepare(dma.hS2, Mash[i] + (id * sizeof(FTransform)), sizeof(FTransform));
			}

		}
		dma.ExecuteMemoryReads(dma.hS2);

		for (auto kk = 0; kk < allplayer.sum; kk++) {
			entitylist temp2 = allplayer;
			int i = temp2.i[kk];

			actorpointer[i] = dma.SRead<uint64_t>(dma.hS2, playerarrayf + i * 8);
			pawnpointer[i] = dma.SRead<uint64_t>(dma.hS2, actorpointer[i] + offsets::PawnPrivate);
			teampointer1[i] = dma.SRead<uint64_t>(dma.hS2, pawnpointer[i] + offsets::USquadComponent);
			teampointer2[i] = dma.SRead<int>(dma.hS2, teampointer1[i] + offsets::GetSquadIndex);
			h1[i] = dma.SRead<uint64_t>(dma.hS2, pawnpointer[i] + offsets::healthcomponent);
			health[i] = dma.SRead<float>(dma.hS2, h1[i] + offsets::currenthealth);
			maxhealth[i] = dma.SRead<float>(dma.hS2, h1[i] + offsets::maxhealth);

			if (health[i] <= 0) { drawed[i] = false; continue;}
			if (maxhealth[i] > 350.0f) { drawed[i] = false; continue; }
			////bone
			bone[i] = dma.SRead<uint64_t>(dma.hS2, pawnpointer[i] + offsets::USkeletalMeshComponent);
			std::cout << "bone  " << bone[i] << std::endl;
			Mash[i] = dma.SRead<uint64_t>(dma.hS2, bone[i] + offsets::MeshArray);
			if (Mash[i] == 0) {
				Mash[i] = dma.SRead<uint64_t>(dma.hS2, bone[i] + offsets::cache);
			}
			ComponentToWorld[i] = dma.SRead<FTransform>(dma.hS2, bone[i] + offsets::ComponentToWorld);

			draw_skeleton(i);

			drawed[i] = true;

		}
	}

}




