#include "pch.h"
#include "CEffectScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CGameObject.h>

CEffectScript::CEffectScript()
	:CScript((int)SCRIPT_TYPE::EFFECTSCRIPT)
	, m_sEffectName()
	, m_fLifeTime(-1.f)
{
}

CEffectScript::CEffectScript(wstring _effectName)
	: CScript((int)SCRIPT_TYPE::EFFECTSCRIPT)
	, m_sEffectName(_effectName)
	, m_pScriptObject(nullptr)
	, m_fLifeTime(-1.f)
{
}

CEffectScript::CEffectScript(wstring _effectName, float _lifeTime)
	: CScript((int)SCRIPT_TYPE::EFFECTSCRIPT)
	, m_sEffectName(_effectName)
	, m_pScriptObject(nullptr)
	, m_fLifeTime(_lifeTime)
{
}

CEffectScript::~CEffectScript()
{	
}

void CEffectScript::start()
{
}

void CEffectScript::update()
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CGameObject*> pvector = pScene->GetLayer(L"Effect")->GetRootObjects();

	for (size_t i = 0; i < pvector.size(); i++)
	{
		if (pvector[i]->GetName() == m_sEffectName)
		{
			m_pScriptObject = pvector[i];

			if (m_fLifeTime == -1.f) {

				if (m_pScriptObject->Animator2D()->GetCurAnim()->IsFinish())
				{
					if (nullptr != m_pScriptObject->Animator2D()->GetCurAnim())
						pvector[i]->Destroy();
				}

			}
			else
			{
				m_fLifeTime -= DT;
				if (0 >= m_fLifeTime)
				{
					m_pScriptObject->Destroy();

				}
			}
		}
	}
	

}

void CEffectScript::lateupdate()
{
}

void CEffectScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CEffectScript::SaveToScene(FILE* _pFile)
{
}

void CEffectScript::LoadFromScene(FILE* _pFile)
{
}
