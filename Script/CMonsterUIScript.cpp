#include "pch.h"
#include "CMonsterUIScript.h"
#include "CMonsterScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

CMonsterUIScript::CMonsterUIScript()
	: CScript((int)SCRIPT_TYPE::MONSTERUISCRIPT)
	, m_vStartPos()
	, m_bStartFunc(false)
{
}

CMonsterUIScript::~CMonsterUIScript()
{
}

void CMonsterUIScript::start()
{
	m_iFullHP = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetMonsterHP();

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	Vec3 scale = GetOwner()->Transform()->GetRelativeScale();
	if (0 < pos.x)
		m_vStartPos = Vec2(pos.x - scale.x / 2, pos.y);

	m_fOriginHpWidth = scale.x;
	m_bStartFunc = true;
}

void CMonsterUIScript::update()
{
	if (!m_bStartFunc)
	{
		start();
		return;
	}
	Vec3 newPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 newScale = GetOwner()->Transform()->GetRelativeScale();

	if (0 >= GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetMonsterHP())
	{
		newScale.y = 0;
		GetOwner()->Transform()->SetRelativeScale(newScale);
		return;
	}


	if (GetOwner()->GetName() == L"MonsterHP")
	{// 현재 피통이 꽉 안차있으면 사이즈 변경
		float curHp = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetMonsterHP();
		if (curHp < m_iFullHP)
		{
			Vec3 rotate = GetOwner()->GetParent()->Transform()->GetRelativeRotation();
			float newHpwidth = m_fOriginHpWidth * (curHp / (float)m_iFullHP);
			if (rotate.y < -1) // rotate true
			{
				newPos.x = -m_fOriginHpWidth/2 + newHpwidth / 2;
				newScale.x = newHpwidth;
			}
			else // no rotate
			{
				newPos.x = -m_fOriginHpWidth / 2 + newHpwidth / 2;
				newScale.x = newHpwidth;
			}
		}
	}

	GetOwner()->Transform()->SetRelativePos(newPos);
	GetOwner()->Transform()->SetRelativeScale(newScale);
}
