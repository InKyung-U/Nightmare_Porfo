#include "pch.h"
#include "CGuardScript.h"
#include "CMonster_Attack_Collider.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CScript.h>

CGuardScript::CGuardScript()
	: CScript((int)SCRIPT_TYPE::GUARDSCRIPT)
	, m_pPlayer(nullptr)
{
}

CGuardScript::CGuardScript(CGameObject* _playerobj)
	: CScript((int)SCRIPT_TYPE::GUARDSCRIPT)
	, m_pPlayer(_playerobj)
{
	m_pPlayer->GetScript<CPlayerScript>()->SetGuardOn(true);
}

CGuardScript::~CGuardScript()
{
	m_pPlayer->GetScript<CPlayerScript>()->SetGuardOn(false);
}

void CGuardScript::update()
{
	
}

void CGuardScript::OnCollisionEnter(CGameObject* _pOtherObj)
{
	CScript* pScript = _pOtherObj->GetScript<CMonster_Attack_Collider>();
	if (nullptr == pScript)
		return;

	CGameObject* pMonsterCollider = ((CMonster_Attack_Collider*)pScript)->GetOwnerMonster();
	pMonsterCollider->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_STUN);

	_pOtherObj->Destroy();
}
