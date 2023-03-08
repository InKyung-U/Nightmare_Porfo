#include "pch.h"
#include "CSceneScript_Start.h"
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

#include "CGravity.h"
#include "CCameraMoveScript.h"
CSceneScript_Start::CSceneScript_Start()
	: CScript((int)SCRIPT_TYPE::SCENESCRIPT_START)
{
	m_pScene01 = new CScene;
	CSceneMgr::GetInst()->ChangeSceneNumber(0);
	m_pScene01->SetLayerName(0, L"Tile");
	m_pScene01->SetLayerName(1, L"Default");
	m_pScene01->SetLayerName(2, L"Player");
	m_pScene01->SetLayerName(3, L"Monster");
	m_pScene01->SetLayerName(4, L"PlayerAttack");
	m_pScene01->SetLayerName(5, L"MonsterAttack");
	m_pScene01->SetLayerName(6, L"Effect");
	m_pScene01->SetLayerName(7, L"Wall");
	m_pScene01->SetLayerName(8, L"ReflectArrow");
	m_pScene01->SetLayerName(9, L"Guard");

	

	// Player Object
	CGameObject* pPlayerObject = new CGameObject;
	pPlayerObject->SetName(L"Player");
	pPlayerObject->AddComponent(new CTransform);
	pPlayerObject->AddComponent(new CMeshRender);
	pPlayerObject->AddComponent(new CCollider2D);
	pPlayerObject->AddComponent(new CAnimator2D);
	pPlayerObject->AddComponent(new CGravity);
	//pPlayerObject->AddComponent(new CPlayerScript);

	pPlayerObject->Transform()->SetRelativePos(-600.f, 0.f, 200.f);
	pPlayerObject->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 1.f));

	pPlayerObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPlayerObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

	pPlayerObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pPlayerObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pPlayerObject->Collider2D()->SetOffsetScale(Vec2(40.f, 60.f));

	m_pScene01->AddObject(pPlayerObject, L"Player");

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
	m_pScene01->AddObject(pCamObj, L"Default");

	// UI 카메라
	CGameObject* pUICamObj = new CGameObject;
	pUICamObj->SetName(L"UICamera");
	pUICamObj->AddComponent(new CTransform);
	pUICamObj->AddComponent(new CCamera);
	pUICamObj->Transform()->SetRelativePos(0, 1800, 0);
	pUICamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICamObj->Camera()->SetCameraAsMain();
	pUICamObj->Camera()->CheckLayerMaskAll();

	m_pScene01->AddObject(pUICamObj, L"Default");
}

CSceneScript_Start::~CSceneScript_Start()
{
}

void CSceneScript_Start::update()
{
}
