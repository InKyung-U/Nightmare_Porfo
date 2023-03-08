#include "pch.h"
#include "MenuUI.h"

#include <Engine/CCore.h>
#include <Engine/CPathMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Script/CScriptMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CEventMgr.h>
#include <Script/CSceneSaveLoad.h>
#include <Engine/CScript.h>

#include "CImGuiMgr.h"
#include "SceneOutliner.h"
#include "TileMapUI.h"
#include "AnimationUI.h"

MenuUI::MenuUI()
	: UI("Menu")
    , m_bPackaging(false)
    , m_bSceneSave(false)
    , m_bSceneLoad(false)
    , m_bScenePlayPause(false)
    , m_bSceneStop(false)
    , m_sScriptName()
    , m_bScript(false)
    , m_bTileMapUI(false)
    , m_bAnimationUI(false)
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::update()
{
    Task();
}

void MenuUI::render()
{    
    if (ImGui::BeginMainMenuBar())
    {
        render_update();       

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{
    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("Packaging", NULL, &m_bPackaging);
       
        
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Scene"))
    {
        ImGui::MenuItem("Scene Save", NULL, &m_bSceneSave);
        ImGui::MenuItem("Scene Load", NULL, &m_bSceneLoad);

        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if(SCENE_STATE::PLAY == eState)
            m_strPlayPause = "Pause";
        else
            m_strPlayPause = "Play";

        ImGui::MenuItem(m_strPlayPause.c_str(), NULL, &m_bScenePlayPause);

        if (SCENE_STATE::STOP == eState)
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, false);
        else
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, true);

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Component"))
    {
        if (ImGui::BeginMenu("Add Component"))
        {
			ImGui::MenuItem("MeshRender", NULL);
			ImGui::MenuItem("Camera", NULL);
			ImGui::MenuItem("Collider2D", NULL);
			ImGui::MenuItem("Collider3D", NULL);
			ImGui::MenuItem("Animator2D", NULL);
			ImGui::MenuItem("Animator3D", NULL);

            if (ImGui::BeginMenu("Add Script"))
            {
                vector<wstring> vecScriptName;
                CScriptMgr::GetScriptInfo(vecScriptName);
                for (size_t i = 0; i < vecScriptName.size(); ++i)
                {
                    string strScriptName = string(vecScriptName[i].begin(), vecScriptName[i].end());
                    if (ImGui::MenuItem(strScriptName.c_str(), NULL, &m_bScript))
                    {
                        m_sScriptName = strScriptName;
                    }
                }
                ImGui::EndMenu();
            }

			ImGui::EndMenu();
		}

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools"))
    {
        ImGui::MenuItem("TileMap UI", NULL, &m_bTileMapUI);
        ImGui::MenuItem("Animation UI", NULL, &m_bAnimationUI);

        ImGui::EndMenu();
    }


        
}



void MenuUI::Task()
{
    if (m_bSceneSave)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"scene";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetSaveFileName(&ofn))
        {            
            CSceneSaveLoad::SaveScene(CSceneMgr::GetInst()->GetCurScene(), szName);            
        }

        m_bSceneSave = false;
    }

    else if (m_bSceneLoad)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"tile";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetOpenFileName(&ofn))
        {
            CScene* pLoadScene = CSceneSaveLoad::LoadScene(szName);
            CSceneMgr::GetInst()->ChangeScene(pLoadScene);
        }

        // SceneOutliner 갱신
        SceneOutliner* pUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");
        pUI->Reset();

        m_bSceneLoad = false;
    }

    if (m_bScenePlayPause)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PLAY);            
        }
        else if (SCENE_STATE::PLAY == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PAUSE);
        }

        m_bScenePlayPause = false;
    }

    if (m_bSceneStop)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP != eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::STOP);
            CSceneFile* pSceneFile = pCurScene->GetSceneFile().Get();

            wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pSceneFile->GetRelativePath();
            CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
            CSceneMgr::GetInst()->ChangeScene(pNewScene);
        }

        // SceneOutliner 갱신
        ((SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner"))->Reset();

        m_bSceneStop = false;
    }

    if (m_bScript)
    {
        string test = m_sScriptName;
        wstring wScriptName;
        wScriptName.assign(test.begin(), test.end());
        TreeNode* pSelecteNode = CImGuiMgr::GetInst()->GetSelectedNode();
        // 선택된 노드가 없으면 return
        if (nullptr == pSelecteNode)
        {
            m_bScript = false;
            return;
        }
        wstring pNodeName;
        pNodeName.assign(pSelecteNode->GetName().begin(), pSelecteNode->GetName().end());
        // 선택된 object에 스크립트 추가
        
        CSceneMgr::GetInst()->FindObjectByName(pNodeName)->AddComponent((CComponent*)(CScriptMgr::GetScript(wScriptName)));
        CImGuiMgr::GetInst()->SetScriptAdd(true);
        //CImGuiMgr::GetInst()->SetSelectedNode(nullptr);
        m_bScript = false;
    }

    if (m_bTileMapUI)
    {
        TileMapUI* pTileMapUI = (TileMapUI*)CImGuiMgr::GetInst()->FindUI("##TileMapUI");
        //pTileMapUI->Clear();
        //pTileMapUI->SetTitle("Tile Select");
        if (nullptr == pTileMapUI)
            return;
        pTileMapUI->Activate();
        m_bTileMapUI = false;
    }

    if (m_bAnimationUI)
    {
        AnimationUI* pAnimationUI = (AnimationUI*)CImGuiMgr::GetInst()->FindUI("##AnimationUI");
        //pAnimationUI->Clear();
        //pAnimationUI->SetTitle("Tile Select");
        if (nullptr == pAnimationUI)
            return;
        pAnimationUI->Activate();
        m_bAnimationUI = false;
    }
}
