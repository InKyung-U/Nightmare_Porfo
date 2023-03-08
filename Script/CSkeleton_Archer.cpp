#include "pch.h"
#include "CSkeleton_Archer.h"

#include "CPlayer_Weapon_Collider.h"
#include "CPlayerArrow.h"
#include "CWall.h"
#include "CMonsterUIScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CScript.h>
#include <Engine/CCollider2D.h>


CSkeleton_Archer::CSkeleton_Archer()
	: CMonsterScript((int)SCRIPT_TYPE::SKELETON_ARCHER)
	, m_fSpeed(100.f)
	, m_iMonsterHP(175)
	, m_fTraceRange(200.f)
	, m_fAttackRange(80.f)
	, m_fAttackCoolTime(6.f)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_bStartFunc(false)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_fAttackTime(0.f)
	, m_bDeadAnimationOn(false)
	, m_bStunOn(false)
{
	
}

CSkeleton_Archer::CSkeleton_Archer(const CSkeleton_Archer& _Origin)
	: CMonsterScript(_Origin)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_iMonsterHP(_Origin.m_iMonsterHP)
	, m_fTraceRange(_Origin.m_fTraceRange)
	, m_fAttackRange(_Origin.m_fAttackRange)
	, m_fAttackCoolTime(_Origin.m_fAttackCoolTime)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_bStartFunc(false)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_fAttackTime(0.f)
	, m_bDeadAnimationOn(false)
	, m_bStunOn(false)
{
}

CSkeleton_Archer::~CSkeleton_Archer()
{
	
}

void CSkeleton_Archer::start()
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

	CGameObject* teat = GetOwner();

	if (teat->Animator2D() == nullptr)
		return;

	teat->Animator2D()->LoadAnimFromFile(L"SkeletonArcher_Attack_0", backgroundSize);
	teat->Animator2D()->LoadAnimFromFile(L"SkeletonArcher_Attack_1", backgroundSize);
	teat->Animator2D()->LoadAnimFromFile(L"SkeletonArcher_Die", backgroundSize);
	teat->Animator2D()->LoadAnimFromFile(L"SkeletonArcher_Idle", backgroundSize);
	teat->Animator2D()->LoadAnimFromFile(L"SkeletonArcher_Run", backgroundSize);
	teat->Animator2D()->LoadAnimFromFile(L"SkeletonArcher_Stun", backgroundSize);
	teat->Animator2D()->LoadAnimFromFile(L"SkeletonArcher_Hit", backgroundSize);
	

	teat->Animator2D()->Play(L"SkeletonArcher_Idle", true);
	m_bStartFunc = true;


	CGameObject* pMonsterHPBar = new CGameObject;
	pMonsterHPBar->SetName(L"MonsterHP");
	pMonsterHPBar->AddComponent(new CTransform);
	pMonsterHPBar->AddComponent(new CMeshRender);
	pMonsterHPBar->AddComponent(new CMonsterUIScript);
	pMonsterHPBar->SetLayerIndex(1);
	pMonsterHPBar->Transform()->SetIgnoreParentScale(false);
	pMonsterHPBar->Transform()->SetRelativePos(0.f, 0.1f, 100.f);
	pMonsterHPBar->Transform()->SetRelativeScale(0.083f, 0.007f, 1.f);
	pMonsterHPBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterHPBar->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BossHPUI.mtrl"));
	pMonsterHPBar->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\UI_EnemyHp_Bar.png", L"texture\\ui\\UI_EnemyHp_Bar.png"));
	GetOwner()->AddChild(pMonsterHPBar);

	// HP Frame
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"MonsterHP_Frame");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->SetLayerIndex(1);
	pObject->Transform()->SetIgnoreParentScale(false);
	pObject->Transform()->SetRelativePos(0.f, 0.1f, 100.f);
	pObject->Transform()->SetRelativeScale(0.1f, 0.010f, 1.f);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MonsterHPFrameUI.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\UI_EnemyHp_Frame.png", L"texture\\ui\\UI_EnemyHp_Frame.png"));
	GetOwner()->AddChild(pObject);
}

void CSkeleton_Archer::update()
{
	if (!m_bStartFunc)
	{
		start();
	}
	if (m_bStartFunc == false)
		return;
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
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Run", true);
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
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Stun", false);
			m_bStunOn = true;
		}
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Run", true);
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			m_bStunOn = false;
		}
		return;
	}

	if (m_pMonsterState == tMonsterState::MONSTER_ATTACK)
	{
		m_fAttackTime += DT;
		if (0.6f < m_fAttackTime && 10.f > m_fAttackTime)
		{
			m_fAttackTime = 10.f;
			CMonsterScript::CreateMonsterAttackCollider(0.2f, Vec3(50.f, 0.f, 0.f), Vec3(40.f, 60.f, 1), vRotate, 1.f);
		}
		GetOwner()->Animator2D()->Play(L"SkeletonArcher_Attack_0", false);
		static bool sound1 = false;
		if (!sound1)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_SkeletonArcher_Attack_0.wav", L"sound\\SE_Enemy_SkeletonArcher_Attack_0.wav");
			m_pMonsterSound->Play(1, 0.3f);
			sound1 = true;
		}
		if (GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Run", true);
			m_fAttackTime = 0;
			sound1 = false;
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_FARAWAY_ATTACK)
	{
		m_fAttackTime += DT;
		static bool sound2 = false;
		if (!sound2)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_SkeletonArcher_Attack_1.wav", L"sound\\SE_Enemy_SkeletonArcher_Attack_1.wav");
			m_pMonsterSound->Play(1, 0.3f);
			sound2 = true;
		}
		if (0.4f < m_fAttackTime && 10.f > m_fAttackTime)
		{
			m_fAttackTime = 10.f;
			CMonsterScript::CreateArrow(Vec3(20.f, 0.f, 0.f));
		}
		GetOwner()->Animator2D()->Play(L"SkeletonArcher_Attack_1", false);
		if (GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Run", true);
			m_fAttackTime = 0.f;
			sound2 = false;
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_NON_TRACE)
	{
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_IDLE)
		{
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Idle", true);
		}
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_LEFT_RUN)
		{
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Run", true);
		}
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_RIGHT_RUN)
		{
			GetOwner()->Animator2D()->Play(L"SkeletonArcher_Run", true);
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_TRACE)
	{
	}
	if (m_pMonsterState == tMonsterState::MONSTER_DEAD)
	{
		m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_Die.wav", L"sound\\SE_Enemy_Die.wav");
		m_pMonsterSound->Play(1, 0.5f, true);
		GetOwner()->Animator2D()->Play(L"SkeletonArcher_Die", false);
		CreateRewardsParticle(vPos);
		m_bDeadAnimationOn = true;
	}


}

void CSkeleton_Archer::lateupdate()
{
}

void CSkeleton_Archer::OnCollisionEnter(CGameObject* _OtherObject)
{
	CMonsterScript::OnCollisionEnter(_OtherObject);
	//화살이나 망치나 암튼 무기 오브젝트에 맞으면?
	CScript* weapon = _OtherObject->GetScript<CPlayer_Weapon_Collider>();
	CScript* arrow = _OtherObject->GetScript<CPlayerArrow>();
	CScript* wall = _OtherObject->GetScript<CWall>();

	if (nullptr != weapon)
	{
		GetOwner()->Animator2D()->Play(L"SkeletonArcher_Hit", false);
	}
	else if (nullptr != arrow)
	{
		GetOwner()->Animator2D()->Play(L"SkeletonArcher_Hit", false);
	}
	else if (nullptr != wall)
	{
		ContectWallDir dir = MonsterContectWall(_OtherObject);
	}
}

void CSkeleton_Archer::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);

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

void CSkeleton_Archer::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);

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
