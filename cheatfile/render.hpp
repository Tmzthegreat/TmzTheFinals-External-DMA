#pragma once

#include <chrono>
#include <map>
#include <imgui.h>
#include <WinUser.h>
//#include "global.h"
//#include "aimbot.hpp"

void DrawRect(int x, int y, int w, int h, ImColor color, int thickness)
{
    //ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.2f)));
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, 1.0f);



}



void DrawPlayerBox(int headCenterX, int headCenterY, int boxWidth, int boxHeight, ImColor color, int thickness) {
    int x = headCenterX - boxWidth / 2;
    int y = headCenterY; // y is now the position of the player's head

    // Uncomment if you want to draw a filled rectangle
    //ImGui::GetBackgroundDrawList()->AddRectFilled(
    //    ImVec2(x, y),
    //    ImVec2(x + boxWidth, y + boxHeight),
    //    ImGui::ColorConvertFloat4ToU32(ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.2f))
    //);

    ImGui::GetBackgroundDrawList()->AddRect(
        ImVec2(x, y),
        ImVec2(x + boxWidth, y + boxHeight),
        ImGui::ColorConvertFloat4ToU32(color),
        0.0f,
        0,
        static_cast<float>(thickness)
    );
}

void ShowFPS()
{
	ImGuiIO& io = ImGui::GetIO();
	float fps = io.Framerate;
	ImVec2 text_pos(10, 10); 

	std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));

	ImGui::GetBackgroundDrawList()->AddText(text_pos, ImColor(255, 0, 255, 255) , fpsText.c_str());

}


class HealthBar
{
private:
	using TimePoint_ = std::chrono::steady_clock::time_point;
private:
	// ��ʾ����Ѫ��ʱ��(ms)
	const int ShowBackUpHealthDuration = 500;
	// ���Ѫ��
	float MaxHealth = 0.f;
	// ��ǰѪ��
	float CurrentHealth = 0.f;
	// �������Ѫ����С
	float LastestBackupHealth = 0.f;
	// Ѫ������
	ImVec2 RectPos{};
	// Ѫ����С
	ImVec2 RectSize{};
	// ������ʾ����Ѫ��
	bool InShowBackupHealth = false;
	// ��ʾ����Ѫ����ʼʱ���
	TimePoint_ BackupHealthTimePoint{};
public:
	HealthBar() {}
	// ����
	void DrawHealthBar_Horizontal(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size);
	// ����
	void DrawHealthBar_Vertical(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size);
private:
	// ��ɫ����
	ImColor Mix(ImColor Col_1, ImColor Col_2, float t);
	// ��һ�׶�Ѫ����ɫ 0.5-1
	ImColor FirstStageColor = ImColor(96, 246, 113, 220);
	// �ڶ��׶�Ѫ����ɫ 0.5-0.2
	ImColor SecondStageColor = ImColor(247, 214, 103, 220);
	// �����׶�Ѫ����ɫ 0.2-0.0
	ImColor ThirdStageColor = ImColor(255, 95, 95, 220);
	// ����Ѫ����ɫ
	ImColor BackupHealthColor = ImColor(255, 255, 255, 220);
	// �߿���ɫ
	ImColor FrameColor = ImColor(45, 45, 45, 220);
	// ������ɫ
	ImColor BackGroundColor = ImColor(90, 90, 90, 220);
};

void HealthBar::DrawHealthBar_Horizontal(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size)
{
	auto InRange = [&](float value, float min, float max) -> bool
		{
			return value > min && value <= max;
		};

	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

	this->MaxHealth = MaxHealth;
	this->CurrentHealth = CurrentHealth;
	this->RectPos = Pos;
	this->RectSize = Size;

	// ռ��
	float Proportion = CurrentHealth / MaxHealth;
	// Ѫ�������
	float Width = RectSize.x * Proportion;
	// Ѫ������ɫ
	ImColor Color;

	// ����
	DrawList->AddRectFilled(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		BackGroundColor, 5, 15);

	// ��ɫ�л�
	float Color_Lerp_t = pow(Proportion, 2.5);
	if (InRange(Proportion, 0.5, 1))
		Color = Mix(FirstStageColor, SecondStageColor, Color_Lerp_t * 3 - 1);
	else
		Color = Mix(SecondStageColor, ThirdStageColor, Color_Lerp_t * 4);

	// �����������Ѫ��
	if (LastestBackupHealth == 0
		|| LastestBackupHealth < CurrentHealth)
		LastestBackupHealth = CurrentHealth;

	if (LastestBackupHealth != CurrentHealth)
	{
		if (!InShowBackupHealth)
		{
			BackupHealthTimePoint = std::chrono::steady_clock::now();
			InShowBackupHealth = true;
		}

		std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
		if (CurrentTime - BackupHealthTimePoint > std::chrono::milliseconds(ShowBackUpHealthDuration))
		{
			// ��ʱ��ֹͣ��ʾ����Ѫ�������Ҹ����������Ѫ��
			LastestBackupHealth = CurrentHealth;
			InShowBackupHealth = false;
		}

		if (InShowBackupHealth)
		{
			// ����Ѫ�����ƿ��
			float BackupHealthWidth = LastestBackupHealth / MaxHealth * RectSize.x;
			// ����Ѫ��alpha����
			float BackupHealthColorAlpha = 1 - 0.95 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			ImColor BackupHealthColorTemp = BackupHealthColor;
			BackupHealthColorTemp.Value.w = BackupHealthColorAlpha;
			// ����Ѫ����Ȼ���
			float BackupHealthWidth_Lerp = 1 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			BackupHealthWidth_Lerp *= (BackupHealthWidth - Width);
			BackupHealthWidth -= BackupHealthWidth_Lerp;
			// ����Ѫ��
			DrawList->AddRectFilled(RectPos,
				{ RectPos.x + BackupHealthWidth,RectPos.y + RectSize.y },
				BackupHealthColorTemp, 5);
		}
	}

	// Ѫ��
	DrawList->AddRectFilled(RectPos,
		{ RectPos.x + Width,RectPos.y + RectSize.y },
		Color, 5);

	// �߿�
	DrawList->AddRect(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		FrameColor, 5, 15, 1);
}

void HealthBar::DrawHealthBar_Vertical(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size)
{
	auto InRange = [&](float value, float min, float max) -> bool
		{
			return value > min && value <= max;
		};

	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

	this->MaxHealth = MaxHealth;
	this->CurrentHealth = CurrentHealth;
	this->RectPos = Pos;
	this->RectSize = Size;

	// ռ��
	float Proportion = CurrentHealth / MaxHealth;
	// Ѫ�����߶�
	float Height = RectSize.y * Proportion;
	// Ѫ������ɫ
	ImColor Color;

	// ����
	DrawList->AddRectFilled(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		BackGroundColor, 5, 15);

	// ��ɫ�л�
	float Color_Lerp_t = pow(Proportion, 2.5);
	if (InRange(Proportion, 0.5, 1))
		Color = Mix(FirstStageColor, SecondStageColor, Color_Lerp_t * 3 - 1);
	else
		Color = Mix(SecondStageColor, ThirdStageColor, Color_Lerp_t * 4);

	// �����������Ѫ��
	if (LastestBackupHealth == 0
		|| LastestBackupHealth < CurrentHealth)
		LastestBackupHealth = CurrentHealth;

	if (LastestBackupHealth != CurrentHealth)
	{
		if (!InShowBackupHealth)
		{
			BackupHealthTimePoint = std::chrono::steady_clock::now();
			InShowBackupHealth = true;
		}

		std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
		if (CurrentTime - BackupHealthTimePoint > std::chrono::milliseconds(ShowBackUpHealthDuration))
		{
			// ��ʱ��ֹͣ��ʾ����Ѫ�������Ҹ����������Ѫ��
			LastestBackupHealth = CurrentHealth;
			InShowBackupHealth = false;
		}

		if (InShowBackupHealth)
		{
			// ����Ѫ�����Ƹ߶�
			float BackupHealthHeight = LastestBackupHealth / MaxHealth * RectSize.y;
			// ����Ѫ��alpha����
			float BackupHealthColorAlpha = 1 - 0.95 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			ImColor BackupHealthColorTemp = BackupHealthColor;
			BackupHealthColorTemp.Value.w = BackupHealthColorAlpha;
			// ����Ѫ���߶Ȼ���
			float BackupHealthHeight_Lerp = 1 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			BackupHealthHeight_Lerp *= (BackupHealthHeight - Height);
			BackupHealthHeight -= BackupHealthHeight_Lerp;
			// ����Ѫ��
			DrawList->AddRectFilled({ RectPos.x,RectPos.y + RectSize.y - BackupHealthHeight },
				{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
				BackupHealthColorTemp, 5);
		}
	}

	// Ѫ��
	DrawList->AddRectFilled({ RectPos.x,RectPos.y + RectSize.y - Height },
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		Color, 5);

	// �߿�
	DrawList->AddRect(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		FrameColor, 5, 15, 1);
}

ImColor HealthBar::Mix(ImColor Col_1, ImColor Col_2, float t)
{
	ImColor Col;
	Col.Value.x = t * Col_1.Value.x + (1 - t) * Col_2.Value.x;
	Col.Value.y = t * Col_1.Value.y + (1 - t) * Col_2.Value.y;
	Col.Value.z = t * Col_1.Value.z + (1 - t) * Col_2.Value.z;
	Col.Value.w = Col_1.Value.w;
	return Col;
}

// Sign�����κ����͵��˱�ʶ��Ĭ�Ͽɴ����˵�ַ
void DrawHealthBar(DWORD Sign, float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool Horizontal)
{
	static std::map<DWORD, HealthBar> HealthBarMap;
	if (!HealthBarMap.count(Sign))
	{
		HealthBarMap.insert({ Sign,HealthBar() });
	}
	if (HealthBarMap.count(Sign))
	{
		if (Horizontal)
			HealthBarMap[Sign].DrawHealthBar_Horizontal(MaxHealth, CurrentHealth, Pos, Size);
		else
			HealthBarMap[Sign].DrawHealthBar_Vertical(MaxHealth, CurrentHealth, Pos, Size);
	}
}


void drawFov(float radius) {

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	double screenWidth1 = GetSystemMetrics(SM_CXSCREEN);
	// ��ȡ��Ļ�߶�
	double screenHeight1 = GetSystemMetrics(SM_CYSCREEN);
	// ���� FOV Բ�εĲ���
	ImVec2 center = ImVec2(screenWidth1 / 2, screenHeight1 / 2); // ��Ļ����
	//float radius = 100.0f; // FOV �뾶
	ImU32 col = IM_COL32(255, 0, 0, 255); // ��ɫ

	// ����Բ��
	draw_list->AddCircle(center, radius, col);
}

void DrawFilledRect2(int x, int y, int w, int h, ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}
void DrawNormalBox(int x, int y, int w, int h, int borderPx, ImColor color)
{
	DrawFilledRect2(x + borderPx, y, w, borderPx, color);
	DrawFilledRect2(x + w - w + borderPx, y, w, borderPx, color);
	DrawFilledRect2(x, y, borderPx, h, color);
	DrawFilledRect2(x, y + h - h + borderPx * 2, borderPx, h, color);
	DrawFilledRect2(x + borderPx, y + h + borderPx, w, borderPx, color);
	DrawFilledRect2(x + w - w + borderPx, y + h + borderPx, w, borderPx, color);
	DrawFilledRect2(x + w + borderPx, y, borderPx, h, color);
	DrawFilledRect2(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);
}

void Drawplayerinfo(int x, int y, int w, int h, ImColor color, int thickness, int i, float health,float maxhealth, int teamId, ImDrawList* drawList) {

	drawList->AddRect(ImVec2(x, y), ImVec2(w, h), color, 0, 0, thickness);

	ImVec2 healthBarPos = ImVec2(x - 6, y);
	ImVec2 healthBarSize = ImVec2(4, h);

	DrawHealthBar(i, maxhealth, health, healthBarPos, healthBarSize, false);

	ImVec2 teamIdPos = ImVec2(x - 20, y - 20);

	char teamIdStr[32];
	sprintf_s(teamIdStr, "Team %d", teamId);

	drawList->AddText(teamIdPos, IM_COL32(255, 0, 255, 255), teamIdStr);
}

ImColor GetRainbowColor(float speed, float& rainbow, float alpha = 255.0f) {
	rainbow += speed;
	if (rainbow > 1.0f) rainbow -= 1.0f;

	ImVec4 color = ImColor::HSV(rainbow, 0.6f, 0.6f);
	color.w = alpha / 255.0f;
	return ImColor(color);
}

float rainbow = 0.0f; 

void RenderRainbowBox(ImDrawList* drawList, const ImVec2& boxPos, const ImVec2& boxSize, float thickness, float speed) {
	ImColor rainbowColor = GetRainbowColor(speed, rainbow, 255.0f); 

	drawList->AddRect(boxPos, ImVec2(boxPos.x + boxSize.x, boxPos.y + boxSize.y), rainbowColor, 0.0f, 0, thickness);
}


void AddColoredRect(ImDrawList* drawList, const ImVec2& boxPos, const ImVec2& boxSize, const ImVec4* color, float thickness) {
	if (drawList == nullptr || color == nullptr) {
		return; 
	}

	ImVec2 boxEnd = ImVec2(boxPos.x + boxSize.x, boxPos.y + boxSize.y);
	ImU32 colU32 = ImGui::ColorConvertFloat4ToU32(*color);

	drawList->AddRect(boxPos, boxEnd, colU32, 0.0f, 0, thickness);
}


void AddColoredLine(ImDrawList* drawList, const ImVec2& from, const ImVec2& to, const ImVec4* color, float thickness) {
	if (drawList == nullptr || color == nullptr) {
		return; 
	}

	ImU32 colU32 = ImGui::ColorConvertFloat4ToU32(*color); 

	drawList->AddLine(from, to, colU32, thickness);
}

void AddColoredText(ImDrawList* drawList, const ImVec2& textPos, const ImVec4* color, const char* text) {
	if (drawList == nullptr || color == nullptr || text == nullptr) {
		return; 
	}

	ImU32 colU32 = ImGui::ColorConvertFloat4ToU32(*color); 
	drawList->AddText(textPos, colU32, text);
}