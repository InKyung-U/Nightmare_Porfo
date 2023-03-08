#include "pch.h"
#include "CPlayer_Weapon_Collider.h"

#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CGameObject.h>

CPlayer_Weapon_Collider::CPlayer_Weapon_Collider()
	:CScript((int)SCRIPT_TYPE::PLAYER_WEAPON_COLLIDER)
	, m_fLiveTime(0.f)
	, m_pCurWeapon(tCurWeaponType::BOW)
	, m_bHammerSpAttack(false)
	, m_bSwordOverAttack(false)
	, m_iAttack(0)
{
}

CPlayer_Weapon_Collider::CPlayer_Weapon_Collider(float _liveTime, tCurWeaponType _type, int _attack)
	:CScript((int)SCRIPT_TYPE::PLAYER_WEAPON_COLLIDER)
	, m_fLiveTime(_liveTime)
	, m_pCurWeapon(_type)
	, m_bHammerSpAttack(false)
	, m_bSwordOverAttack(false)
	, m_iAttack(_attack)
{	
}

CPlayer_Weapon_Collider::CPlayer_Weapon_Collider(float _liveTime, tCurWeaponType _type, bool _swordEon, bool _hammerQ, int _attack)
	:CScript((int)SCRIPT_TYPE::PLAYER_WEAPON_COLLIDER)
	, m_fLiveTime(_liveTime)
	, m_pCurWeapon(_type)
	, m_bHammerSpAttack(_hammerQ)
	, m_bSwordOverAttack(_swordEon)
	, m_iAttack(_attack)
{
}

CPlayer_Weapon_Collider::~CPlayer_Weapon_Collider()
{
}

void CPlayer_Weapon_Collider::start()
{
}

void CPlayer_Weapon_Collider::update()
{
	m_fLiveTime -= DT;


	if (m_fLiveTime <= 0)
		GetOwner()->Destroy();
}

void CPlayer_Weapon_Collider::lateupdate()
{
}

void CPlayer_Weapon_Collider::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CPlayer_Weapon_Collider::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fLiveTime, sizeof(float), 1, _pFile);
}

void CPlayer_Weapon_Collider::LoadFromScene(FILE* _pFile)
{
	fread(&m_fLiveTime, sizeof(float), 1, _pFile);
}
