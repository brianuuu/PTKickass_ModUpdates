#include "HudPause.h"

Chao::CSD::RCPtr<Chao::CSD::CProject> m_projectPause;
boost::shared_ptr<Sonic::CGameObjectCSD> m_spPause;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseBG;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseHeader;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseWindow;
Chao::CSD::RCPtr<Chao::CSD::CScene> m_scenePauseSelect;
uint32_t m_cursorPos = 0;

void HudPause_PlayMotion(Chao::CSD::RCPtr<Chao::CSD::CScene>& scene, char const* motion, bool loop = false, float start = 0.0f, float end = -1.0f)
{
    if (!scene) return;
    scene->SetHideFlag(false);
    scene->SetMotion(motion);
    scene->SetMotionFrame(start);
    if (end > start)
    {
        scene->m_MotionEndFrame = end;
    }
    scene->m_MotionDisableFlag = false;
    scene->m_MotionSpeed = 1.0f;
    scene->m_MotionRepeatType = loop ? Chao::CSD::eMotionRepeatType_Loop : Chao::CSD::eMotionRepeatType_PlayOnce;
    scene->Update();
}

void HudPause_StopMotion(Chao::CSD::RCPtr<Chao::CSD::CScene>& scene, char const* motion, float frame)
{
    if (!scene) return;
    scene->SetHideFlag(false);
    scene->SetMotion(motion);
    scene->SetMotionFrame(frame);
    scene->m_MotionDisableFlag = true;
    scene->m_MotionSpeed = 0.0f;
    scene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
    scene->Update();
}

void __fastcall HudPause_CPauseRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
    if (m_spPause)
    {
        m_spPause->SendMessage(m_spPause->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
        m_spPause = nullptr;
    }

    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseBG);
    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseHeader);
    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseWindow);
    Chao::CSD::CProject::DestroyScene(m_projectPause.Get(), m_scenePauseSelect);

    m_projectPause = nullptr;
}

void HudPause_CreatePauseScreen(uint32_t* This)
{
    Sonic::CGameObject* gameObject = (Sonic::CGameObject*)This[1];
    HudPause_CPauseRemoveCallback(gameObject, nullptr, nullptr);

    Sonic::CCsdDatabaseWrapper wrapper(gameObject->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
    m_projectPause = wrapper.GetCsdProject("ui_pause_swa")->m_rcProject;

    m_scenePauseBG = m_projectPause->CreateScene("bg");
    m_scenePauseBG->SetHideFlag(true);
    m_scenePauseHeader = m_projectPause->CreateScene("status_title");
    m_scenePauseHeader->SetHideFlag(true);
    m_scenePauseWindow = m_projectPause->CreateScene("bg_1");
    m_scenePauseWindow->SetHideFlag(true);
    m_scenePauseSelect = m_projectPause->CreateScene("bg_1_select");
    m_scenePauseSelect->SetHideFlag(true);

    if (m_projectPause && !m_spPause)
    {
        m_spPause = boost::make_shared<Sonic::CGameObjectCSD>(m_projectPause, 0.5f, "HUD_A2", true);
        Sonic::CGameDocument::GetInstance()->AddGameObject(m_spPause, "main", gameObject);
    }
}

void HudPause_OpenPauseWindow()
{
    m_cursorPos = 0;
    m_scenePauseWindow->GetNode("center")->SetScale(62.0f, 0.9f);
    m_scenePauseWindow->GetNode("text_area")->SetScale(62.0f, 0.9f);
    HudPause_PlayMotion(m_scenePauseWindow, "Intro_Anim");
}

void HudPause_OpenPauseScreen()
{
    HudPause_PlayMotion(m_scenePauseBG, "Intro_Anim");
    HudPause_PlayMotion(m_scenePauseHeader, "Intro_Anim");

    HudPause_StopMotion(m_scenePauseSelect, "Size_Anim", 8.0f);
    HudPause_PlayMotion(m_scenePauseSelect, "Usual_Anim", true);
    HudPause_StopMotion(m_scenePauseSelect, "Scroll_Anim", 0.0f);
    m_scenePauseSelect->SetHideFlag(true);

    HudPause_OpenPauseWindow();
}

void HudPause_ClosePauseScreen()
{
    m_scenePauseBG->SetHideFlag(true);
    m_scenePauseHeader->SetHideFlag(true);
    m_scenePauseWindow->SetHideFlag(true);
    m_scenePauseSelect->SetHideFlag(true);
}

HOOK(bool, __fastcall, HudPause_CPauseVisualActInit, 0x109FAD0, uint32_t* This)
{
    HudPause_CreatePauseScreen(This);
    return originalHudPause_CPauseVisualActInit(This);
}

HOOK(bool, __fastcall, HudPause_CPauseVisualActOpened, 0x109F8F0, uint32_t* This)
{
    return m_scenePauseWindow->m_MotionDisableFlag == 1;
}

HOOK(int, __fastcall, HudPause_CPauseVisualActCase, 0x109F910, uint32_t* This, void* Edx, int Case)
{
    int result = originalHudPause_CPauseVisualActCase(This, Edx, Case);

    uint32_t cursorPos = This[3];
    auto& originalScene = *(Chao::CSD::RCPtr<Chao::CSD::CScene>*)(&This[9]);
    switch (Case)
    {
    case 0: // Start
    {
        originalScene->SetHideFlag(true);
        HudPause_OpenPauseScreen();
        break;
    }
    case 1: // End
    {
        originalScene->SetHideFlag(true);
        break;
    }
    case 2: // Start animation complete
    {
        originalScene->SetHideFlag(false);
        m_scenePauseSelect->SetHideFlag(false);
        break;
    }
    case 3: // New cursor pos
    {
        originalScene->SetMotionFrame(originalScene->m_MotionEndFrame);
        if (cursorPos == m_cursorPos + 1)
        {
            // Scroll down
            HudPause_PlayMotion(m_scenePauseSelect, "Scroll_Anim", false, m_cursorPos * 10.0f, cursorPos * 10.0f);
        }
        else if (cursorPos == m_cursorPos - 1)
        {
            // Scroll up
            HudPause_PlayMotion(m_scenePauseSelect, "Scroll_Anim", false, 120.0f - m_cursorPos * 10.0f, 120.0f - cursorPos * 10.0f);
        }
        else
        {
            HudPause_StopMotion(m_scenePauseSelect, "Scroll_Anim", cursorPos * 10.0f);
        }
        break;
    }
    case 4: // Confirm
    {
        break;
    }
    case 5: // Back from confirm dialog
    {
        originalScene->SetHideFlag(true);
        HudPause_OpenPauseWindow();
        break;
    }
    default: break;
    }

    m_cursorPos = cursorPos;
    return result;
}

HOOK(void, __fastcall, HudPause_CPauseCStateCloseBegin, 0x42A710, hh::fnd::CStateMachineBase::CStateBase* This)
{
    int* context = (int*)This->m_pContext;
    if (context[58] != -1)
    {
        // Unpause
        HudPause_ClosePauseScreen();
    }
    else
    {
        // HOWTO/WINDOW
        m_scenePauseWindow->SetHideFlag(true);
        m_scenePauseSelect->SetHideFlag(true);
    }

    originalHudPause_CPauseCStateCloseBegin(This);
}

HOOK(int, __fastcall, HudPause_CPauseCStateIdleBegin, 0x42A340, hh::fnd::CStateMachineBase::CStateBase* This)
{
    HudPause_ClosePauseScreen();
    return originalHudPause_CPauseCStateIdleBegin(This);
}

void HudPause::Install()
{
    WRITE_MEMORY(0x16A41A4, void*, HudPause_CPauseRemoveCallback);
    INSTALL_HOOK(HudPause_CPauseVisualActInit);
    INSTALL_HOOK(HudPause_CPauseVisualActOpened);
    INSTALL_HOOK(HudPause_CPauseVisualActCase);
    INSTALL_HOOK(HudPause_CPauseCStateCloseBegin);
    INSTALL_HOOK(HudPause_CPauseCStateIdleBegin);

    // Put HowTo layout to HUD_Pause
    WRITE_MEMORY(0x10AFC56, uint32_t, 0x1E66C18);
}
