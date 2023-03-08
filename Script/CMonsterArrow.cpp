#include "pch.h"
#include "CMonsterArrow.h"

CMonsterArrow::CMonsterArrow()
	: CScript((int)SCRIPT_TYPE::MONSTERARROW)
	, m_pState()
	, m_fSpeed(0.f)
	, m_fLifeTime(4.f)
{
}

CMonsterArrow::CMonsterArrow(tMonsterRorL _RorL)
	: CScript((int)SCRIPT_TYPE::MONSTERARROW)
	, m_pState(_RorL)
	, m_fSpeed(100.f)
	, m_fLifeTime(4.f)
{
}

CMonsterArrow::~CMonsterArrow()
{
}

void CMonsterArrow::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	if (m_pState == tMonsterRorL::MONSTER_LEFT)
	{
		vPos.x -= DT * m_fSpeed;
	}
	else
	{
		vPos.x += DT * m_fSpeed;
	}

	Transform()->SetRelativePos(vPos);

	m_fLifeTime -= DT;
	if (0 >= m_fLifeTime)
		GetOwner()->Destroy();
}

void CMonsterArrow::OnCollisionEnter(CGameObject* _pOtherObj)
{
}
