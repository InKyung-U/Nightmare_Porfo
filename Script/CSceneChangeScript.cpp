#include "pch.h"

#include "CSceneChangeScript.h"

CSceneChangeScript::CSceneChangeScript()
	: CScript((int)SCRIPT_TYPE::SCENECHANGESCRIPT)
{
}

CSceneChangeScript::~CSceneChangeScript()
{
}

void CSceneChangeScript::update()
{
}

void CSceneChangeScript::OnCollisionEnter(CGameObject* _pOtherObj)
{
	if (_pOtherObj->GetName() == L"Player")
	{
		int NextSceneNum = CSceneMgr::GetInst()->GetCurSceneNumber() + 1;
		CSceneMgr::GetInst()->ChangeSceneNumber(NextSceneNum);


	}
}
