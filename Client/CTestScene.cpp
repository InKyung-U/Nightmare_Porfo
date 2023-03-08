#include "pch.h"
#include "CTestScene.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

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

#include <Engine/CSceneFile.h>

#include <Script/CPlayerScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CMissileScript.h>
#include <Script/CSkeleton_Archer.h>
#include <Script/CWall.h>
#include <Script/CGravity.h>
#include <Script/CSkeleton_Archer.h>
#include <Script/CPlayerUIScript.h>
#include <Script/CSceneSaveLoad.h>
#include <Script/CSceneScript_01.h>
#include <Script/CSceneScript_02.h>
#include <Script/CSceneScript_03.h>

void CTestScene::CreateTestScene()
{
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\Test.scene", L"scene\\Test.scene");
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\Test_01.scene", L"scene\\Test_01.scene");

	CScene* pCurScene = new CScene;
	static UINT pStageNum =-1;
	//pStageNum = CSceneMgr::GetInst()->GetCurSceneNumber();

	if (pStageNum == -1)
	{
		CGameObject* num1Scene = new CGameObject;
		num1Scene->AddComponent(new CSceneScript_01);
		CScene* newScene = new CScene;
		newScene = num1Scene->GetScript<CSceneScript_01>()->GetScene();
		delete num1Scene;
		//CSceneMgr::GetInst()->ChangeScene(pCurScene);

		CSceneMgr::GetInst()->ChangeScene(newScene);

		CCollisionMgr::GetInst()->CollisionCheck(L"PlayerAttack", L"Monster");
		CCollisionMgr::GetInst()->CollisionCheck(L"MonsterAttack", L"Player");
		CCollisionMgr::GetInst()->CollisionCheck(L"ReflectArrow", L"Wall");
		CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Wall");
		CCollisionMgr::GetInst()->CollisionCheck(L"ReflectArrow", L"Monster");
		CCollisionMgr::GetInst()->CollisionCheck(L"MonsterAttack", L"Guard");
		CCollisionMgr::GetInst()->CollisionCheck(L"Monster", L"Wall");


		return;
	}

	if (pStageNum == 0)
	{
		wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
		strTileFolder = strTileFolder + L"scene\\stage01.scene";
		CScene* pLoadScene = CSceneSaveLoad::LoadScene(strTileFolder);

		CPrefab* prefab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Skeleton_Archer_Re.pref", L"prefab\\Skeleton_Archer_Re.pref", false).Get();
		CGameObject* monster = prefab->Instantiate();
		monster->Transform()->SetRelativePos(Vec3(-400, -250, 0));
		pLoadScene->AddObject(monster, L"Monster");
		//CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

		monster = prefab->Instantiate();
		monster->Transform()->SetRelativePos(Vec3(0, -250, 0));
		pLoadScene->AddObject(monster, L"Monster");
		//CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

		//CPrefab* playerpref = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Player.pref", L"prefab\\Player.pref", false).Get();
		//CGameObject* player = playerpref->Instantiate();
		//player->Transform()->SetRelativePos(Vec3(-400, -250, 0));
		//pLoadScene->AddObject(player, L"Player");

		pCurScene = pLoadScene;
	}
	if (pStageNum == 1)
	{
		CGameObject* num1Scene = new CGameObject;
		num1Scene->AddComponent(new CSceneScript_02);
		CScene* newScene = new CScene;
		newScene = num1Scene->GetScript<CSceneScript_02>()->GetScene();
		delete num1Scene;
		//CSceneMgr::GetInst()->ChangeScene(pCurScene);

		CSceneMgr::GetInst()->ChangeScene(newScene);

		CCollisionMgr::GetInst()->CollisionCheck(L"PlayerAttack", L"Monster");
		CCollisionMgr::GetInst()->CollisionCheck(L"MonsterAttack", L"Player");
		CCollisionMgr::GetInst()->CollisionCheck(L"ReflectArrow", L"Wall");
		CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Wall");
		CCollisionMgr::GetInst()->CollisionCheck(L"ReflectArrow", L"Monster");
		CCollisionMgr::GetInst()->CollisionCheck(L"MonsterAttack", L"Guard");
		CCollisionMgr::GetInst()->CollisionCheck(L"Monster", L"Wall");


		return;
	}
	if (pStageNum == 2)
	{
		wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
		strTileFolder = strTileFolder + L"scene\\stage04.scene";
		CScene* pLoadScene = CSceneSaveLoad::LoadScene(strTileFolder);

		CPrefab* prefab1 = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\StoneEye.pref", L"prefab\\Skeleton_Lizard.pref", false).Get();
		CPrefab* prefab2 = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Wolf.pref", L"prefab\\Wolf.pref", false).Get();
		CPrefab* prefab3 = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\WoodGolem.pref", L"prefab\\WoodGolem.pref", false).Get();

		CGameObject* monster = prefab2->Instantiate();
		monster->Transform()->SetRelativePos(Vec3(400, 25, 0));
		pLoadScene->AddObject(monster, L"Monster");
		CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

		monster = prefab1->Instantiate();
		monster->Transform()->SetRelativePos(Vec3(1100, 25, 0));
		pLoadScene->AddObject(monster, L"Monster");
		CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

		monster = prefab3->Instantiate();
		monster->Transform()->SetRelativePos(Vec3(300, 350, 0));
		pLoadScene->AddObject(monster, L"Monster"); monster = prefab1->Instantiate();
		CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

		monster = prefab3->Instantiate();
		monster->Transform()->SetRelativePos(Vec3(800, 350, 0));
		pLoadScene->AddObject(monster, L"Monster");
		CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();

		pCurScene = pLoadScene;
	}
	if (pStageNum == 3)
	{
		wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
		strTileFolder = strTileFolder + L"scene\\Boss_Stage.scene";
		CScene* pLoadScene = CSceneSaveLoad::LoadScene(strTileFolder);
		CPrefab* prefab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\BossWolf.pref", L"prefab\\BossWolf.pref", false).Get();

		CGameObject* monster = prefab->Instantiate();
		monster->Transform()->SetRelativePos(Vec3(0, -310, 0));
		pLoadScene->AddObject(monster, L"Monster");
		//CSceneMgr::GetInst()->GetCurScene()->AddMonsterCount();


		pCurScene = pLoadScene;
	}


	CSceneMgr::GetInst()->ChangeScene(pCurScene);
	pCurScene->SetLayerName(0, L"Tile");
	pCurScene->SetLayerName(1, L"Default");
	pCurScene->SetLayerName(2, L"Player");
	pCurScene->SetLayerName(3, L"Monster");
	pCurScene->SetLayerName(4, L"PlayerAttack");
	pCurScene->SetLayerName(5, L"MonsterAttack");
	pCurScene->SetLayerName(6, L"Effect");
	pCurScene->SetLayerName(7, L"Wall");
	pCurScene->SetLayerName(8, L"ReflectArrow");
	pCurScene->SetLayerName(9, L"Guard");


	// Texture 한장 로딩해보기
	//CResMgr::GetInst()->Load<CTexture>(L"texture\\Player.bmp", L"texture\\Player.bmp");
	//CResMgr::GetInst()->Load<CTexture>(L"texture\\MagicCircle.png", L"texture\\MagicCircle.png");

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
	pCurScene->AddObject(pCamObj, L"Default");

	// UI 카메라
	CGameObject* pUICamObj = new CGameObject;
	pUICamObj->SetName(L"UICamera");
	pUICamObj->AddComponent(new CTransform);
	pUICamObj->AddComponent(new CCamera);
	pUICamObj->Transform()->SetRelativePos(0, 1800, 0);
	pUICamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICamObj->Camera()->SetCameraAsMain();
	pUICamObj->Camera()->CheckLayerMaskAll();

	pCurScene->AddObject(pUICamObj, L"Default");

	// 광원 오브젝트 추가
	CGameObject* pLight2D = new CGameObject;
	pLight2D->SetName(L"DirLight");

	pLight2D->AddComponent(new CTransform);
	pLight2D->AddComponent(new CMeshRender);
	pLight2D->AddComponent(new CLight2D);

	pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	pLight2D->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight2D->Light2D()->SetDiffuse(Vec3(0.8f, 0.8f, 0.8f));

	pCurScene->AddObject(pLight2D, L"Default");


	//pLight2D = new CGameObject;
	//pLight2D->SetName(L"PointLight");
	//
	//pLight2D->AddComponent(new CTransform);
	//pLight2D->AddComponent(new CMeshRender);
	//pLight2D->AddComponent(new CLight2D);
	//
	//pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	//
	//pLight2D->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pLight2D->Light2D()->SetRange(500.f);
	//pLight2D->Light2D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));	
	//
	//pCurScene->AddObject(pLight2D, L"Default");


	// Background Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Background");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 500.f);
	pObject->Transform()->SetRelativeScale(1600.f, 1600.f, 1.f);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PaperBurnMtrl.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\back.png", L"texture\\back.png"));
	pCurScene->AddObject(pObject, L"Default");

	//// Prefab 만들기
	//if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Background.pref"))
	//{
	//	CPrefab* pPrefab = new CPrefab;
	//	pPrefab->SetProto(pObject->Clone());
	//	CResMgr::GetInst()->AddRes<CPrefab>(L"prefab\\Background.pref", pPrefab);
	//}

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

	pCurScene->AddObject(pPlayerObject, L"Player");

	// wall
	CGameObject* pWall = new CGameObject;
	pWall->SetName(L"Wall");
	pWall->AddComponent(new CTransform);
	pWall->AddComponent(new CMeshRender);
	pWall->AddComponent(new CCollider2D);
	pWall->AddComponent(new CWall);
	pWall->Transform()->SetRelativePos(200.f, 0.f, 200.f);
	pWall->Transform()->SetRelativeScale(Vec3(200.f, 600.f, 1.f));
	pWall->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pWall->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DAlphaBlendMtrl.mtrl"));
	pWall->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pWall->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pWall->Collider2D()->SetOffsetScale(Vec2(200.f, 600.f));
	pCurScene->AddObject(pWall, L"Wall");

	CGameObject* pWall2 = pWall->Clone();
	pWall2->AddComponent(new CWall);
	pWall2->Transform()->SetRelativePos(-200.f, -200.f, 200.f);
	pWall2->Transform()->SetRelativeScale(Vec3(800.f, 100.f, 1.f));
	pWall2->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pWall2->Collider2D()->SetOffsetScale(Vec2(800.f, 100.f));
	pCurScene->AddObject(pWall2, L"Wall");

	//CGameObject* pWall3 = pWall->Clone();
	//pWall3->AddComponent(new CWall);
	//pWall3->Transform()->SetRelativePos(-200.f, 100.f, 200.f);
	//pWall3->Transform()->SetRelativeScale(Vec3(800.f, 100.f, 1.f));
	//pWall3->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	//pWall3->Collider2D()->SetOffsetScale(Vec2(800.f, 100.f));
	//pCurScene->AddObject(pWall3, L"Wall");

	// 프리펩 사용
	//CPrefab* prefab = CResMgr::GetInst()->Load<CPrefab>(L"prefab\\Skeleton_Archer.pref", L"prefab\\Skeleton_Archer.pref", false).Get();
	//pCurScene->AddObject(prefab->Instantiate() , L"Monster");
	//CSceneMgr::GetInst()->SpawnObject(pMonster1, vMonsterPos, L"Monster1", 1);

	//CPrefab* prefab = CResMgr::GetInst()->Load<CPrefab>(prefabName, prefabName, false).Get();
	//CSceneMgr::GetInst()->SpawnObject(prefab->Instantiate(), 1);

	// Monster Object
	CGameObject* pMonsterObject = new CGameObject;
	pMonsterObject->SetName(L"TestMonster");
	pMonsterObject->AddComponent(new CTransform);
	pMonsterObject->AddComponent(new CMeshRender);
	pMonsterObject->AddComponent(new CCollider2D);
	pMonsterObject->AddComponent(new CAnimator2D);
	pMonsterObject->AddComponent(new CGravity);
	//pMonsterObject->AddComponent(new CMonsterScript);
	//pMonsterObject->AddComponent(new CSkeleton_Archer);
	pMonsterObject->Transform()->SetRelativePos(-100.f, 0.f, 200.f);
	pMonsterObject->Transform()->SetRelativeScale(Vec3(800.f, 800.f, 1.f));
	pMonsterObject->SetLayerIndex(3);
	pMonsterObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pMonsterObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pMonsterObject->Collider2D()->SetOffsetPos(Vec2(0.f, -10.f));
	pMonsterObject->Collider2D()->SetOffsetScale(Vec2(200.f, 120.f));
	pCurScene->AddObject(pMonsterObject, L"Monster");


	// Tile Map 만들기
	CGameObject* pTileMapObject = new CGameObject;

	pTileMapObject->SetName(L"TileMap");
	pTileMapObject->AddComponent(new CTransform);
	pTileMapObject->AddComponent(new CTileMap);
	pTileMapObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pTileMapObject->Transform()->SetRelativeScale(1000.f, 1000.f, 1.f);

	Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\Tileset_1.png", L"texture\\Tileset_1.png");
	pTileMapObject->TileMap()->SetAtlasTex(pTileAtlas);
	pTileMapObject->TileMap()->SetSlicePixel(Vec2(32, 32));
	UINT x = 10;
	UINT y = 10;
	pTileMapObject->Transform()->SetRelativeScale(2000, 1000, 1.f);
	pTileMapObject->TileMap()->SetTileSize(Vec2(32.f, 32.f));
	pTileMapObject->TileMap()->SetTileMapCount(x, y);

	for (int i = 0; i < x * y; ++i)
	{
		pTileMapObject->TileMap()->SetTileData(i, 92);
	}
	pCurScene->AddObject(pTileMapObject, L"Tile");


	// PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcessObject");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPostProcess->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	pCurScene->AddObject(pPostProcess, L"Default");

	pPostProcess->Deactivate();


	//((CCameraMoveScript*)pCamObj->GetScript(0))->SetFilter(pPostProcess);
	//CCameraMoveScript* pCamMoveScript = pCamObj->GetScript<CCameraMoveScript>();


	// 충돌 레이어 설정
	//CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");	
	CCollisionMgr::GetInst()->CollisionCheck(L"PlayerAttack", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"MonsterAttack", L"Player");
	CCollisionMgr::GetInst()->CollisionCheck(L"ReflectArrow", L"Wall");
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Wall");
	CCollisionMgr::GetInst()->CollisionCheck(L"ReflectArrow", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"MonsterAttack", L"Guard");
	CCollisionMgr::GetInst()->CollisionCheck(L"Monster", L"Wall");


	//pCurScene->SetResKey(L"scene\\TestScene.scene");
	//wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();	
	//CSceneSaveLoad::SaveScene(pCurScene, strSceneFilePath + L"scene\\TestScene.scene");

	//strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
	//CSceneSaveLoad::LoadScene(strSceneFilePath + L"scene\\stage03.scene");
	//CSceneSaveLoad::LoadScene(strSceneFilePath + L"scene\\stage01.scene");

	pCurScene->start();
	pCurScene->SetSceneState(SCENE_STATE::PLAY);

}