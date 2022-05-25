#pragma once
class HudLoading
{
public:
	static void Install();

	static void StartFadeOut();
	static bool IsFadeOutCompleted();
	static void StartResidentLoading();
	static void EndResidentLoading();
};

