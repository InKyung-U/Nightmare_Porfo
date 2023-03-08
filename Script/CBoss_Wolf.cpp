#include "pch.h"
#include "CBoss_Wolf.h"
#include "CEffectScript.h"
#include "CShadowWolfScript.h"
#include "CBossMonsterUIScript.h"

#include "CPlayer_Weapon_Collider.h"
#include "CPlayerArrow.h"
#include "CWall.h"
#include "CGravity.h"
#include "CMonsterUIScript.h"
#include <Engine/CCollider2D.h>

#include <random>

CBoss_Wolf::CBoss_Wolf()
	: CMonsterScript((int)SCRIPT_TYPE::BOSS_WOLF)
	, m_fSpeed(50.f)
	, m_iMonsterHP(1000)
	, m_iMonsterHP_Fix(1000)
	, m_fTraceRange(500.f)
	, m_fAttackRange(100.f)
	, m_fAttackCoolTime(9.f)
	, m_pMonsterState(tMonsterState::MONSTER_TRACE)
	, m_bStartFunc(false)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_bDuringAttack(false)
	, m_bBossDisappear(false)
	, m_bBossFlip(false)
	, m_bStunOn(false)
	, m_bDeadAnimationOn(false)
	, m_fAttackTime(0.f)
	, m_bHowlingOn(false)
	, m_bHowlingEndOn(false)
	, m_bBreathOn(false)
	, m_bDashOn(false)
	, m_bShadowDashOn(false)
	, m_vShadowStartPos(Vec3(0.f,0.f,0.f))
	, m_bBreath1(false)
	, m_bBreath2(false)
	, m_pMonsterObject(nullptr)
	, m_iHowlingCount(5)
{
}

CBoss_Wolf::CBoss_Wolf(const CBoss_Wolf& _Origin)
	: CMonsterScript(_Origin)
	, m_fSpeed(50.f)
	, m_iMonsterHP(1000)
	, m_iMonsterHP_Fix(1000)
	, m_fTraceRange(500.f)
	, m_fAttackRange(100.f)
	, m_fAttackCoolTime(9.f)
	, m_pMonsterState(tMonsterState::MONSTER_TRACE)
	, m_bStartFunc(false)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_bDuringAttack(false)
	, m_bBossDisappear(false)
	, m_bBossFlip(false)
	, m_bStunOn(false)
	, m_bDeadAnimationOn(false)
	, m_fAttackTime(0.f)
	, m_bHowlingOn(false)
	, m_bHowlingEndOn(false)
	, m_bBreathOn(false)
	, m_bDashOn(false)
	, m_bShadowDashOn(false)
	, m_vShadowStartPos(Vec3(0.f, 0.f, 0.f))
	, m_bBreath1(false)
	, m_bBreath2(false)
	, m_pMonsterObject(nullptr)
	, m_iHowlingCount(6)
{
}

CBoss_Wolf::~CBoss_Wolf()
{
}

void CBoss_Wolf::start()
{
	CMonsterScript::start();

	m_pMonsterObject = GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject();

	if (m_bStartFunc)
		return;
	GetOwner()->GetScript<CMonsterScript>()->SetMonsterHP(m_iMonsterHP);
	GetOwner()->GetScript<CMonsterScript>()->SetMonsterSpeed(m_fSpeed);
	GetOwner()->GetScript<CMonsterScript>()->SetMonsterState(m_pMonsterState);
	GetOwner()->GetScript<CMonsterScript>()->SetTraceRange(m_fTraceRange);
	GetOwner()->GetScript<CMonsterScript>()->SetAttackRange(m_fAttackRange);
	GetOwner()->GetScript<CMonsterScript>()->SetAttackCoolTime(m_fAttackCoolTime);


	Vec2 backgroundSize = Vec2(600, 600);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Breath", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Breath_End", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Breath_Ready", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Dash", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Dash_Shadow", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Die", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Hit", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Howling", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Howling_End", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Howling_Ready", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Idle", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Move_Appear", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Move_Disappear", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Object_PoisonField", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Object_PoisonField_End", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Rock_01", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Rock_02", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Spawn", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Stomp", backgroundSize);
	GetOwner()->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Stomp_End", backgroundSize);

	GetOwner()->Animator2D()->Play(L"Boss_Wolf_Spawn", false);
	GetOwner()->GetScript<CGravity>()->EnableGravity();
	SetAttackMotion();
	m_bStartFunc = true;

	CResMgr::GetInst()->Load<CMaterial>(L"material\\BossHPUI.mtrl", L"material\\BossHPUI.mtrl");
	CResMgr::GetInst()->Load<CMaterial>(L"material\\BossFrameUI.mtrl", L"material\\BossFrameUI.mtrl");
	
	// HP Bar
	CGameObject* BossHPBar = new CGameObject;
	BossHPBar->SetName(L"BossHP_Bar");
	BossHPBar->AddComponent(new CTransform);
	BossHPBar->AddComponent(new CMeshRender);
	BossHPBar->AddComponent(new CBossMonsterUIScript);
	BossHPBar->Transform()->SetRelativePos(0.f, 2075.f, 100.f);
	BossHPBar->Transform()->SetRelativeScale(960.f, 33.f, 1.f);
	BossHPBar->Transform()->SetIgnoreParentScale(true);
	BossHPBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	BossHPBar->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BossHPUI.mtrl"));
	BossHPBar->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\UI_BossHp_Bar.png", L"texture\\ui\\UI_BossHp_Bar.png"));
	CSceneMgr::GetInst()->GetReadyScene()->AddObject(BossHPBar, L"Default");

	//  HP Frame
	CGameObject* PlayerUI = new CGameObject;
	PlayerUI->SetName(L"BossHP_FrameUI");
	PlayerUI->AddComponent(new CTransform);
	PlayerUI->AddComponent(new CMeshRender);
	PlayerUI->Transform()->SetRelativePos(0.f, 2100.f, 100.f);
	PlayerUI->Transform()->SetRelativeScale(1000.f, 150.f, 1.f);
	PlayerUI->Transform()->SetIgnoreParentScale(true);
	PlayerUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	PlayerUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BossFrameUI.mtrl"));
	PlayerUI->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_BossHp_Frame.png", L"texture\\UI_BossHp_Frame.png"));
	CSceneMgr::GetInst()->GetReadyScene()->AddObject(PlayerUI, L"Default");
}

void CBoss_Wolf::update()
{
	if (nullptr == GetOwner()->Animator2D())
		return;

	if (!m_bStartFunc)
	{
		start();
		m_bStartFunc = true;
	}
	//CMonsterScript::update();
	// 요기 밑부터 구현

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRotate = Transform()->GetRelativeRotation();

	if (m_bDeadAnimationOn)
	{
		static bool soundDeath = false;
		if (!soundDeath)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Die.wav", L"sound\\SE_Boss_Lupus_Die.wav");
			m_pMonsterSound->Play(1, 0.3f, true);
			soundDeath = true;
		}
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			//GetOwner()->Animator2D()->GetCurAnim()->Reset();
			GetOwner()->Destroy();
		}
		return;
	}
	if (0 >= GetOwner()->GetScript<CMonsterScript>()->GetMonsterHP())
	{
		GetOwner()->Animator2D()->Play(L"Boss_Wolf_Die", false);
		m_bDeadAnimationOn = true;
		return;
	}

	// 쿨타임이 증가되어 있다면 무조건 쉰다.
	if (0 < m_fAttackCoolTime) 
	{
		m_fAttackCoolTime -= DT;
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{	
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
			if (m_bBossDisappear)
			{
				vPos.y = 1000; 
				if (m_pMotionState.StartState == MotionState::MOTION_DURING)
				{
					if (m_pMotionState.AttackMotion == BossAttackMotion::A_LANDATTACK1
						|| m_pMotionState.AttackMotion == BossAttackMotion::A_LANDATTACK2
						|| m_pMotionState.AttackMotion == BossAttackMotion::A_LANDATTACK3)
					{
						if (vRotate.y < 1.f)
						{
							vPos.x = -500;
						}
						else
						{
							vPos.x = 500;
						}

					}
					else
					{
						vPos.x = 0;
					}
				}
			}
			
			if (m_bBossFlip)
			{
				m_bBossFlip = false;
				if (vRotate.y < 1.f)
				{
					vRotate.y = 3.141592f;
					m_pMonsterObject->GetScript<CMonsterScript>()->SetMonsterRorL(tMonsterRorL::MONSTER_LEFT);
				}
				else
				{
					vRotate.y = 0.f;
					m_pMonsterObject->GetScript<CMonsterScript>()->SetMonsterRorL(tMonsterRorL::MONSTER_RIGHT);
				}
			}

			if (m_pMotionState.StartState == MotionState::MOTION_END)
			{
				if (m_pMotionState.AttackState == MotionState::MOTION_BEFORE_START)
				{
					m_bBossDisappear = false;
					vPos.y = -310;
				}
			}
			GetOwner()->Animator2D()->Play(L"Boss_Wolf_Idle", false);
		}
		Transform()->SetRelativeRotation(vRotate);
		Transform()->SetRelativePos(vPos);
		return;
	}
	else   // 쿨타임이 끝났다면 조건을 확인한다. 두번째 모션이 끝났는지 시작 전인지.
	{
		// 쿨타임 종료시 플레이중이던 애니메이션 리셋 한번 시켜줌
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Animator2D()->GetCurAnim()->Reset();
		}

		if (m_pMotionState.AttackState == MotionState::MOTION_END)
		{
			// 두번째 모션까지 끝났다면 다시 세팅해준다.
			SetAttackMotion(); 

			// 초기화 할 값들 다 여기서 초기화 해줌. 여기서 안하면.. 또 들어감.ㅋ
			m_bHowlingOn = false;
			m_bBreathOn = false;
			m_bDashOn = false;
			m_bShadowDashOn = false;

			return;
		}
		// 첫번째 모션만 끝났다면 밑의 실행문을 마저 실행한다.
	}

	//======================
	// 공격이 진행되고 있음
	//======================

	if (m_pMotionState.StartState == MotionState::MOTION_BEFORE_START)
	{
		m_pMotionState.StartState = MotionState::MOTION_DURING;
		if (m_pMotionState.StartMotion == BossStartMotion::S_START1)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Stomp_Ready.wav", L"sound\\SE_Boss_Lupus_Stomp_Ready.wav");
			m_pMonsterSound->Play(1, 0.3f, true);
			GetOwner()->Animator2D()->Play(L"Boss_Wolf_Stomp", false);
		}
		else if (m_pMotionState.StartMotion == BossStartMotion::S_START2)
		{
			m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Move_Disappear.wav", L"sound\\SE_Boss_Lupus_Move_Disappear.wav");
			m_pMonsterSound->Play(1, 0.3f, true);
			GetOwner()->Animator2D()->Play(L"Boss_Wolf_Move_Disappear", false);
		}
		m_bBossDisappear = true;
		m_fAttackCoolTime = 5.f;

	}
	else if (m_pMotionState.StartState == MotionState::MOTION_DURING)
	{
		//================
		// 시작 모션 구현
		//================		
		m_pMotionState.StartState = MotionState::MOTION_END;
		return;
		//m_fAttackCoolTime = 2.f;
	}
	else if (m_pMotionState.StartState == MotionState::MOTION_END)
	{
		if (m_pMotionState.AttackState == MotionState::MOTION_BEFORE_START)
		{
			vPos.y = -310.f;
			m_bBossDisappear = false;
			Transform()->SetRelativePos(vPos);

			if (m_pMotionState.AttackMotion == BossAttackMotion::A_AIRATTACK1
				|| m_pMotionState.AttackMotion == BossAttackMotion::A_AIRATTACK2
				|| m_pMotionState.AttackMotion == BossAttackMotion::A_AIRATTACK3)
			{
				m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Stomp.wav", L"sound\\SE_Boss_Lupus_Stomp.wav");
				m_pMonsterSound->Play(1, 0.3f, true);
				CMonsterScript::CreateMonsterAttackCollider(0.4f, Vec3(0.f, 0.f, 0.f), Vec3(800.f, 200.f, 1.f), vRotate, 20.f);
				GetOwner()->Animator2D()->Play(L"Boss_Wolf_Stomp_End", false);
			}
			else
			{
				m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Move_Appear.wav", L"sound\\SE_Boss_Lupus_Move_Appear.wav");
				m_pMonsterSound->Play(1, 0.3f, true);
				m_pMonsterObject->Animator2D()->Play(L"Boss_Wolf_Move_Appear", false);
			}

			m_pMotionState.AttackState = MotionState::MOTION_DURING;
			m_fAttackCoolTime = 1.f;
		}
		else if (m_pMotionState.AttackState == MotionState::MOTION_DURING)
		{
			// 한번 돌아서 특수 기믹 진행중이면 진행하고 return;
			if (m_bHowlingOn)
			{
				m_fAttackTime += DT;
				if (15.f <= m_fAttackTime)
				{
					//m_pMonsterObject->Animator2D()->GetCurAnim()->Reset();
					m_pMonsterObject->Animator2D()->Play(L"Boss_Wolf_Howling_End", false);
					m_fAttackTime = 0.f;
					//m_bHowlingOn = false;
					m_fAttackCoolTime = 2.f;
					m_pMotionState.AttackState = MotionState::MOTION_END;
				}
				else if (1.1f < m_fAttackTime && m_fAttackTime < 15.f)
				{
					static float HowingTime = 0.f;
					HowingTime -= DT;
					if (5 == m_iHowlingCount)
					{
						m_pMonsterObject->Animator2D()->GetCurAnim()->Reset();
						m_pMonsterObject->Animator2D()->Play(L"Boss_Wolf_Howling", true);
						m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Howling.wav", L"sound\\SE_Boss_Lupus_Howling.wav");
						m_pMonsterSound->Play(1, 0.3f, true);
					}
					if (0.f > HowingTime)
					{
						m_iHowlingCount--;
						HowingTime = 0.9f;

						if (m_iHowlingCount == 0)
						{
							m_fAttackTime = 15.f;
							m_iHowlingCount = 5;
							HowingTime = 0.f;
						}
						else
						{
							m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Effect_Boss_Lupus_Poison.wav", L"sound\\SE_Effect_Boss_Lupus_Poison.wav");
							m_pMonsterSound->Play(1, 0.3f, true);
							CreateMonsterEffect(L"Effect_Boss_Wolf_Howling", Vec3(0.f, 0.f, 0.f), Vec3(1000.f, 600.f, 1.f), vRotate.y);
							CreateMonsterAttackCollider(0.8f, Vec3(0.f, 0.f, 0.f), Vec3(1000.f, 200.f, 1.f), vRotate, 5.f);
						}
					}
				}
				
			}
			else if (m_bBreathOn)
			{
				//static bool breath = false;
				//if (!breath)
				//{
				//	m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Breath.wav", L"sound\\SE_BosSE_Boss_Lupus_Breath_Lupus_Howling_Ready.wav");
				//	m_pMonsterSound->Play(1, 0.3f, true);
				//	breath = true;
				//}
				m_fAttackTime += DT;
				m_pMonsterObject;
				if (m_bBreath2)
				{
					if (5.3f < m_fAttackTime)
					{
						GetOwner()->Animator2D()->GetCurAnim()->Reset();
						m_bBreath1 = false;
						m_bBreath2 = false;
						m_fAttackTime = 0.f;
						m_fAttackCoolTime = 2.f;
						m_pMotionState.AttackState = MotionState::MOTION_END;
						return;
					}
				}
				else if (m_bBreath1)
				{
					if (5.1f < m_fAttackTime)
					{
						if (vRotate.y > 1.f)
						{ // 회전시
							GetOwner()->Animator2D()->Play(L"Boss_Wolf_Breath_End", false);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Middle_End", Vec3(-700.f, 0.f, 0.f), Vec3(800.f, 600.f, 1.f), vRotate.y, 0.4f);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Start_End", Vec3(-200.f, 0.f, 0.f), Vec3(600.f, 600.f, 1.f), vRotate.y, 0.4f);

						}
						else
						{
							GetOwner()->Animator2D()->Play(L"Boss_Wolf_Breath_End", false);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Middle_End", Vec3(700.f, 0.f, 0.f), Vec3(800.f, 600.f, 1.f), vRotate.y, 0.4f);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Start_End", Vec3(200.f, 0.f, 0.f), Vec3(600.f, 600.f, 1.f), vRotate.y, 0.4f);

						}
						m_bBreath2 = true;

					}
				}
				else if (m_bBreath1 == false)
				{
					if (0.9f <= m_fAttackTime)
					{
						if (vRotate.y > 1.f)
						{
							CMonsterScript::CreateMonsterAttackCollider(4.5f, Vec3(500.f, 0.f, 0.f), Vec3(1200.f, 500.f, 1.f), vRotate, 10.f);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Start", Vec3(-200.f, -20.f, 0.f), Vec3(600.f, 600.f, 1.f), vRotate.y, 4.5f);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Middle", Vec3(-700.f, -20.f, 0.f), Vec3(800.f, 600.f, 1.f), vRotate.y, 4.5f);
						}
						else
						{
							CMonsterScript::CreateMonsterAttackCollider(4.5f, Vec3(500.f, 0.f, 0.f), Vec3(1200.f, 500.f, 1.f), vRotate, 10.f);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Start", Vec3(200.f, -20.f, 0.f), Vec3(600.f, 600.f, 1.f), vRotate.y, 4.5f);
							CreateBossMonsterEffect(L"Effect_Boss_Wolf_Breath_Middle", Vec3(700.f, -20.f, 0.f), Vec3(800.f, 600.f, 1.f), vRotate.y, 4.5f);
						}
			
						m_pMonsterObject->Animator2D()->GetCurAnim()->Reset();
						m_pMonsterObject->Animator2D()->Play(L"Boss_Wolf_Breath", true);
						m_bBreath1 = true;
					}
				}

			}
			else if (m_bDashOn)
			{
				m_fAttackTime += DT;
				static bool DashSound = false;
				if (!DashSound)
				{
					m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Dash.wav", L"sound\\SE_Boss_Lupus_Dash.wav");
					m_pMonsterSound->Play(1, 0.3f, true);
					DashSound = true;
				}
				if (1.f <= m_fAttackTime && 1.8f >= m_fAttackTime)
				{
					if (m_pMonsterObject->GetScript<CMonsterScript>()->GetMonsterRorL() == tMonsterRorL::MONSTER_RIGHT)
					{
						if (1.f <= m_fAttackTime && 1.f + DT >= m_fAttackTime)
							CMonsterScript::CreateMonsterAttackCollider(0.8f, Vec3(450.f, 0.f, 0.f), Vec3(1000.f, 200.f, 1.f), vRotate, 10.f);

						vPos.x += DT * 1200.f;
					}
					else
					{
						if (1.f <= m_fAttackTime && 1.f + DT >= m_fAttackTime)
							CMonsterScript::CreateMonsterAttackCollider(0.8f, Vec3(450.f, 0.f, 0.f), Vec3(1000.f, 200.f, 1.f), vRotate, 10.f);

						vPos.x -= DT * 1200.f;
					}
				}

				if (2.5f < m_fAttackTime)
				{
					m_pMotionState.AttackState = MotionState::MOTION_END;
					m_fAttackCoolTime = 2.f;
					m_fAttackTime = 0.f;
					DashSound = false;
				}
			}
			else if (m_bShadowDashOn)
			{
				m_fAttackTime += DT;
				m_fAttackTime += DT;
				static bool DashSound2 = false;
				if (!DashSound2)
				{
					m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Dash.wav", L"sound\\SE_Boss_Lupus_Dash.wav");
					m_pMonsterSound->Play(1, 0.3f, true);
					DashSound2 = true;
				}
				if (1.f <= m_fAttackTime && 1.8f >= m_fAttackTime)
					if (m_pMonsterObject->GetScript<CMonsterScript>()->GetMonsterRorL() == tMonsterRorL::MONSTER_RIGHT)
					{
						if (1.f <= m_fAttackTime && 1.f + DT >= m_fAttackTime)
							CMonsterScript::CreateMonsterAttackCollider(0.8f, Vec3(450.f, 0.f, 0.f), Vec3(1000.f, 200.f, 1.f), vRotate, 5.f);
						vPos.x += DT * 1200.f;
					}
					else
					{
						if (1.f <= m_fAttackTime && 1.f + DT >= m_fAttackTime)
							CMonsterScript::CreateMonsterAttackCollider(0.8f, Vec3(450.f, 0.f, 0.f), Vec3(1000.f, 200.f, 1.f), vRotate, 5.f);
						vPos.x -= DT * 1200.f;
					}

				if (2.5f < m_fAttackTime)
				{
					m_pMotionState.AttackState = MotionState::MOTION_END;
					m_fAttackCoolTime = 2.f;
					m_fAttackTime = 0.f;
					DashSound2 = false;
					CreateShadow();
				}

			}

			//=====================
			// 중간 공격 모션 구현
			//=====================
			//GetOwner()->GetScript<CMonsterScript>()->GetMonsterObject()->Animator2D()->GetCurAnim()->Reset();

			if (m_pMotionState.AttackMotion == BossAttackMotion::A_AIRATTACK1)
			{				
				m_pMotionState.AttackState = MotionState::MOTION_END;
				m_fAttackCoolTime = 2.f;
			}
			else if (m_pMotionState.AttackMotion == BossAttackMotion::A_AIRATTACK2)
			{
				m_pMotionState.AttackState = MotionState::MOTION_END;
				m_fAttackCoolTime = 2.f;
			}
			else if (m_pMotionState.AttackMotion == BossAttackMotion::A_AIRATTACK3)
			{
				if (m_bHowlingOn == false)
				{
					m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Howling_Ready.wav", L"sound\\SE_Boss_Lupus_Howling_Ready.wav");
					m_pMonsterSound->Play(1, 0.3f, true);
					GetOwner()->Animator2D()->Play(L"Boss_Wolf_Howling_Ready", false);
					m_bHowlingOn = true;
				}
				//m_pMotionState.AttackState = MotionState::MOTION_END;
			}
			else if (m_pMotionState.AttackMotion == BossAttackMotion::A_LANDATTACK1)
			{
				if (m_bBreathOn == false)
				{
					m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Breath_Ready.wav", L"sound\\SE_Boss_Lupus_Breath_Ready.wav");
					m_pMonsterSound->Play(1, 0.3f, true);
					GetOwner()->Animator2D()->Play(L"Boss_Wolf_Breath_Ready", false);
					if (vRotate.y > 1.f)
						CreateMonsterEffect(L"Effect_Boss_Wolf_Breath_Mouth", Vec3(-200.f, 0.f, 0.f), Vec3(600.f, 600.f, 1.f), vRotate.y);
					else
						CreateMonsterEffect(L"Effect_Boss_Wolf_Breath_Mouth", Vec3(200.f, 0.f, 0.f), Vec3(600.f, 600.f, 1.f), vRotate.y);
					m_bBreathOn = true;
				}
				//m_pMotionState.AttackState = MotionState::MOTION_END;
			}
			else if (m_pMotionState.AttackMotion == BossAttackMotion::A_LANDATTACK2)
			{
				if (m_bDashOn == false)
				{
					m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Dash_Ready.wav", L"sound\\SE_Boss_Lupus_Dash_Ready.wav");
					m_pMonsterSound->Play(1, 0.3f, true);
					GetOwner()->Animator2D()->Play(L"Boss_Wolf_Dash", false);
					m_bDashOn = true;
					m_bBossFlip = true;
				}
			}
			else if (m_pMotionState.AttackMotion == BossAttackMotion::A_LANDATTACK3)
			{
				if (m_bShadowDashOn == false)
				{
					m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Boss_Lupus_Dash_Ready.wav", L"sound\\SE_Boss_Lupus_Dash_Ready.wav");
					m_pMonsterSound->Play(1, 0.3f, true);
					GetOwner()->Animator2D()->Play(L"Boss_Wolf_Dash", false);
					m_vShadowStartPos = vPos;
					m_bShadowDashOn = true;
					m_bBossFlip = true;
				}
			}

		}
	}

	Transform()->SetRelativePos(vPos);
}

void CBoss_Wolf::lateupdate()
{
}

//======================
// Attack State Setting
//======================

void CBoss_Wolf::SetAttackMotion()
{
	m_bDuringAttack = true;		// 모션 세팅을 하면서 어택을 true 로 만들고
	m_fAttackCoolTime = 5.f;	// 쿨 타임도 미리 증가시켜둔다.

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1);
	int startM = dis(gen);

	if (startM == 0)
		m_pStartMotion = BossStartMotion::S_START1;
	if (startM == 1)
		m_pStartMotion = BossStartMotion::S_START2;

	//m_pStartMotion = BossStartMotion::S_START2;

	if (m_pStartMotion == BossStartMotion::S_START1)
	{
		std::random_device rd2;
		std::mt19937 gen2(rd2());
		
		m_pAttackMotion;
		int attackM;
		if (m_iMonsterHP_Fix / 2 > m_iMonsterHP) // 피가 50퍼보다 작으면 독 브레스도 포함
		{
			std::uniform_int_distribution<int> dis(2, 2);
			attackM = 2;
		}
		else
		{
			std::uniform_int_distribution<int> dis(0, 1);
			attackM = dis(gen2);
		}

		if (attackM == 0)
		{
			m_pAttackMotion = BossAttackMotion::A_AIRATTACK1;
		}
		if (attackM == 1)
		{
			m_pAttackMotion = BossAttackMotion::A_AIRATTACK2;
		}
		if (attackM == 2)
		{
			m_pAttackMotion = BossAttackMotion::A_AIRATTACK3;
		}
		//m_pAttackMotion = BossAttackMotion::A_AIRATTACK3;
	}
	else if (m_pStartMotion == BossStartMotion::S_START2)
	{
		std::random_device rd2;
		std::mt19937 gen2(rd2());
		int attackM;
		if (m_iMonsterHP_Fix / 2 > m_iMonsterHP) // 피가 50퍼보다 작으면 쉐도우 돌진도 포함
		{
			std::uniform_int_distribution<int> dis(2, 2);
			attackM = 2;
		}
		else
		{
			std::uniform_int_distribution<int> dis(0, 1);
			attackM = dis(gen2);
		}

		if (attackM == 0)
		{
			m_pAttackMotion = BossAttackMotion::A_LANDATTACK1;
		}
		if (attackM == 1)
		{
			m_pAttackMotion = BossAttackMotion::A_LANDATTACK2;
		}
		if (attackM == 2)
		{
			m_pAttackMotion = BossAttackMotion::A_LANDATTACK3;
		}
		//m_pAttackMotion = BossAttackMotion::A_LANDATTACK3;
	}

	m_pMotionState.AttackMotion = m_pAttackMotion;
	m_pMotionState.AttackState = MotionState::MOTION_BEFORE_START;
	m_pMotionState.StartMotion = m_pStartMotion;
	m_pMotionState.StartState = MotionState::MOTION_BEFORE_START;
}

void CBoss_Wolf::AttackEnd()
{
}

void CBoss_Wolf::CreateBossMonsterEffect(wstring _effectName, Vec3 _offsetPos, Vec3 _scale, float _yRotation, float _time)
{
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	Vec3 vPlayerRotation = Transform()->GetRelativeRotation();
	Vec3 vMonsterSclae = Transform()->GetRelativeScale();

	vPlayerPos.x += _offsetPos.x;
	vPlayerRotation.y = _yRotation;
	vPlayerPos.y += _offsetPos.y;
	vPlayerPos.z = 100.f;

	CGameObject* pObject = new CGameObject;
	pObject->SetName(_effectName);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CAnimator2D);
	Vec3 playerpos = Transform()->GetRelativePos();
	pObject->Transform()->SetRelativePos(vPlayerPos);
	pObject->Transform()->SetRelativeScale(_scale);
	pObject->Transform()->SetRelativeRotation(vPlayerRotation);
	Vec2 backgroundSize = Vec2(200, 200);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pObject->Animator2D()->LoadAnimFromFile(_effectName, backgroundSize);
	pObject->Animator2D()->Play(_effectName, true);
	Vec3 vMissilePos = Vec3(playerpos.x + 50.f, playerpos.y, 200.f);

	pObject->AddComponent(new CEffectScript(_effectName, _time));
	CSceneMgr::GetInst()->GetCurScene()->AddObject(pObject, L"Effect");
}

void CBoss_Wolf::CreateShadow()
{
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	Vec3 vPlayerRotate = Transform()->GetRelativeRotation();

	Vec3 vMonsterSclae = Transform()->GetRelativeScale();

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"BossShadow");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CCollider2D);

	pObject->AddComponent(new CShadowWolfScript(m_vShadowStartPos, m_pMonsterObject->GetScript<CMonsterScript>()->GetMonsterRorL(), vPlayerRotate.y));

	Vec3 playerpos = Transform()->GetRelativePos();
	pObject->Transform()->SetRelativePos(m_vShadowStartPos);
	pObject->Transform()->SetRelativeScale(vMonsterSclae);

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	Vec2 backgroundSize = Vec2(600, 600);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pObject->Animator2D()->LoadAnimFromFile(L"Boss_Wolf_Dash_Shadow", backgroundSize);
	pObject->Animator2D()->Play(L"Boss_Wolf_Dash_Shadow", false);

	CSceneMgr::GetInst()->GetCurScene()->AddObject(pObject, L"MonsterAttack");
}

void CBoss_Wolf::OnCollisionEnter(CGameObject* _OtherObject)
{
	CMonsterScript::OnCollisionEnter(_OtherObject);

	float CurHp = CMonsterScript::GetMonsterHP();
	m_iMonsterHP = (UINT)CurHp;
}

void CBoss_Wolf::OnCollisionExit(CGameObject* _OtherObject)
{
	
}

void CBoss_Wolf::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fTraceRange, sizeof(float), 1, _pFile);
	fwrite(&m_fAttackRange, sizeof(float), 1, _pFile);
	fwrite(&m_fAttackCoolTime, sizeof(float), 1, _pFile);
	fwrite(&m_iMonsterHP, sizeof(UINT), 1, _pFile);
	fwrite(&m_iMonsterHP_Fix, sizeof(UINT), 1, _pFile);
	fwrite(&m_pMonsterState, sizeof(tMonsterState), 1, _pFile);
	fwrite(&m_pMonsterIdleState, sizeof(tMonsterIdleState), 1, _pFile);
	fwrite(&m_bStartFunc, sizeof(bool), 1, _pFile);
	fwrite(&m_bDuringAttack, sizeof(bool), 1, _pFile);
	fwrite(&m_bBossDisappear, sizeof(bool), 1, _pFile);
	fwrite(&m_bBossFlip, sizeof(bool), 1, _pFile);
	fwrite(&m_bDeadAnimationOn, sizeof(bool), 1, _pFile);
	fwrite(&m_fAttackTime, sizeof(float), 1, _pFile);
	fwrite(&m_bStunOn, sizeof(bool), 1, _pFile);
	fwrite(&m_pStartMotion, sizeof(BossStartMotion), 1, _pFile);
	fwrite(&m_pAttackMotion, sizeof(BossAttackMotion), 1, _pFile);
	fwrite(&m_pMotionState, sizeof(CurBossMotion), 1, _pFile);
	fwrite(&m_bHowlingOn, sizeof(bool), 1, _pFile);
	fwrite(&m_iHowlingCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_bHowlingEndOn, sizeof(bool), 1, _pFile);
	fwrite(&m_bBreathOn, sizeof(bool), 1, _pFile);
	fwrite(&m_bDashOn, sizeof(bool), 1, _pFile);
	fwrite(&m_bShadowDashOn, sizeof(bool), 1, _pFile);
	fwrite(&m_vShadowStartPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_bBreath1, sizeof(bool), 1, _pFile);
	fwrite(&m_bBreath2, sizeof(bool), 1, _pFile);
	fwrite(&m_pMonsterObject, sizeof(CGameObject*), 1, _pFile);
}

void CBoss_Wolf::LoadFromScene(FILE* _pFile)
{
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_fTraceRange, sizeof(float), 1, _pFile);
	fread(&m_fAttackRange, sizeof(float), 1, _pFile);
	fread(&m_fAttackCoolTime, sizeof(float), 1, _pFile);
	fread(&m_iMonsterHP, sizeof(UINT), 1, _pFile);
	fread(&m_iMonsterHP_Fix, sizeof(UINT), 1, _pFile);
	fread(&m_pMonsterState, sizeof(tMonsterState), 1, _pFile);
	fread(&m_pMonsterIdleState, sizeof(tMonsterIdleState), 1, _pFile);
	fread(&m_bStartFunc, sizeof(bool), 1, _pFile);
	fread(&m_bDuringAttack, sizeof(bool), 1, _pFile);
	fread(&m_bBossDisappear, sizeof(bool), 1, _pFile);
	fread(&m_bBossFlip, sizeof(bool), 1, _pFile);
	fread(&m_bDeadAnimationOn, sizeof(bool), 1, _pFile);
	fread(&m_fAttackTime, sizeof(float), 1, _pFile);
	fread(&m_bStunOn, sizeof(bool), 1, _pFile);
	fread(&m_pStartMotion, sizeof(BossStartMotion), 1, _pFile);
	fread(&m_pAttackMotion, sizeof(BossAttackMotion), 1, _pFile);
	fread(&m_pMotionState, sizeof(CurBossMotion), 1, _pFile);
	fread(&m_bHowlingOn, sizeof(bool), 1, _pFile);
	fread(&m_iHowlingCount, sizeof(UINT), 1, _pFile);
	fread(&m_bHowlingEndOn, sizeof(bool), 1, _pFile);
	fread(&m_bBreathOn, sizeof(bool), 1, _pFile);
	fread(&m_bDashOn, sizeof(bool), 1, _pFile);
	fread(&m_bShadowDashOn, sizeof(bool), 1, _pFile);
	fread(&m_vShadowStartPos, sizeof(Vec3), 1, _pFile);
	fread(&m_bBreath1, sizeof(bool), 1, _pFile);
	fread(&m_bBreath2, sizeof(bool), 1, _pFile);
	fread(&m_pMonsterObject, sizeof(CGameObject*), 1, _pFile);
}

