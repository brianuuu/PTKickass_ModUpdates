#include "HudLoading.h"

int m_tailsTextStart = 0;
float m_tailsTextScrollTimer = 0.1f;
int const m_tailsTextSize = 34;
char const* m_tailsText[] =
{
	"<?tml version=\\\"1.0\\\" encode=\\\"TFF-S\\\" standalone=\\\"yes\\\"?>",
	"<assembly tmlns=\\\"urn:tails.com:asm.v1\\\" ApplicationVersion=\\\"1.0\\\">",
	"<assemblyIdentity",
	"version=\\\"1.0.0.0\\\"",
	"processor=\\\"T88\\\"",
	"name=\\\"TL1.exe\\\"",
	"type=\\\"Tal32\\\"",
	"/>",
	"<description>Description</description>",
	"<dependency>",
	"<dependent>",
	"<assemblyIdentity",
	"type=\\\"tails99\\\"",
	"name=\\\"Tales.Systems.Common-Controls\\\"",
	"version=\\\"1.2.3.4\\\"",
	"processor=\\\"T66\\\"",
	"KeyToken=\\\"stlwi19902828\\\"",
	"language=\\\"*\\\"",
	"/>",
	"</dependent>",
	"</dependency>",
	"<dependency>",
	"<dependent>",
	"<assemblyIdentity",
	"type=\\\"sonic64\\\"",
	"name=\\\"Sonic.Systems.Common-Controls\\\"",
	"version=\\\"1.2.3.4\\\"",
	"processor=\\\"S99\\\"",
	"KeyToken=\\\"98lgh12916\\\"",
	"language=\\\"*\\\"",
	"/>",
	"</dependent>",
	"</dependency>",
	"</assembly>",
};

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

void HudLoading_SetTailsText()
{
	rcLoadingPDA->GetNode("source_1")->SetText(m_tailsText[(m_tailsTextStart + 0) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_2")->SetText(m_tailsText[(m_tailsTextStart + 1) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_3")->SetText(m_tailsText[(m_tailsTextStart + 2) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_4")->SetText(m_tailsText[(m_tailsTextStart + 3) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_5")->SetText(m_tailsText[(m_tailsTextStart + 4) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_6")->SetText(m_tailsText[(m_tailsTextStart + 5) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_7")->SetText(m_tailsText[(m_tailsTextStart + 6) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_8")->SetText(m_tailsText[(m_tailsTextStart + 7) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_9")->SetText(m_tailsText[(m_tailsTextStart + 8) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_10")->SetText(m_tailsText[(m_tailsTextStart + 9) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_11")->SetText(m_tailsText[(m_tailsTextStart + 10) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_12")->SetText(m_tailsText[(m_tailsTextStart + 11) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_13")->SetText(m_tailsText[(m_tailsTextStart + 12) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_14")->SetText(m_tailsText[(m_tailsTextStart + 13) % m_tailsTextSize]);
	rcLoadingPDA->GetNode("source_15")->SetText(m_tailsText[(m_tailsTextStart + 14) % m_tailsTextSize]);
}

HOOK(int*, __fastcall, HudLoading_CHudLoadingCStateIdleBegin, 0x1092710, hh::fnd::CStateMachineBase::CStateBase* This)
{
	HudLoading_CreateScene(This);

	return originalHudLoading_CHudLoadingCStateIdleBegin(This);
}

bool m_isEvent = false;
bool m_loadingDisplayHint = false;
HOOK(void, __fastcall, HudLoading_CHudLoadingCStateIntroBegin, 0x10938F0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	char const* eventName = *(char**)Common::GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x80, 0x2C });
	m_isEvent = !std::string(eventName).empty();

	HudLoading_CreateScene(This);

	m_loadingDisplayHint = false;
	HudLoading_SetTailsText();

	if (m_isEvent)
	{
		m_loadingDisplayHint = true;
		HudLoading_PlayMotion(rcLoadingEvent, "loop", 0.0f, true);
		HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
	}
	else
	{
		HudLoading_PlayMotion(rcLoadingBG1, "Intro_Anim", 29.0f);

		uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
		rcLoadingPDA->GetNode("imgbox")->SetPatternIndex(stageID);
		rcLoadingPDA->GetNode("imgbox")->SetHideFlag(stageID == SMT_pam000 || Common::IsCurrentStageMission());
		rcLoadingPDA->GetNode("img2")->SetHideFlag(stageID != SMT_pam000);

		if (stageID == SMT_pam000)
		{
			HudLoading_PlayMotion(rcLoadingPDA, "Intro_Anim_1", 29.0f);
			HudLoading_PlayMotion(rcLoadingPDATxt, "Intro_Anim_1", 29.0f);
		}
		else if (Common::IsCurrentStageMission())
		{
			HudLoading_PlayMotion(rcLoadingPDA, "Intro_Anim_3", 29.0f);
			HudLoading_PlayMotion(rcLoadingPDATxt, "Intro_Anim_3", 29.0f);
			rcLoadingPDA->GetNode("hint")->SetHideFlag(true);
		}
		else
		{
			HudLoading_PlayMotion(rcLoadingPDA,"Intro_Anim_2", 29.0f);
			HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
			rcLoadingPDATxt->SetHideFlag(true);

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

			rcLoadingInfo->GetNode("controller")->SetHideFlag(true);
			rcLoadingInfo->GetNode("pos_text_sonic")->SetHideFlag(true);
			rcLoadingInfo->GetNode("pos_text_evil")->SetHideFlag(true);
			rcLoadingInfo->GetNode("imgbox_character")->SetHideFlag(false);
			rcLoadingInfo->GetNode("imgbox_character")->SetPatternIndex(hintPattern);
		}
	}

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

			if (!Common::IsCurrentStageMission())
			{
				HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
			}
		}

		m_loadingDisplayHint = true;
	}

	if (m_isEvent)
	{
		This->m_pStateMachine->ChangeState("Usual");
	}
	else if (rcLoadingPDA->m_MotionDisableFlag)
	{
		HudLoading_PlayMotion(rcLoadingPDA, stageID == SMT_pam000 ? "Usual_Anim_1" : (Common::IsCurrentStageMission() ? "Usual_Anim_3" : "Usual_Anim_2"));
		This->m_pStateMachine->ChangeState("Usual");
	}

	if (rcLoadingPDATxt->m_MotionDisableFlag)
	{
		HudLoading_PlayMotion(rcLoadingPDATxt, Common::IsCurrentStageMission() ? "Usual_Anim_3" : "Usual_Anim_2", 0.0f, true);
	}

	return nullptr;
}

HOOK(int, __fastcall, HudLoading_CHudLoadingCStateUsualAdvance, 0x10926E0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	HudLoading_SetTailsText();

	m_tailsTextScrollTimer -= This->m_pStateMachine->m_UpdateInfo.DeltaTime;
	if (m_tailsTextScrollTimer < 0.0f)
	{
		m_tailsTextScrollTimer = 0.1f;
		m_tailsTextStart = (m_tailsTextStart + 1) % m_tailsTextSize;
	}

	return originalHudLoading_CHudLoadingCStateUsualAdvance(This);
}

HOOK(int32_t*, __fastcall, HudLoading_CHudLoadingCStateOutroBegin, 0x1093410, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!m_isEvent)
	{
		HudLoading_PlayMotion(rcLoadingBG1, "Outro_Anim");
		HudLoading_PlayMotion(rcLoadingPDA, "Outro_Anim");
		HudLoading_PlayMotion(rcLoadingPDATxt, "Outro_Anim");
		HudLoading_PlayMotion(rcLoadingBG2, "Outro_Anim");
		rcLoadingInfo->SetHideFlag(true);

		uint8_t stageID = Common::GetCurrentStageID() & 0xFF;
		rcLoadingPDA->GetNode("img2")->SetHideFlag(stageID != SMT_pam000 && !Common::IsCurrentStageMission());
	}

	return originalHudLoading_CHudLoadingCStateOutroBegin(This);
}

HOOK(int, __fastcall, HudLoading_CHudLoadingCStateOutroTitleBegin, 0x1093380, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (m_isEvent)
	{
		rcLoadingEvent->SetHideFlag(true);
		rcLoadingPDATxt->SetHideFlag(true);
	}

	return originalHudLoading_CHudLoadingCStateOutroTitleBegin(This);
}

void __declspec(naked) HudLoading_StartResidentLoading()
{
	static uint32_t sub_75FA60 = 0x75FA60;
	static uint32_t returnAddress = 0x44A2F8;
	__asm
	{
		push	eax
		push	ecx
		call	HudLoading::StartResidentLoading
		pop		ecx
		pop		eax

		call	[sub_75FA60]
		jmp		[returnAddress]
	}
}

void __declspec(naked) HudLoading_EndResidentLoading()
{
	static uint32_t sub_75FA60 = 0x75FA60;
	static uint32_t returnAddress = 0x44A505;
	__asm
	{
		push	eax
		push	ecx
		call	HudLoading::EndResidentLoading
		pop		ecx
		pop		eax

		call	[sub_75FA60]
		jmp		[returnAddress]
	}
}

void HudLoading::Install()
{
	// Always use stage loading even on PAM
	WRITE_MEMORY(0x1093EB8, uint8_t, 0xEB);
	WRITE_NOP(0x109273D, 9);

	// Disable loading screen sfx
	WRITE_MEMORY(0x44A2E8, int, -1);
	WRITE_MEMORY(0x44A4F5, int, -1);
	WRITE_MEMORY(0x10933C1, int, -1);
	WRITE_MEMORY(0x1093651, int, -1);
	WRITE_MEMORY(0x1093562, int, -1);
	WRITE_MEMORY(0x10936EC, int, -1);

	// State hooks
	INSTALL_HOOK(HudLoading_CHudLoadingCStateIdleBegin);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateIntroBegin);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateIntroAdvance);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateUsualAdvance);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateOutroBegin);
	INSTALL_HOOK(HudLoading_CHudLoadingCStateOutroTitleBegin);

	// Put resident loading layer below loading
	WRITE_MEMORY(0xD6AE00, uint32_t, 0x1E66C20);

	// Resident Loading
	WRITE_JUMP(0x44A2F3, HudLoading_StartResidentLoading);
	WRITE_JUMP(0x44A500, HudLoading_EndResidentLoading);
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

void HudLoading::StartResidentLoading()
{
	if (!rcLoadingPDATxt) return;
	HudLoading_PlayMotion(rcLoadingPDATxt, "Usual_Anim_2", 0.0f, true);
}

void HudLoading::EndResidentLoading()
{
	if (!rcLoadingPDATxt) return;
	HudLoading_PlayMotion(rcLoadingPDATxt, "Outro_Anim");
}
