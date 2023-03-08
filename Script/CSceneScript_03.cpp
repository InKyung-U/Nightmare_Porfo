#include "pch.h"
#include "CSceneScript_03.h"
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

CSceneScript_03::CSceneScript_03()
	: CScript((int)SCRIPT_TYPE::SCENESCRIPT_03)
{
	m_pScene01 = new CScene;
	CSceneMgr::GetInst()->ChangeSceneNumber(3);

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder = strTileFolder + L"scene\\stage04.scene";
	CScene* pLoadScene = CSceneSaveLoad::LoadScene(strTileFolder);

	CSceneMgr::GetInst()->SetReadyScene(pLoadScene);

	tEventInfo info;
	info.eType = EVENT_TYPE::SCENE_CHANGE;
	info.lParam = (DWORD_PTR)pLoadScene;
	CEventMgr::GetInst()->AddEvent(info);

	CPrefab* prefab1 = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\StoneEye_Re.pref", L"prefab\\Skeleton_Lizard_Re.pref", false).Get();
	CPrefab* prefab2 = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Wolf_Re.pref", L"prefab\\Wolf_Re.pref", false).Get();
	CPrefab* prefab3 = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\WoodGolem_Re01.pref", L"prefab\\WoodGolem_Re01.pref", false).Get();

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

	CGameObject* monster = prefab2->Instantiate();
	monster->Transform()->SetRelativePos(Vec3(400, 25, 0));
	pLoadScene->AddObject(monster, L"Monster");
	CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

	monster = prefab1->Instantiate();
	monster->Transform()->SetRelativePos(Vec3(1100, 25, 0));
	pLoadScene->AddObject(monster, L"Monster");
	CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

	monster = prefab3->Instantiate();
	monster->Transform()->SetRelativePos(Vec3(300, -350, 0));
	pLoadScene->AddObject(monster, L"Monster"); monster = prefab1->Instantiate();
	CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

	monster = prefab3->Instantiate();
	monster->Transform()->SetRelativePos(Vec3(800, -350, 0));
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

	pPlayerObject->Transform()->SetRelativePos(-451.f, -372.f, 200.f);
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

	CGameObject* ChangeScene = new CGameObject;
	ChangeScene->SetName(L"ChangeScene");
	ChangeScene->AddComponent(new CTransform);
	ChangeScene->AddComponent(new CMeshRender);
	ChangeScene->AddComponent(new CCollider2D);
	ChangeScene->AddComponent(new CWall);
	ChangeScene->AddComponent(new CSceneChangeScript);
	ChangeScene->Transform()->SetRelativePos(1500, -300, 0);
	ChangeScene->Transform()->SetRelativeScale(Vec3(100, 300, 1.f));
	ChangeScene->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ChangeScene->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DAlphaBlendMtrl.mtrl"));
	ChangeScene->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	ChangeScene->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	ChangeScene->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	pLoadScene->AddObject(ChangeScene, L"Wall");

	m_pScene01 = pLoadScene;
	m_pScene01->start();
	m_pScene01->SetSceneState(SCENE_STATE::PLAY);
}

CSceneScript_03::~CSceneScript_03()
{
}

void CSceneScript_03::update()
{
}
