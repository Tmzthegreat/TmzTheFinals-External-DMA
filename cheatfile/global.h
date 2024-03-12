#include <cstddef>
#include "struct.hpp"
//#include "imgui/OS-ImGui_Struct.h"
#include <vector>
//#include <mutex>
//#include <shared_mutex>
#pragma once
//std::shared_timed_mutex mtx;
entitylist allplayer;
FMinimalViewInfo ViewInfo;
double distancess[1000];
uint64_t UWorldf = NULL;
uint64_t UWorld2f = NULL;
uint64_t UWorld3f = NULL;
uint64_t modbase2 = NULL;
uint64_t level = NULL;
uint64_t worldactor = NULL;
int worldcount;
int playerCount;
uint64_t GameStatef = NULL;
uint64_t GameStatef2 = NULL;
uint64_t playerarrayf = NULL;
uint64_t playerarrayf2 = NULL;
//int playerCountf;
uint64_t matrixf = NULL;
uint64_t matrix2f = NULL;
uint64_t matrix3f = NULL;
uint64_t matrix4f = NULL;
uint64_t matrix5f = NULL;
uint64_t viewview = NULL;
uint64_t bonematrix = NULL;
//MAT4X4 bonematrix;
uint64_t actoraddress = NULL;
uint64_t pawn = NULL;
uint64_t rootComponentAddress = NULL;
Vec3 Location;
//uint64_t matrix5f = NULL;
uint64_t actorpointer[100];
uint64_t actorpointer2[100];
uint64_t pawnpointer[100];
uint64_t rootpointer[100];
uint64_t teampointer1[100];
int teampointer2[100];
uint64_t playername[10000];

//self
uint64_t owning = NULL;
uint64_t LocalPlayers = NULL;
uint64_t self = NULL;
uint64_t localcontroller = NULL;
uint64_t ackpawn = NULL;
uint64_t localteam = NULL;
int localteamindex = NULL;

//health
uint64_t h1[100];
uint64_t h2[100];
uint64_t h3[100];
float health[10000];
float maxhealth[10000];
//bone
uint64_t bone[1000];
uint64_t Mash[1000];
FTransform old_bone[11000][100];
FTransform bone2[11000][100];
FTransform ComponentToWorld[11000];
FVector headBone[100];
FVector boneFrom[100];
FVector boneTo[100];
Vec3 SPos[11000][1000];
Vec3 WPos = Vec3(0, 0, 0);
std::vector<int> boneIds = { 1, 4, 7, 8, 9, 10, 11, 32, 33, 34, 65, 66, 67, 69, 70, 71 };
//;
//aimbot
Vec3 head[100][100];
Vec3 feet[100][100];
Vec3 feet2[100][100];
int leftkey;
int rightkey;
int sidekey1;
int sidekey2;
float fovRadius = 50.0f; 
std::vector<bool> currentKeyState(256, false);
std::vector<bool> recentKeyState(256, false);
double screenWidth = GetSystemMetrics(SM_CXSCREEN);

double screenHeight = GetSystemMetrics(SM_CYSCREEN);

const int screenCenterX = screenWidth / 2;
const int screenCenterY = screenHeight / 2;
//const int radius = 200;
//Vec3 selectedTarget;
float aim_sensitive = 1;
float speed = 60;
int targetIndex;

bool checkaim = FALSE;

int ii = 0;


Vec3 minScreenPos, maxScreenPos;

bool drawed[11000];

std::string name[10000];

uint32_t num_chars[1000];

std::wstring playerNameW[10000];
std::string playerName[10000];

ImVec2 textPos[10000];

ImVec2 paddedBoxPos[10000];
ImVec2 paddedBoxSize[10000];

ImVec2 textSize[1000];

D3DMATRIX tempMatrix;

float mindraw[1000];

float maxdraw[1000];

float cachemaxdraw[1000];