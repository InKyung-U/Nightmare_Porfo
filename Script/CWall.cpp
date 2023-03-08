#include "pch.h"
#include "CWall.h"

#include "CPlayerArrow.h"
#include "CPlayerScript.h"
#include "CGravity.h"
#include "CMonsterScript.h"
#include "CBoss_Wolf.h"

#include <Engine/CScene.h>
#include <random>

CWall::CWall()
	: CScript((int)SCRIPT_TYPE::WALL)
	, m_bThrough(false)
{
}

CWall::CWall(bool _through)
	: CScript((int)SCRIPT_TYPE::WALL)
	, m_bThrough(_through)
{
}

CWall::~CWall()
{
}

void CWall::update()
{

}

void CWall::OnCollisionEnter(CGameObject* _pOtherObj)
{
	CScript* arrowscript = _pOtherObj->GetScript<CPlayerArrow>();
	_pOtherObj->GetName();
	if (arrowscript != nullptr)
	{

	}	

	//CScript* playercript = _pOtherObj->GetScript<CPlayerScript>();
	CScript* pGravity = _pOtherObj->GetScript<CGravity>();
	// 플레이어랑 비교했을때 위에서 만나야 온그라운드
	if (pGravity != nullptr)
	{
		CScript* pplayer = _pOtherObj->GetScript<CPlayerScript>();
		if (pplayer != nullptr)
		{
			ContectWallDir dir = _pOtherObj->GetScript<CPlayerScript>()->ContectWall(GetOwner());
 			if (dir == ContectWallDir::DOWN)
  				_pOtherObj->GetScript<CGravity>()->OnGround();
			else if (!m_bThrough)
			{
				if (dir == ContectWallDir::UP)
					_pOtherObj->GetScript<CGravity>()->GetVelocity().y = -200.f;
			}
			else if (m_bThrough)
			{
				if (dir == ContectWallDir::UP)
				{

				}	// just move
			}

		}
		else
			_pOtherObj->GetScript<CGravity>()->OnGround();
	}


	// 몬스터랑 벽이랑 만났을 때
	
	CScript* monstercript = _pOtherObj->GetScript<CMonsterScript>();
	CScript* BossScript = _pOtherObj->GetScript<CBoss_Wolf>();
	if (BossScript != nullptr)
	{
		//ContectWallDir dir = _pOtherObj->GetScript<CMonsterScript>()->MonsterContectWall(GetOwner());
		//Vec3 wolfPos = _pOtherObj->GetScript<CMonsterScript>()->Transform()->GetRelativePos();
		//if (dir == ContectWallDir::RIGHT)
		//{
		//	wolfPos.x += DT * 600.f;
		//	_pOtherObj->GetScript<CBoss_Wolf>()->Transform()->SetRelativePos(wolfPos);
		//	_pOtherObj->GetScript<CGravity>()->GetVelocity().x = -600;
		//	//DBG_PRINTEXCEPTION_C
		//}
		//else if (dir == ContectWallDir::LEFT)
		//{
		//	wolfPos.x -= DT * 600.f;
		//	_pOtherObj->GetScript<CBoss_Wolf>()->Transform()->SetRelativePos(wolfPos);
		//	_pOtherObj->GetScript<CGravity>()->GetVelocity().x = +100;
		//}
	}
	else if (monstercript != nullptr)
	{
		ContectWallDir dir = _pOtherObj->GetScript<CMonsterScript>()->MonsterContectWall(GetOwner());
		if (dir == ContectWallDir::DOWN)
			_pOtherObj->GetScript<CGravity>()->OnGround();

		if (dir == ContectWallDir::RIGHT)
			_pOtherObj->GetScript<CGravity>()->GetVelocity().x = -100;
		else if (dir == ContectWallDir::LEFT)
			_pOtherObj->GetScript<CGravity>()->GetVelocity().x = +100;
			
	}
	
 }

void CWall::OnCollisionExit(CGameObject* _OtherObject)
{
	CScript* pGravity = _OtherObject->GetScript<CGravity>();
	// 플레이어랑 비교했을때 위에서 만나야 온그라운드
	if (pGravity != nullptr)
	{
		CScript* pplayer = _OtherObject->GetScript<CPlayerScript>();
		if (pplayer != nullptr)
		{
			ContectWallDir dir = _OtherObject->GetScript<CPlayerScript>()->ContectWall(GetOwner());
			if (dir == ContectWallDir::DOWN || dir == ContectWallDir::UP)
			{
				_OtherObject->GetScript<CGravity>()->OnAir();
			}
		}

		CScript* pMonster = _OtherObject->GetScript<CMonsterScript>();
		if (pMonster != nullptr)
		{
			ContectWallDir dir = _OtherObject->GetScript<CMonsterScript>()->MonsterContectWall(GetOwner());
			if (dir == ContectWallDir::DOWN)
			{
				_OtherObject->GetScript<CGravity>()->OnAir();
			}
		}
	}


	CScript* monstercript = _OtherObject->GetScript<CMonsterScript>();
	if (monstercript != nullptr)
	{
		ContectWallDir dir = _OtherObject->GetScript<CMonsterScript>()->MonsterContectWall(GetOwner());
		if (dir == ContectWallDir::DOWN)
			_OtherObject->GetScript<CGravity>()->OnGround();

		if (dir == ContectWallDir::RIGHT)
		{
			_OtherObject->GetScript<CGravity>()->GetVelocity().x = 0;
			_OtherObject->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_NON_TRACE);
		}
		if (dir == ContectWallDir::LEFT)
		{
			_OtherObject->GetScript<CGravity>()->GetVelocity().x = 0;
			_OtherObject->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_NON_TRACE);
		}
	}
}

