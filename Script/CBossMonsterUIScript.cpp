#include "pch.h"
#include "CBossMonsterUIScript.h"
#include "CMonsterScript.h"
#include "CBoss_Wolf.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

CBossMonsterUIScript::CBossMonsterUIScript()
	: CScript((int)SCRIPT_TYPE::PLAYERUISCRIPT)
	, m_vStartPos()
	, m_bStartFunc(false)
{
}

CBossMonsterUIScript::~CBossMonsterUIScript()
{
}

void CBossMonsterUIScript::start()
{
	vector<CGameObject*> vec = CSceneMgr::GetInst()->GetCurScene()->GetLayer(3)->GetRootObjects();

	if (vec.size() == 0)
		return;

	m_pBoss = vec[0];

	if (m_pBoss == nullptr)
		return;

	m_iFullHP = m_pBoss->GetScript<CBoss_Wolf>()->GetBossMonsterHP();

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	Vec3 scale = GetOwner()->Transform()->GetRelativeScale();
	m_vStartPos = Vec2(pos.x - scale.x / 2, pos.y);


	m_fOriginHpWidth = scale.x;
	m_bStartFunc = true;
}

void CBossMonsterUIScript::update()
{
	if (!m_bStartFunc)
	{
		start();
		return;
	}

	if (m_pBoss->GetScript<CBoss_Wolf>() == nullptr)
	{
		GetOwner()->Destroy();
		return;
	}

	Vec3 newPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 newScale = GetOwner()->Transform()->GetRelativeScale();

	if (GetOwner()->GetName() == L"BossHP_Bar")
	{// 현재 피통이 꽉 안차있으면 사이즈 변경
		float curHp = (float)(m_pBoss->GetScript<CMonsterScript>()->GetMonsterHP());
		if (curHp < m_iFullHP)
		{
			float newHpwidth = m_fOriginHpWidth * (curHp / (float)m_iFullHP);
			if (0 > m_vStartPos.x)
			{
				newPos.x = m_vStartPos.x + newHpwidth / 2;
			}
			newScale.x = newHpwidth;
		}
		m_pBoss->GetScript<CBoss_Wolf>()->SetBossMonsterHP(curHp);
	}	
	GetOwner()->Transform()->SetRelativePos(newPos);
	GetOwner()->Transform()->SetRelativeScale(newScale);
}
