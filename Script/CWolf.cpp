#include "pch.h"
#include "CWolf.h"

#include "CPlayer_Weapon_Collider.h"
#include "CPlayerArrow.h"
#include "CWall.h"
#include "CMonsterUIScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>

CWolf::CWolf()
	: CMonsterScript((int)SCRIPT_TYPE::WOLF)
	, m_fSpeed(100.f)
	, m_iMonsterHP(175)
	, m_fTraceRange(400.f)
	, m_fAttackRange(50.f)
	, m_fAttackCoolTime(5.f)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_bStartFunc(false)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_fAttackTime(0.f)
	, m_bDeadAnimationOn(false)
	, m_bStunOn(false)
{

}

CWolf::CWolf(const CWolf& _origin)
	: CMonsterScript(_origin)
	, m_fSpeed(_origin.m_fSpeed)
	, m_iMonsterHP(_origin.m_iMonsterHP)
	, m_fTraceRange(_origin.m_fTraceRange)
	, m_fAttackRange(_origin.m_fAttackRange)
	, m_fAttackCoolTime(_origin.m_fAttackCoolTime)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_bStartFunc(false)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_fAttackTime(0.f)
	, m_bDeadAnimationOn(false)
	, m_bStunOn(false)
{
}

CWolf::~CWolf()
{
}

void CWolf::start()
{
	CMonsterScript::start();

	if (m_bStartFunc)
		return;
	GetOwner()->GetScript<CMonsterScript>()->SetMonsterHP(m_iMonsterHP);
	GetOwner()->GetScript<CMonsterScript>()->SetMonsterSpeed(m_fSpeed);
	GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_NON_TRACE);
	GetOwner()->GetScript<CMonsterScript>()->SetTraceRange(m_fTraceRange);
	GetOwner()->GetScript<CMonsterScript>()->SetAttackRange(m_fAttackRange);
	GetOwner()->GetScript<CMonsterScript>()->SetAttackCoolTime(m_fAttackCoolTime);


	Vec2 backgroundSize = Vec2(300, 300);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Wolf_Attack_0", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Wolf_Attack_1", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Wolf_Die", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Wolf_Idle", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Wolf_Run", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Wolf_Stun", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Wolf_Hit", backgroundSize);

	m_bStartFunc = true;
	GetOwner()->Animator2D()->Play(L"Wolf_Idle", true);

	CGameObject* pMonsterHPBar = new CGameObject;
	pMonsterHPBar->SetName(L"MonsterHP");
	pMonsterHPBar->AddComponent(new CTransform);
	pMonsterHPBar->AddComponent(new CMeshRender);
	pMonsterHPBar->AddComponent(new CMonsterUIScript);
	pMonsterHPBar->SetLayerIndex(1);
	pMonsterHPBar->Transform()->SetIgnoreParentScale(false);
	pMonsterHPBar->Transform()->SetRelativePos(0.f, 0.1f, 100.f);
	pMonsterHPBar->Transform()->SetRelativeScale(0.09f, 0.007f, 1.f);
	pMonsterHPBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterHPBar->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BossHPUI.mtrl"));
	pMonsterHPBar->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\UI_EnemyHp_Bar.png", L"texture\\ui\\UI_EnemyHp_Bar.png"));
	GetOwner()->AddChild(pMonsterHPBar);

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"MonsterHP");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CMonsterUIScript);
	pObject->SetLayerIndex(1);
	pObject->Transform()->SetIgnoreParentScale(false);
	pObject->Transform()->SetRelativePos(0.f, 0.1f, 100.f);
	pObject->Transform()->SetRelativeScale(0.1f, 0.010f, 1.f);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MonsterHPFrameUI.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\UI_EnemyHp_Frame.png", L"texture\\ui\\UI_EnemyHp_Frame.png"));
	GetOwner()->AddChild(pObject);
}

void CWolf::update()
{
	if (!m_bStartFunc)
	{
		start();
		m_bStartFunc = true;
	}
	CMonsterScript::update();
	// 요기 밑부터 구현

	if (m_bDeadAnimationOn)
	{
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			//GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->Destroy();
		}
		return;
	}

	if (m_pMonsterState == tMonsterState::MONSTER_HIT)
	{
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->Animator2D()->Play(L"Wolf_Run", true);
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
		}
	}

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRotate = Transform()->GetRelativeRotation();

	m_pMonsterState = GetOwner()->GetScript<CMonsterScript>()->GetCurMonsterState();
	m_pMonsterIdleState = GetOwner()->GetScript<CMonsterScript>()->GetMonsterIdleState();

	if (m_pMonsterState == tMonsterState::MONSTER_STUN)
	{
		if (m_bStunOn == false)
		{
			GetOwner()->Animator2D()->Play(L"Wolf_Stun", false);
			m_bStunOn = true;
		}
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->Play(L"Wolf_Run", true);
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			m_bStunOn = false;
		}
		return;
	}

	if (m_pMonsterState == tMonsterState::MONSTER_ATTACK)
	{
		static bool soundon1 = false;
		if (!soundon1)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_Wolf_Attack_1.wav", L"sound\\SE_Enemy_Wolf_Attack_1.wav");
			m_pMonsterSound->Play(1, 0.3f, true);
			soundon1 = true;
		}
		GetOwner()->Animator2D()->Play(L"Wolf_Attack_0", false);
		m_fAttackTime += DT;
		if (0.8f < m_fAttackTime && 10.f > m_fAttackTime)
		{
			m_fAttackTime = 10.f;
			CMonsterScript::CreateMonsterAttackCollider(0.3f, Vec3(50.f, 0.f, 0.f), Vec3(40.f, 60.f, 1), vRotate, 1.f);
		}
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			GetOwner()->Animator2D()->Play(L"Wolf_Run", true);
			m_fAttackTime = 0.f;
			soundon1 = false;
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_FARAWAY_ATTACK)
	{
		static bool soundon2 = false;
		if (!soundon2)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_Wolf_Attack_0.wav", L"sound\\SE_Enemy_Wolf_Attack_0.wav");
			m_pMonsterSound->Play(1, 0.3f, true);
			soundon2 = true;
		}
		GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"Wolf_Attack_1", false);
		m_fAttackTime += DT;
		if (0.9f < m_fAttackTime && 10.f > m_fAttackTime)
		{
			m_fAttackTime = 10.f;
			CMonsterScript::CreateMonsterAttackCollider(0.2f, Vec3(50.f, 0.f, 0.f), Vec3(40.f, 60.f, 1), vRotate, 1.f);
		}
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			GetOwner()->Animator2D()->Play(L"Wolf_Run", true);
			m_fAttackTime = 0.f;
			soundon2 = false;
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_NON_TRACE)
	{
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_IDLE)
		{
			GetOwner()->Animator2D()->Play(L"Wolf_Idle", true);
		}
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_LEFT_RUN)
		{
			GetOwner()->Animator2D()->Play(L"Wolf_Run", true);
		}
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_RIGHT_RUN)
		{
			GetOwner()->Animator2D()->Play(L"Wolf_Run", true);
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_TRACE)
	{
	}
	if (m_pMonsterState == tMonsterState::MONSTER_DEAD)
	{
		m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_Die.wav", L"sound\\SE_Enemy_Die.wav");
		m_pMonsterSound->Play(1, 0.3f, true);
		GetOwner()->Animator2D()->Play(L"Wolf_Die", false);
		CreateRewardsParticle(vPos);
		m_bDeadAnimationOn = true;
	}


}

void CWolf::lateupdate()
{
}

void CWolf::OnCollisionEnter(CGameObject* _OtherObject)
{
	CMonsterScript::OnCollisionEnter(_OtherObject);
	//화살이나 망치나 암튼 무기 오브젝트에 맞으면?
	CScript* weapon = _OtherObject->GetScript<CPlayer_Weapon_Collider>();
	CScript* arrow = _OtherObject->GetScript<CPlayerArrow>();
	CScript* wall = _OtherObject->GetScript<CWall>();

	if (nullptr != weapon)
	{
		GetOwner()->Animator2D()->Play(L"Wolf_Hit", false);
	}
	else if (nullptr != arrow)
	{
		GetOwner()->Animator2D()->Play(L"Wolf_Hit", false);
	}
	else if (nullptr != wall)
	{
		ContectWallDir dir = MonsterContectWall(_OtherObject);
	}
}

void CWolf::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fTraceRange, sizeof(float), 1, _pFile);
	fwrite(&m_fAttackRange, sizeof(float), 1, _pFile);
	fwrite(&m_fAttackCoolTime, sizeof(float), 1, _pFile);
	fwrite(&m_iMonsterHP, sizeof(UINT), 1, _pFile);
	fwrite(&m_pMonsterState, sizeof(tMonsterState), 1, _pFile);
	fwrite(&m_pMonsterIdleState, sizeof(tMonsterIdleState), 1, _pFile);
	fwrite(&m_bStartFunc, sizeof(bool), 1, _pFile);
	fwrite(&m_bDeadAnimationOn, sizeof(bool), 1, _pFile);
	fwrite(&m_fAttackTime, sizeof(float), 1, _pFile);
	fwrite(&m_bStunOn, sizeof(bool), 1, _pFile);
}

void CWolf::LoadFromScene(FILE* _pFile)
{
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_fTraceRange, sizeof(float), 1, _pFile);
	fread(&m_fAttackRange, sizeof(float), 1, _pFile);
	fread(&m_fAttackCoolTime, sizeof(float), 1, _pFile);
	fread(&m_iMonsterHP, sizeof(UINT), 1, _pFile);
	fread(&m_pMonsterState, sizeof(tMonsterState), 1, _pFile);
	fread(&m_pMonsterIdleState, sizeof(tMonsterIdleState), 1, _pFile);
	fread(&m_bStartFunc, sizeof(bool), 1, _pFile);
	fread(&m_bDeadAnimationOn, sizeof(bool), 1, _pFile);
	fread(&m_fAttackTime, sizeof(float), 1, _pFile);
	fread(&m_bStunOn, sizeof(bool), 1, _pFile);
}
