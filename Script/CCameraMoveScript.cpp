#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CScript.h>
#include <Engine/CLayer.h>

#include "CPlayerScript.h"


CCameraMoveScript::CCameraMoveScript()
	: CScript((int)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_Filter(nullptr)
	, m_fCamSpeed(1000.f)	
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	float fSpeed = m_fCamSpeed;
	vector<CGameObject*> playerVec = CSceneMgr::GetInst()->GetCurScene()->GetLayer(2)->GetRootObjects();

	// 타일 받아오기
	vector<CGameObject*> TileVec = CSceneMgr::GetInst()->GetCurScene()->GetLayer(0)->GetRootObjects();
	CGameObject* Tile1 = nullptr;
	CGameObject* Tile2 = nullptr;

	if (TileVec.size() != 0)
		Tile1 = TileVec[0];
	if (TileVec.size() == 2)
		Tile2 = TileVec[1];

	CGameObject* player = nullptr;
	Vec3 playerPos;
	if (playerVec.size() != 0)
	{
		if (nullptr != playerVec[0]->GetScript<CPlayerScript>())
		{
			player = playerVec[0];
			playerPos = player->Transform()->GetRelativePos();
		}
	}


	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType()) // 원근체크
	{
		if (KEY_PRESSED(KEY::W))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			vPos += DT * vFront * fSpeed;
		}
	
		if (KEY_PRESSED(KEY::S))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			vPos -= DT * vFront * fSpeed;
		}
	
		if (KEY_PRESSED(KEY::A))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			vPos -= DT * vRight * fSpeed;
		}
	
		if (KEY_PRESSED(KEY::D))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			vPos += DT * vRight * fSpeed;
		}
	}

	else // 노 원근
	{
		if (player == nullptr)
		{
			if (KEY_PRESSED(KEY::W))
			{
				vPos.y += DT * fSpeed;
			}

			if (KEY_PRESSED(KEY::S))
			{
				vPos.y -= DT * fSpeed;
			}

			if (KEY_PRESSED(KEY::A))
			{
				vPos.x -= DT * fSpeed;
			}

			if (KEY_PRESSED(KEY::D))
			{
				vPos.x += DT * fSpeed;
			}
		}
		else
		{
			// 타일 존재시 타일넓이로도 보정필요
			if (nullptr != Tile1)
			{
				Vec3 Tile1Pos = Tile1->Transform()->GetRelativePos();
				Vec3 Tile1Scale = Tile1->Transform()->GetRelativeScale();
				Vec3 cameraPos = GetOwner()->Transform()->GetRelativePos();
				float cameraWidth = GetOwner()->Camera()->GetWidth();
				float AspectRatio = GetOwner()->Camera()->GetAspectRatio();
				float cameraHeight = cameraWidth / AspectRatio;
				float Tile1L = Tile1Pos.x - Tile1Scale.x / 2;
				float Tile1R = Tile1Pos.x + Tile1Scale.x / 2;
				float Tile1U = Tile1Pos.y + Tile1Scale.y / 2;
				float Tile1D = Tile1Pos.y - Tile1Scale.y / 2;
				float camerL = cameraPos.x - cameraWidth / 2;
				float camerR = cameraPos.x + cameraWidth / 2;
				float cameraU = Tile1Pos.x - Tile1Scale.y / 2;
				float cameraD = Tile1Pos.x + Tile1Scale.y / 2;

				//case1. 타일 1개
				if (Tile2 == nullptr)
				{
					// 카메라가 양 옆으로 넘어간 상황
					//vPos = playerPos;
					if (cameraWidth/2 >= playerPos.x - Tile1L)
					{						
						vPos.x = Tile1L + cameraWidth / 2;
					}
					else if (cameraWidth/2 >= Tile1R - playerPos.x)
					{						
						vPos.x = Tile1R - cameraWidth / 2;
					}
					else
					{
						vPos.x = playerPos.x;
					}
					
					if (cameraHeight/2 > playerPos.y - Tile1D + 300)
					{
						vPos.y = Tile1D + cameraHeight / 2 + 300;
					}
					else if(cameraHeight/2 > Tile1U - playerPos.y)
					{
						vPos.y = Tile1U - cameraHeight / 2;
					}
					else
					{
						vPos.y = playerPos.y;
					}



				}
				else // case2. 타일 2개
				{
					Vec3 Tile2Pos = Tile2->Transform()->GetRelativePos();
					Vec3 Tile2Scale = Tile2->Transform()->GetRelativeScale();
					float Tile2L = Tile2Pos.x - Tile2Scale.x / 2;
					float Tile2R = Tile2Pos.x + Tile2Scale.x / 2;
					float Tile2D = Tile2Pos.y - Tile2Scale.y / 2;
					float Tile2U = Tile2Pos.y + Tile2Scale.y / 2;

					if (Tile1Pos.x < Tile2Pos.x) // 타일1 - 타일2
					{
						// 카메라가 양 옆으로 넘어간 상황
						if (cameraWidth / 2 >= playerPos.x - Tile1L)
						{
							vPos.x = Tile1L + cameraWidth / 2;
						}
						else if (cameraWidth / 2 >= Tile2R - playerPos.x)
						{
							vPos.x = Tile2R - cameraWidth / 2;
						}
						else
						{
							vPos.x = playerPos.x;
						}

						if (cameraHeight / 2 > playerPos.y - Tile1D + 300)
						{
							vPos.y = Tile1D + cameraHeight / 2;
						}
						else if (cameraHeight / 2 > Tile1U - playerPos.y + 300)
						{
							vPos.y = Tile1U - cameraHeight / 2;
						}
						else
						{
							vPos.y = playerPos.y;
						}
					}
					else // 타일2 - 타일1
					{
						// 카메라가 양 옆으로 넘어간 상황
						if (cameraWidth / 2 >= playerPos.x - Tile2L)
						{
							vPos.x = Tile2L + cameraWidth / 2;
						}
						else if (cameraWidth / 2 >= Tile1R - playerPos.x)
						{
							vPos.x = Tile1R - cameraWidth / 2;
						}
						else
						{
							vPos.x = playerPos.x;
						}

						if (cameraHeight / 2 > playerPos.y - Tile1D + 300)
						{
							vPos.y = Tile1D + cameraHeight / 2 + 300;
						}
						else if (cameraHeight / 2 > Tile1U - playerPos.y)
						{
							vPos.y = Tile1U - cameraHeight / 2;
						}
						else
						{
							vPos.y = playerPos.y;
						}
					}
				}
			}
			//else
			//	vPos = playerPos;			
		}

	}

	//if (nullptr != m_Filter && KEY_TAP(KEY::F))
	//{
	//	if (m_Filter->IsActive())
	//		m_Filter->Deactivate();
	//	else
	//		m_Filter->Activate();
	//}
	

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * XM_PI;
		vRot.x -= DT * vMouseDir.y * XM_PI;
	
		Transform()->SetRelativeRotation(vRot);
	}

	Transform()->SetRelativePos(vPos);
}
