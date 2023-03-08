#include "pch.h"

#include "CNotMoveScript.h"

CNotMoveScript::CNotMoveScript()
	: CScript((int)SCRIPT_TYPE::NOTMOVESCRIPT)
{
}

CNotMoveScript::~CNotMoveScript()
{
}

void CNotMoveScript::update()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetMonsterCount() == 0)
	{
		GetOwner()->Destroy();
	}
}

void CNotMoveScript::OnCollisionEnter(CGameObject* _pOtherObj)
{
}

void CNotMoveScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
