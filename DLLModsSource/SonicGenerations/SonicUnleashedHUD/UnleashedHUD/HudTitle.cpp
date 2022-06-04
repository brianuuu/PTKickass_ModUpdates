Chao::CSD::RCPtr<Chao::CSD::CProject> rcTitleScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcTitleLogo_1, rcTitlebg, rcTitleMenu, rcTitleMenuScroll, rcTitleMenuTXT;
boost::shared_ptr<Sonic::CGameObjectCSD> spTitleScreen;
int currentTitleIndex = 0;

void HudTitle::CreateScreen(Sonic::CGameObject* pParentGameObject)
{
	if (rcTitleScreen && !spTitleScreen)
		pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spTitleScreen = boost::make_shared<Sonic::CGameObjectCSD>(rcTitleScreen, 0.5f, "HUD", false), "main", pParentGameObject);
}

void HudTitle::KillScreen()
{
	if (spTitleScreen)
	{
		spTitleScreen->SendMessage(spTitleScreen->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		spTitleScreen = nullptr;
	}
}

void HudTitle::ToggleScreen(const bool visible, Sonic::CGameObject* pParentGameObject)
{
	if (visible)
		CreateScreen(pParentGameObject);
	else
		KillScreen();
}

void HudTitle::FreezeMotion(Chao::CSD::CScene* pScene)
{
	pScene->SetMotionFrame(pScene->m_MotionEndFrame);
	pScene->m_MotionSpeed = 0.0f;
	pScene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
}

void HudTitle::PlayAnimation(Chao::CSD::CScene* pScene, const char* name, Chao::CSD::EMotionRepeatType repeatType, float motionSpeed, float startFrame)
{
	pScene->SetMotion(name);
	pScene->m_MotionRepeatType = repeatType;
	pScene->m_MotionDisableFlag = 0;
	pScene->m_MotionSpeed = motionSpeed;
	pScene->SetMotionFrame(startFrame);
	pScene->Update(0);
}

void HudTitle::IntroAnim(Chao::CSD::RCPtr<Chao::CSD::CScene> scene)
{
	scene->SetMotion("Intro_Anim");
	scene->SetMotionFrame(0.0f);
	scene->m_MotionSpeed = 1;
	scene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scene->Update(0.0f);
}

void UnleashedTitleText()
{
	rcTitleMenuTXT->SetPosition(0, 0);

	if (currentTitleIndex < 0)
		currentTitleIndex = 4;
	else if(currentTitleIndex > 4)
		currentTitleIndex = 0;

	rcTitleMenuTXT->GetNode("txt_0")->SetHideFlag(!(currentTitleIndex == 0));
	rcTitleMenuTXT->GetNode("txt_1")->SetHideFlag(!(currentTitleIndex == 1));
	rcTitleMenuTXT->GetNode("txt_2")->SetHideFlag(!(currentTitleIndex == 2));
	rcTitleMenuTXT->GetNode("txt_3")->SetHideFlag(!(currentTitleIndex == 3));

#if _DEBUG
	printf("\nCURRENT INDEX: %d", currentTitleIndex);
#endif

	HudTitle::PlayAnimation(*rcTitleMenuTXT, "Intro_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
}

HOOK(int, __fastcall, CTitleMain, 0x0056FBE0, Sonic::CGameObject* This, void* Edx, int a2, int a3, void** a4)
{
	rcTitleScreen = nullptr;
	originalCTitleMain(This, Edx, a2, a3, a4);
	rcTitleScreen = nullptr;

	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	auto spCsdProject = wrapper.GetCsdProject("ui_title_unleashed");
	rcTitleScreen = spCsdProject->m_rcProject;

	rcTitleLogo_1 = rcTitleScreen->CreateScene("title_1");
	rcTitlebg = rcTitleScreen->CreateScene("bg");
	rcTitleMenu = rcTitleScreen->CreateScene("menu");
	rcTitleMenuScroll = rcTitleScreen->CreateScene("menu_scroll");
	rcTitleMenuTXT = rcTitleScreen->CreateScene("txt");
	HudTitle::FreezeMotion(*rcTitleMenuTXT);
	rcTitleMenuTXT->GetNode("txt_4")->SetHideFlag(true);
	HudTitle::FreezeMotion(*rcTitleMenuScroll);
	HudTitle::FreezeMotion(*rcTitleMenu);
	HudTitle::FreezeMotion(*rcTitlebg);
	HudTitle::PlayAnimation(*rcTitleMenu, "Intro_Anim_1", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	HudTitle::PlayAnimation(*rcTitlebg, "Intro_Anim_1", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	rcTitleMenuScroll->SetPosition(0, 650000);
	rcTitleMenuTXT->SetPosition(0, 650000);
	HudTitle::PlayAnimation(*rcTitleMenuScroll, "Scroll_Anim_2_1", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 0);
	HudTitle::FreezeMotion(*rcTitleMenuScroll);
	HudTitle::PlayAnimation(*rcTitleLogo_1, "Intro_Anim_1", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	HudTitle::CreateScreen(This);

	return 0;
}

HOOK(int, __fastcall, CTitleMainSelect, 0x11D1210, int a1)
{
	HudTitle::FreezeMotion(*rcTitleMenu);
	HudTitle::PlayAnimation(*rcTitlebg, "Intro_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	HudTitle::PlayAnimation(*rcTitleMenu, "Intro_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	rcTitleMenuTXT->SetPosition(0, 0);
	UnleashedTitleText();
	rcTitleMenuScroll->SetPosition(0, 0);

	return originalCTitleMainSelect(a1);
}

HOOK(int, __fastcall, CTitleMainWait, 0x11D1410, int a1)
{
	HudTitle::PlayAnimation(*rcTitleMenu, "Intro_Anim_1", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	HudTitle::PlayAnimation(*rcTitlebg, "Intro_Anim_1", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
	HudTitle::PlayAnimation(*rcTitleMenuScroll, "Scroll_Anim_2_1", Chao::CSD::eMotionRepeatType_PlayOnce, 0, 0);
	rcTitleMenuTXT->SetPosition(0, 65000);
	rcTitleMenuScroll->SetPosition(0, 65000);
	return originalCTitleMainWait(a1);
}

HOOK(int, __fastcall, CTitleMainFinish, 0x5727F0, Sonic::CGameObject* This)
{
	auto inputState = Sonic::CInputState::GetInstance();
	auto inputPtr = &inputState->m_PadStates[inputState->m_CurrentPadStateIndex];

	if (inputPtr->LeftStickVertical >= 0.5f)
	{
		currentTitleIndex -= 1;
		HudTitle::PlayAnimation(*rcTitleMenu, "Scroll_Anim_2_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
		UnleashedTitleText();
	}

	if (inputPtr->LeftStickVertical < -0.5f)
	{
		currentTitleIndex += 1;
		HudTitle::PlayAnimation(*rcTitleMenu, "Scroll_Anim_2_1", Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
		UnleashedTitleText();
	}

	return 0;
}

void HudTitle::Install()
{
	INSTALL_HOOK(CTitleMain);
	INSTALL_HOOK(CTitleMainFinish);
	INSTALL_HOOK(CTitleMainWait);
	INSTALL_HOOK(CTitleMainSelect);
}