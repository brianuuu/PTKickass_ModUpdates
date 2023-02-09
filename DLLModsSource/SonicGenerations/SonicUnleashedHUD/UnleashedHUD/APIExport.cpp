#pragma once
#include "HudResult.h"

extern "C" __declspec(dllexport) void API_AddRingScore(int score)
{
	HudResult::AddRingScore(score);
}

extern "C" __declspec(dllexport) void API_AddSpeedScore(int score)
{
	HudResult::AddSpeedScore(score);
}

extern "C" __declspec(dllexport) void API_AddEnemyScore(int score)
{
	HudResult::AddEnemyScore(score);
}

extern "C" __declspec(dllexport) void API_AddTrickScore(int score)
{
	HudResult::AddTrickScore(score);
}
