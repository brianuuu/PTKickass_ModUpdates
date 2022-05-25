#include "HudLoading.h"

Chao::CSD::RCPtr<Chao::CSD::CProject> rcProjectLoading;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingBG1;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingPDA;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingPDATxt;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingInfo;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingEvent;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcLoadingBG2;

void HudLoading_CreateScene(hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (rcProjectLoading) return;

	void* contextBase = This->GetContextBase();
	rcProjectLoading = *(Chao::CSD::RCPtr<Chao::CSD::CProject>*)((uint32_t)contextBase + 188);
	if (!rcProjectLoading) return;

	rcLoadingBG1 = rcProjectLoading->CreateScene("bg_1");
	rcLoadingBG1->SetHideFlag(true);
	rcLoadingPDA = rcProjectLoading->CreateScene("pda");
	rcLoadingPDA->SetHideFlag(true);
	rcLoadingPDATxt = rcProjectLoading->CreateScene("pda_txt");
	rcLoadingPDATxt->SetHideFlag(true);
	rcLoadingInfo = rcProjectLoading->CreateScene("loadinfo");
	rcLoadingInfo->SetHideFlag(true);
	rcLoadingEvent = rcProjectLoading->CreateScene("event_viewer");
	rcLoadingEvent->SetHideFlag(true);
	rcLoadingBG2 = rcProjectLoading->CreateScene("bg_2");
	rcLoadingBG2->SetHideFlag(true);
}

void HudLoading_PlayMotion(Chao::CSD::RCPtr<Chao::CSD::CScene>& scene, char const* motion, float startFrame = 0.0f, bool loop = false)
{
	if (!scene) return;
	scene->SetHideFlag(false);
	scene->SetMotion(motion);
	scene->SetMotionFrame(startFrame);
	scene->m_MotionDisableFlag = false;
	scene->m_MotionSpeed = 1.0f;
	scene->m_MotionRepeatType = loop ? Chao::CSD::eMotionRepeatType_Loop : Chao::CSD::eMotionRepeatType_PlayOnce;
	scene->Update();
}

HOOK(int*, __fastcall, HudLoading_CHudLoadingCStateIdleBegin, 0x1092710, hh::fnd::CStateMachineBase::CStateBase* This)
{
	HudLoading_CreateScene(This);

	return originalHudLoading_CHudLoadingCStateIdleBegin(This);
}

bool m_loadingDisplayHint = false;
HOOK(void, __fastcall, HudLoading_CHudLoadingCStateIntroBegin, 0x10938F0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	HudLoading_CreateScene(This);

	m_loadingDisplayHint = false;
	HudLoading_PlayMotion(rcLoadingBG1, "Intro_Anim", 29.0f);

	uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
	rcLoadingPDA->GetNode("imgbox")->SetPatternIndex(stageID);
	HudLoading_PlayMotion(rcLoadingPDA, "Intro_Anim_2", 29.0f);

	rcLoadingInfo->GetNode("controller")->SetHideFlag(true);
	rcLoadingInfo->GetNode("pos_text_sonic")->SetHideFlag(true);
	rcLoadingInfo->GetNode("pos_text_evil")->SetHideFlag(true);
	rcLoadingInfo->GetNode("imgbox_character")->SetHideFlag(stageID == SMT_pam000);

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	int hintPattern = std::rand() % 7;
	if (stageID == SMT_blb)
	{
		hintPattern = 7;
		HudLoading_PlayMotion(rcLoadingInfo, "360_super");
	}
	else
	{
		HudLoading_PlayMotion(rcLoadingInfo, "360_sonic3");
	}
	rcLoadingInfo->GetNode("imgbox_character")->SetPatternIndex(hintPattern);

	originalHudLoading_CHudLoadingCStateIntroBegin(This);
}

HOOK(int32_t*, __fastcall, HudLoading_CHudLoadingCStateIntroAdvance, 0x10936B0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
	if (!m_loadingDisplayHint && rcLoadingPDA->m_MotionFrame > 45.0f)
	{
		if (stageID != SMT_pam000)
		{
			rcLoadingInfo->GetNode("controller")->SetHideFlag(false);
			rcLoadingInfo->GetNode("pos_text_sonic")->SetHideFlag(false);
			rcLoadingInfo->GetNode("pos_text_evil")->SetHideFlag(false);
		}

		HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
		m_loadingDisplayHint = true;
	}

	if (rcLoadingPDA->m_MotionDisableFlag)
	{
		HudLoading_PlayMotion(rcLoadingPDA, "Usual_Anim_2");
		This->m_pStateMachine->ChangeState("Usual");
	}

	return nullptr;
}

HOOK(int32_t*, __fastcall, HudLoading_CHudLoadingCStateOutroBegin, 0x1093410, hh::fnd::CStateMachineBase::CStateBase* This)
{
	HudLoading_PlayMotion(rcLoadingBG1, "Outro_Anim");
	HudLoading_PlayMotion(rcLoadingPDA, "Outro_Anim");
	HudLoading_PlayMotion(rcLoadingPDATxt, "Outro_Anim");
	HudLoading_PlayMotion(rcLoadingBG2, "Outro_Anim");
	rcLoadingInfo->SetHideFlag(true);

	return originalHudLoading_CHudLoadingCStateOutroBegin(This);
}

void HudLoading::Install()
{
	// Always use stage loading even on PAM
	WRITE_MEMORY(0x1093EB8, uint8_t, 0xEB);
	WRITE_NOP(0x109273D, 9);

	// Disable loading screen sfx
	WRITE_MEMORY(0x10933C1, int, -1);
	WRITE_MEMORY(0x1093651, int, -1);
	WRITE_MEMORY(0x1093562, int, -1);
	WRITE_MEMORY(0x10936EC, int, -1);

	INSTALL_HOOK(HudLoading_CHudLoadingCStateIdleBegin);

	INSTALL_HOOK(HudLoading_CHudLoadingCStateIntroBegin);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateIntroAdvance);

	INSTALL_HOOK(HudLoading_CHudLoadingCStateOutroBegin);
}

void HudLoading::StartFadeOut()
{
	if (!rcLoadingBG1) return;
	HudLoading_PlayMotion(rcLoadingBG1, "Intro_Anim");
}

bool HudLoading::IsFadeOutCompleted()
{
	if (!rcLoadingBG1) return true;
	return rcLoadingBG1->m_MotionDisableFlag;
}
