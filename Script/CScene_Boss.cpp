#include "pch.h"
#include "CScene_Boss.h"
#include <CSceneSaveLoad.h>

#include <Engine/CPrefab.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCamera.h>
#include <Engine/CLight2D.h>
#include <Engine/CTileMap.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CSceneMgr.h>


#include "CGravity.h"
#include "CCameraMoveScript.h"
#include "CPlayerScript.h"
#include "CWall.h"
#include "CSceneChangeScript.h"

CScene_Boss::CScene_Boss()
	: CScript((int)SCRIPT_TYPE::SCENE_BOSS)
{
	CSceneMgr::GetInst()->GetBGM()->Stop();
	Ptr<CSound> bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\BGM_VS_Lupus.wav", L"sound\\BGM_VS_Lupus.wav");
	CSceneMgr::GetInst()->SetBGM(bgm);

	bgm->Play(0, 0.5f);

	m_pScene01 = new CScene;
	CSceneMgr::GetInst()->ChangeSceneNumber(4);

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder = strTileFolder + L"scene\\Boss_Stage.scene";
	CScene* pLoadScene = CSceneSaveLoad::LoadScene(strTileFolder);

	CSceneMgr::GetInst()->SetReadyScene(pLoadScene);

	tEventInfo info;
	info.eType = EVENT_TYPE::SCENE_CHANGE;
	info.lParam = (DWORD_PTR)pLoadScene;
	CEventMgr::GetInst()->AddEvent(info);

	CPrefab* prefab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\BossWolf.pref", L"prefab\\BossWolf.pref", false).Get();

	pLoadScene->SetLayerName(0, L"Tile");
	pLoadScene->SetLayerName(1, L"Default");
	pLoadScene->SetLayerName(2, L"Player");
	pLoadScene->SetLayerName(3, L"Monster");
	pLoadScene->SetLayerName(4, L"PlayerAttack");
	pLoadScene->SetLayerName(5, L"MonsterAttack");
	pLoadScene->SetLayerName(6, L"Effect");
	pLoadScene->SetLayerName(7, L"Wall");
	pLoadScene->SetLayerName(8, L"ReflectArrow");
	pLoadScene->SetLayerName(9, L"Guard");

	CGameObject* monster = prefab->Instantiate();
	monster->Transform()->SetRelativePos(Vec3(0, -310, 0));
	pLoadScene->AddObject(monster, L"Monster");
	CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

	// Player Object
	CGameObject* pPlayerObject = new CGameObject;
	pPlayerObject->SetName(L"Player");
	pPlayerObject->AddComponent(new CTransform);
	pPlayerObject->AddComponent(new CMeshRender);
	pPlayerObject->AddComponent(new CCollider2D);
	pPlayerObject->AddComponent(new CAnimator2D);
	pPlayerObject->AddComponent(new CGravity);
	pPlayerObject->AddComponent(new CPlayerScript);

	pPlayerObject->Transform()->SetRelativePos(-697.f, -371.f, 200.f);
	pPlayerObject->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 1.f));

	pPlayerObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPlayerObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

	pPlayerObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pPlayerObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pPlayerObject->Collider2D()->SetOffsetScale(Vec2(40.f, 60.f));

	pLoadScene->AddObject(pPlayerObject, L"Player");

	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();
	pCamObj->Camera()->SetWidth(1000.f);
	pLoadScene->AddObject(pCamObj, L"Default");

	// UI 카메라
	CGameObject* pUICamObj = new CGameObject;
	pUICamObj->SetName(L"UICamera");
	pUICamObj->AddComponent(new CTransform);
	pUICamObj->AddComponent(new CCamera);
	pUICamObj->Transform()->SetRelativePos(0, 1800, 0);
	pUICamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICamObj->Camera()->SetCameraAsMain();
	pUICamObj->Camera()->CheckLayerMaskAll();

	pLoadScene->AddObject(pUICamObj, L"Default");

	// wall
	//CGameObject* pWall = new CGameObject;
	//pWall->SetName(L"Wall");
	//pWall->AddComponent(new CTransform);
	//pWall->AddComponent(new CMeshRender);
	//pWall->AddComponent(new CCollider2D);
	//pWall->AddComponent(new CWall);
	//pWall->Transform()->SetRelativePos(200.f, 0.f, 200.f);
	//pWall->Transform()->SetRelativeScale(Vec3(2000.f, 0.f, 1.f));
	//pWall->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pWall->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DAlphaBlendMtrl.mtrl"));
	//pWall->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	//pWall->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	//pWall->Collider2D()->SetOffsetScale(Vec2(2000.f, 600.f));
	//pLoadScene->AddObject(pWall, L"Wall");

	m_pScene01 = pLoadScene;
	m_pScene01->start();
	m_pScene01->SetSceneState(SCENE_STATE::PLAY);
}

CScene_Boss::~CScene_Boss()
{
}

void CScene_Boss::update()
{
}
