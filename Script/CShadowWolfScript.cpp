#include "pch.h"
#include "CShadowWolfScript.h"
#include "CMonsterScript.h"

#include <Engine/CTransform.h>

CShadowWolfScript::CShadowWolfScript()
	: CScript((int)SCRIPT_TYPE::SHADOWWOLFSCRIPT)
{
}

CShadowWolfScript::CShadowWolfScript(Vec3 _startPos, tMonsterRorL _state, float _rotateY)
	: CScript((int)SCRIPT_TYPE::SHADOWWOLFSCRIPT)
	, m_vStartPos(_startPos)
	, m_pRorL(_state)
	, m_fAttackTime(0.f)
	, m_fRotate(_rotateY)
{
}


CShadowWolfScript::~CShadowWolfScript()
{
}

void CShadowWolfScript::update()
{
	m_fAttackTime += DT;
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRotate = Transform()->GetRelativeRotation();

	if (1.5f <= m_fAttackTime && 2.f >= m_fAttackTime)
		if (m_pRorL == tMonsterRorL::MONSTER_RIGHT)
		{
			vPos.x += DT * 1200.f;
		}
		else
		{
			vPos.x -= DT * 1200.f;
		}
	if (m_fRotate == 0.f)
	{
		vRotate.y = 0.f;
	}
	else
	{
		vRotate.y = 3.141592f;
	}


	if (2.5f < m_fAttackTime)
	{
		GetOwner()->Destroy();
	}
	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRotate);
}

void CShadowWolfScript::OnCollisionEnter(CGameObject* _pOtherObj)
{
}

