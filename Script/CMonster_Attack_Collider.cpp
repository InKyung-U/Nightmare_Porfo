#include "pch.h"
#include "CMonster_Attack_Collider.h"
#include "CMonsterScript.h"

#include <Engine/CScript.h>

CMonster_Attack_Collider::CMonster_Attack_Collider()
	:CScript((int)SCRIPT_TYPE::PLAYER_WEAPON_COLLIDER)
	, m_fLiveTime(0.f)
	, m_fAttack(0.f)
	, m_pOwnerMonster(nullptr)
{
}

CMonster_Attack_Collider::CMonster_Attack_Collider(float _liveTime, CGameObject* _ownerMonster, float _attack)
	: CScript((int)SCRIPT_TYPE::PLAYER_WEAPON_COLLIDER)
	, m_fLiveTime(_liveTime)
	, m_fAttack(_attack)
	, m_pOwnerMonster(_ownerMonster)
{
}

CMonster_Attack_Collider::~CMonster_Attack_Collider()
{
}


void CMonster_Attack_Collider::start()
{
}

void CMonster_Attack_Collider::update()
{
	m_fLiveTime -= DT;


	if (m_fLiveTime <= 0)
		GetOwner()->Destroy();
}

void CMonster_Attack_Collider::lateupdate()
{
}

void CMonster_Attack_Collider::OnCollisionEnter(CGameObject* _OtherObject)
{
	CScript* pScript = m_pOwnerMonster->GetScript<CMonsterScript>();
	if (_OtherObject->GetLayerIndex() == 9)
	{
		((CMonsterScript*)pScript)->SetMonsterState(tMonsterState::MONSTER_STUN);
		return;
	}
}

void CMonster_Attack_Collider::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fLiveTime, sizeof(float), 1, _pFile);
}

void CMonster_Attack_Collider::LoadFromScene(FILE* _pFile)
{
	fread(&m_fLiveTime, sizeof(float), 1, _pFile);
}
