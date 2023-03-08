#include "pch.h"
#include "CSkeleton_Lizard.h"

#include "CPlayer_Weapon_Collider.h"
#include "CPlayerArrow.h"
#include "CWall.h"
#include "CMonsterUIScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>


CSkeleton_Lizard::CSkeleton_Lizard()
	: CMonsterScript((int)SCRIPT_TYPE::SKELETON_LIZARD)
	, m_fSpeed(100.f)
	, m_iMonsterHP(140)
	, m_fTraceRange(200.f)
	, m_fAttackRange(50.f)
	, m_fAttackCoolTime(4.f)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_bStartFunc(false)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_fAttackTime(0.f)
	, m_bDeadAnimationOn(false)
	, m_bStunOn(false)
{
}
CSkeleton_Lizard::CSkeleton_Lizard(const CSkeleton_Lizard& _Origin)
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


CSkeleton_Lizard::~CSkeleton_Lizard()
{
}

void CSkeleton_Lizard::start()
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
	GetOwner()->Animator2D()->LoadAnimFromFile(L"SkeletonLizard_Attack_0", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"SkeletonLizard_Die", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"SkeletonLizard_Idle", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"SkeletonLizard_Run", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"SkeletonLizard_Stun", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"SkeletonLizard_Hit", backgroundSize);


	m_bStartFunc = true;

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

void CSkeleton_Lizard::update()
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
		if (GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->IsFinish())
		{
			//GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Destroy();
		}
		return;
	}

	if (m_pMonsterState == tMonsterState::MONSTER_HIT)
	{
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->Animator2D()->Play(L"SkeletonLizard_Run", true);
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
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Stun", false);
			m_bStunOn = true;
		}
		if (GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Run", true);
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			m_bStunOn = false;
		}
		return;
	}

	if (m_pMonsterState == tMonsterState::MONSTER_ATTACK)
	{
		GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Attack_0", false);
		m_fAttackTime += DT;
		static bool sound2 = false;
		if (!sound2)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_SkeletonLizard_Attack_0.wav", L"sound\\SE_Enemy_SkeletonLizard_Attack_0.wav");
			m_pMonsterSound->Play(1, 0.3f, true);
			sound2 = true;
		}
		if (0.8f < m_fAttackTime && 10.f > m_fAttackTime)
		{
			m_fAttackTime = 10.f;
			CMonsterScript::CreateMonsterAttackCollider(0.3f, Vec3(50.f, 0.f, 0.f), Vec3(110.f, 80.f, 1), vRotate, 1.f);
		}
		if (GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(tMonsterState::MONSTER_TRACE);
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Run", true);
			m_fAttackTime = 0.f;
			sound2 = false;
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_FARAWAY_ATTACK)
	{
		// 해골도마뱀은 FarAway 가 딱히 필요 없음
		// 추가적인 예외처리 필요~
		GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Run", true);
		CMonsterScript::SetMonsterState(tMonsterState::MONSTER_TRACE);
		//return;
	}
	if (m_pMonsterState == tMonsterState::MONSTER_NON_TRACE)
	{
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_IDLE)
		{
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Idle", true);
		}
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_LEFT_RUN)
		{
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Run", true);
		}
		if (m_pMonsterIdleState == tMonsterIdleState::MONSTER_RIGHT_RUN)
		{
			GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Run", true);
		}
	}
	if (m_pMonsterState == tMonsterState::MONSTER_TRACE)
	{
	}
	if (m_pMonsterState == tMonsterState::MONSTER_DEAD)
	{
		GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->Play(L"SkeletonLizard_Die", false);
		CreateRewardsParticle(vPos);
		m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_Die.wav", L"sound\\SE_Enemy_Die.wav");
		m_pMonsterSound->Play(1, 0.3f, true);
		m_bDeadAnimationOn = true;
	}
}

void CSkeleton_Lizard::lateupdate()
{
}

void CSkeleton_Lizard::OnCollisionEnter(CGameObject* _OtherObject)
{
	CMonsterScript::OnCollisionEnter(_OtherObject);
	//화살이나 망치나 암튼 무기 오브젝트에 맞으면?
	CScript* weapon = _OtherObject->GetScript<CPlayer_Weapon_Collider>();
	CScript* arrow = _OtherObject->GetScript<CPlayerArrow>();
	CScript* wall = _OtherObject->GetScript<CWall>();

	if (nullptr != weapon)
	{
		GetOwner()->Animator2D()->Play(L"SkeletonLizard_Hit", false);
	}
	else if (nullptr != arrow)
	{
		GetOwner()->Animator2D()->Play(L"SkeletonLizard_Hit", false);
	}
	else if (nullptr != wall)
	{
		ContectWallDir dir = MonsterContectWall(_OtherObject);
	}
}


void CSkeleton_Lizard::SaveToScene(FILE* _pFile)
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

void CSkeleton_Lizard::LoadFromScene(FILE* _pFile)
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
