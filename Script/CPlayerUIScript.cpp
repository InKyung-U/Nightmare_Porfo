#include "pch.h"
#include "CPlayerUIScript.h"
#include "CPlayerScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

CPlayerUIScript::CPlayerUIScript()
	: CScript((int)SCRIPT_TYPE::PLAYERUISCRIPT)
	, m_vStartPos()
	, m_bStartFunc(false)
{
}

CPlayerUIScript::~CPlayerUIScript()
{
}

void CPlayerUIScript::start()
{
	vector<CGameObject*> vec = CSceneMgr::GetInst()->GetReadyScene()->GetLayer(2)->GetRootObjects();

	if (vec.size() == 0)
		return;

	m_pPlayer = vec[0];
	m_iFullHP = m_pPlayer->GetScript<CPlayerScript>()->GetFullHP();

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	Vec3 scale = GetOwner()->Transform()->GetRelativeScale();
	if (0 < pos.x)
		m_vStartPos = Vec2(pos.x - scale.x / 2, pos.y);
	else
		m_vStartPos = Vec2(pos.x + scale.x / 2, pos.y);

	m_fOriginHpWidth = scale.x;
	m_bStartFunc = true;
}

void CPlayerUIScript::update()
{
	if (!m_bStartFunc)
	{
		start();
		return;
	}
	if (GetOwner() == nullptr)
	{
		return;
	}
	if (m_pPlayer->GetScript<CPlayerScript>() == nullptr)
	{
		return;
	}

	Vec3 newPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 newScale = GetOwner()->Transform()->GetRelativeScale();

	if (GetOwner()->GetName() == L"PlayerUI_HP")
	{// ���� ������ �� ���������� ������ ����
		float curHp = (float)(m_pPlayer->GetScript<CPlayerScript>()->GetHP());
		if (curHp < m_iFullHP)
		{
			float newHpwidth = m_fOriginHpWidth * (curHp / (float)m_iFullHP);
			if (0 > m_vStartPos.x)
			{
				newPos.x = m_vStartPos.x - newHpwidth / 2;
			}
			newScale.x = newHpwidth;
		}
	}
	else
	{
		//float curHp = (float)(m_pPlayer->GetScript<CPlayerScript>()->GetSP());
	}

	GetOwner()->Transform()->SetRelativePos(newPos);
	GetOwner()->Transform()->SetRelativeScale(newScale);
}
