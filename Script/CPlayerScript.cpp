#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"
#include "CPlayer_Weapon_Collider.h"
#include "CMonster_Attack_Collider.h"
#include "CEffectScript.h"
#include "CPlayerArrow.h"
#include "CGuardScript.h"
#include "CGravity.h"
#include "CWall.h"
#include "CPlayerUIScript.h"
#include "CSceneChangeScript.h"

#include "CSceneScript_Start.h"
#include "CSceneScript_01.h"
#include "CSceneScript_02.h"
#include "CSceneScript_03.h"
#include "CScene_Boss.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CScript.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CCollider2D.h>
#include <Engine/CEventMgr.h>
#include <Engine/CSceneMgr.h>


CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pMissilePrefab(nullptr)
	, m_fSpeed(300.f)
	, m_fBurnStrength(0.f)
	, m_bBurn(false)
	, m_pCurWeapon(tCurWeaponType::NONE)
	, m_pPlayerLookDir(tPlayerLookDir::RIGHT)
	, m_bDash(false)
	, m_fDashTime(0.f)
	, m_bJump(false)
	, m_bJumpFall(false)
	, m_bQAttack(false)
	, m_fJumpTime(0.f)
	, m_pQKeyAttack(tQKeyAttack::Q_KEY_NONE)
	, m_bHammerOverload(false)
	, m_pHammerOverLoadState(tHammerOverLoad::HAM_OVER_NONE)
	, m_fAttackTime(0.f)
	, m_fAttackColliderOn(false)
	, m_bOverLoadStart(false)
	, m_bSwordEAttack(false)
	, m_bPlayerHit(false)
	, m_bGuardOn(false)
	, m_bContectWallR(false)
	, m_bContectWallL(false)
	, m_fHP(100)
	, m_iFullHP(100)
	, m_bPlayerDie(false)
	, stageChangeOn(false)
	, m_pPlayerSound(nullptr)
{
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);

	if (-1 != CSceneMgr::GetInst()->GetHp())
		m_fHP = CSceneMgr::GetInst()->GetHp();

	if (tCurWeaponType::NONE != CSceneMgr::GetInst()->GetWeaponType())
		m_pCurWeapon = CSceneMgr::GetInst()->GetWeaponType();

}

CPlayerScript::CPlayerScript(const CPlayerScript& _Origin)
	: CScript(_Origin)
	, m_pMissilePrefab(nullptr)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_fBurnStrength(0.f)
	, m_bBurn(false)
	, m_pCurWeapon(_Origin.m_pCurWeapon)
	, m_pPlayerLookDir(_Origin.m_pPlayerLookDir)
	, m_bDash(false)
	, m_fDashTime(0.f)
	, m_bJump(false)
	, m_bJumpFall(false)
	, m_bQAttack(false)
	, m_fJumpTime(0.f)
	, m_pQKeyAttack(_Origin.m_pQKeyAttack)
	, m_bHammerOverload(false)
	, m_pHammerOverLoadState(_Origin.m_pHammerOverLoadState)
	, m_fAttackTime(0.f)
	, m_fAttackColliderOn(false)
	, m_bOverLoadStart(false)
	, m_bSwordEAttack(false)
	, m_bPlayerHit(false)
	, m_bGuardOn(false)
	, m_bContectWallR(false)
	, m_bContectWallL(false)
	, m_fHP(_Origin.m_fHP)
	, m_iFullHP(_Origin.m_iFullHP)
	, m_bPlayerDie(false)
	, stageChangeOn(false)
{

	if (-1 != CSceneMgr::GetInst()->GetHp())
		m_fHP = CSceneMgr::GetInst()->GetHp();

	if (tCurWeaponType::NONE != CSceneMgr::GetInst()->GetWeaponType())
		m_pCurWeapon = CSceneMgr::GetInst()->GetWeaponType();

}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::start()
{
	m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");

	vector<CGameObject*> vec = CSceneMgr::GetInst()->GetCurScene()->GetLayer(2)->GetRootObjects();

	if (vec.size() == 0)
		return;


	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(2)->GetRootObjects()[0]->GetScript<CPlayerScript>() == nullptr)
		return;

	if (0 != vec.size())
	{
		m_pPlayerObject = CSceneMgr::GetInst()->GetCurScene()->GetLayer(2)->GetRootObjects()[0];
		if (m_pPlayerObject->GetName() != L"Player")
			return;
	}
	if (nullptr != m_pPlayerObject->GetScript<CPlayerScript>())
	{
		Vec2 backgroungSize = Vec2(300, 300);
		m_pPlayerObject = GetOwner();
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Jump", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Dash", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Die", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Fall", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Fall_End", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Idle", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Landing", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Run", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Spawn", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Default_Stun", backgroungSize);

		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Attack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Dash", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Die", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Fall", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Fall_End", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Idle", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Landing", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Jump", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Overload_Attack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Run", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_SPAttack_Right", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Stun", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Up", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_UPtoFall", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Hit", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Bow_Guard", backgroungSize);

		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Attack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Dash", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Die", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Fall", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Fall_End", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Idle", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Landing", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Jump", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Overload_Attack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Overload_Attack_End", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Overload_Attack_Ready", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Run", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_SPAttack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Stun", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Up", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_UPtoFall", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Hit", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Hammer_Guard", backgroungSize);

		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Attack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Dash", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Die", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Fall", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Fall_End", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Idle", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Landing", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Jump", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Overload_Attack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Run", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_SPAttack", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Stun", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Up", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_UPtoFall", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Hit", backgroungSize);
		m_pPlayerObject->Animator2D()->LoadAnimFromFile(L"Sword_Guard", backgroungSize);

		m_pPlayerObject->Animator2D()->Play(L"Default_Idle", true);
	}
	CResMgr::GetInst()->Load<CMaterial>(L"material\\PlayerHP.mtrl", L"material\\PlayerHP.mtrl");
	CResMgr::GetInst()->Load<CMaterial>(L"material\\PlayerSP.mtrl", L"material\\PlayerSP.mtrl");
	CResMgr::GetInst()->Load<CMaterial>(L"material\\PlayerUI.mtrl", L"material\\PlayerUI.mtrl");

	CSceneMgr::GetInst()->GetReadyScene();

	CGameObject* PlayerHPUI = new CGameObject;
	PlayerHPUI->SetName(L"PlayerUI_HP");
	PlayerHPUI->AddComponent(new CTransform);
	PlayerHPUI->AddComponent(new CMeshRender);
	PlayerHPUI->AddComponent(new CPlayerUIScript);
	//PlayerHPUI->SetLayerIndex(1);
	PlayerHPUI->Transform()->SetRelativePos(-230.f, 1485.f, 100.f);
	PlayerHPUI->Transform()->SetRelativeScale(245.f, 25.f, 1.f);
	PlayerHPUI->Transform()->SetIgnoreParentScale(true);
	PlayerHPUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	PlayerHPUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PlayerHP.mtrl"));
	PlayerHPUI->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\UI_HP_Bar.png", L"texture\\ui\\UI_HP_Bar.png"));
	//m_pPlayerObject->AddChild(PlayerHPUI);
	CSceneMgr::GetInst()->GetReadyScene()->AddObject(PlayerHPUI, L"Default");


	CGameObject* PlayerSPUI = new CGameObject;
	PlayerSPUI->SetName(L"PlayerUI_SP");
	PlayerSPUI->AddComponent(new CTransform);
	PlayerSPUI->AddComponent(new CMeshRender);
	PlayerSPUI->AddComponent(new CPlayerUIScript);
	//PlayerSPUI->SetLayerIndex(1);
	PlayerSPUI->Transform()->SetRelativePos(+230.f, 1485.f, 100.f);
	PlayerSPUI->Transform()->SetRelativeScale(245.f, 25.f, 1.f);
	PlayerSPUI->Transform()->SetIgnoreParentScale(true);
	PlayerSPUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	PlayerSPUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PlayerSP.mtrl"));
	PlayerSPUI->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\UI_SPAttack_Bar.png", L"texture\\ui\\UI_SPAttack_Bar.png"));
	//m_pPlayerObject->AddChild(PlayerSPUI);
	CSceneMgr::GetInst()->GetReadyScene()->AddObject(PlayerSPUI, L"Default");

	CGameObject* PlayerUI = new CGameObject;
	PlayerUI->SetName(L"PlayerUI_Weapon");
	PlayerUI->AddComponent(new CTransform);
	PlayerUI->AddComponent(new CMeshRender);
	//PlayerUI->SetLayerIndex(1);
	PlayerUI->Transform()->SetRelativePos(0.f, 1500.f, 100.f);
	PlayerUI->Transform()->SetRelativeScale(800.f, 230.f, 1.f);
	PlayerUI->Transform()->SetIgnoreParentScale(true);
	PlayerUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	PlayerUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PlayerUI.mtrl"));
	PlayerUI->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_HP_Frame_Sword.png", L"texture\\UI_HP_Frame_Sword.png"));
	CSceneMgr::GetInst()->GetReadyScene()->AddObject(PlayerUI, L"Default");
	//m_pPlayerObject->AddChild(PlayerUI);
	m_pUIFrame = PlayerUI;

	if (m_pCurWeapon != tCurWeaponType::NONE)
	{
		if (m_pCurWeapon == tCurWeaponType::SWORD)
			m_pUIFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_HP_Frame_Sword.png", L"texture\\UI_HP_Frame_Sword.png"));
		else if (m_pCurWeapon == tCurWeaponType::BOW)
			m_pUIFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_HP_Frame_Bow.png", L"texture\\UI_HP_Frame_Bow.png"));
		else if (m_pCurWeapon == tCurWeaponType::HAMMER)
			m_pUIFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_HP_Frame_Hammer.png", L"texture\\UI_HP_Frame_Hammer.png"));
	}
}

void CPlayerScript::update()
{
	if (stageChangeOn)
	{
		ChangeState();
		return;
	}

	if (m_pPlayerObject == nullptr)
	{
		start();
		return;
	}
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRotate = Transform()->GetRelativeRotation();

	if (m_bPlayerDie)
	{
		if (true == m_pPlayerObject->Animator2D()->GetCurAnim()->IsFinish())
		{
			CSceneMgr::GetInst()->ChangeSceneNumber(-1);
			m_pPlayerObject->Destroy();
			return;
		}
	}
	if (KEY_TAP(KEY::_5))
	{
		m_fHP -= 10;
	}

	if (m_bPlayerHit)
	{
		if (true == m_pPlayerObject->Animator2D()->GetCurAnim()->IsFinish())
		{
			m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
			m_bPlayerHit = false;
			ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
			m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_Hit.wav", L"sound\\SE_Player_Hit.wav");
			m_pPlayerSound->Play(1, 0.5f);
		}	
	}

	if (KEY_PRESSED(KEY::A))
	{
		if(!m_bDash)
			if(!m_bJump)
				if (!m_bJumpFall)
					if(!m_bQAttack && !m_bPlayerHit)
						{
							ChangeAnim(L"Default_Run", L"Sword_Run", L"Bow_Run", L"Hammer_Run", true);
							vRotate.y = -3.141592;
							if (m_bContectWallR)
								m_bContectWallR = false;
						}
		if(!m_bContectWallL)
			vPos.x -= DT * m_fSpeed;
		m_pPlayerLookDir = tPlayerLookDir::LEFT;
	}
	if (KEY_PRESSED(KEY::D))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack && !m_bPlayerHit)
						{
							ChangeAnim(L"Default_Run", L"Sword_Run", L"Bow_Run", L"Hammer_Run", true);
							vRotate.y = 0;
							if (m_bContectWallL)
								m_bContectWallL = false;
						}
		if (!m_bContectWallR)
			vPos.x += DT * m_fSpeed;
		m_pPlayerLookDir = tPlayerLookDir::RIGHT;
	}
	if (KEY_AWAY(KEY::A))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack && !m_bPlayerHit)
						{
							m_pPlayerObject->Animator2D()->SetAccTimeZero();
							ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
						}
	}
	if (KEY_AWAY(KEY::D))
	{
		if(!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack && !m_bPlayerHit)
						{
							m_pPlayerObject->Animator2D()->SetAccTimeZero();
							ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
						}
	}

	//======
	// DASH
	//======
	if (KEY_TAP(KEY::LSHFT))
	{
		ChangeAnim(L"Default_Dash", L"Sword_Dash", L"Bow_Dash", L"Hammer_Dash", false);
		CreatePlayerEffect(L"Effect_Player_Dash", Vec3(10.f, -20.f, 0.f), Vec3(400, 400, 1), false);
		m_bDash = true;
		m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_Dash.wav", L"sound\\SE_Player_Dash.wav");
		m_pPlayerSound->Play(1, 0.3f, true);
	}
	if (m_bDash)
	{
		if (KEY_PRESSED(KEY::A))
		{
			vRotate.y = -3.141592;
			m_pPlayerLookDir = tPlayerLookDir::LEFT;
		}
		else if (KEY_PRESSED(KEY::D))
		{
			vRotate.y = 0;
			m_pPlayerLookDir = tPlayerLookDir::RIGHT;
		}
		if (m_pPlayerLookDir == tPlayerLookDir::RIGHT)
		{
			if (!m_bContectWallR)
				vPos.x += DT * m_fSpeed * 3.f;
			if (m_bContectWallL)
				m_bContectWallL = false;
		}
		if (m_pPlayerLookDir == tPlayerLookDir::LEFT)
		{
			if (!m_bContectWallL)
				vPos.x -= DT * m_fSpeed * 3.f;
			if (m_bContectWallR)
				m_bContectWallR = false;
		}
		m_fDashTime += DT;

		if (0.3f < m_fDashTime)
		{
			m_bDash = false;
			if(m_pPlayerObject->GetScript<CGravity>()->IsGround())
				ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
			else
				ChangeAnim(L"Default_Fall", L"Sword_Fall", L"Bow_Fall", L"Hammer_Fall", true);
			m_fDashTime = 0.f;
		}
	}

	//======
	// JUMP
	//======
	if (KEY_TAP(KEY::SPACE))
	{
		m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_Jump.wav", L"sound\\SE_Player_Jump.wav");
		m_pPlayerSound->Play(1, 0.3f, true);

		m_bJump = true;
		m_fJumpTime = 0.f;
		ChangeAnim(L"Default_Jump", L"Sword_Jump", L"Bow_Jump", L"Hammer_Jump", false);
		CreatePlayerEffect(L"Effect_Player_Jump", Vec3(0.f, -20.f, 0.f), Vec3(500.f, 500.f, 1.f), false);
		m_pPlayerObject->GetScript<CGravity>()->GetVelocity().y = +400.f;
		m_pPlayerObject->GetScript<CGravity>()->OnAir();
		if (KEY_PRESSED(KEY::A))
		{
			vRotate.y = -3.141592;
			m_pPlayerLookDir = tPlayerLookDir::LEFT;
		}
		else if (KEY_PRESSED(KEY::D))
		{
			vRotate.y = 0;
			m_pPlayerLookDir = tPlayerLookDir::RIGHT;
		}
	}
	else if (m_bJump)
	{
		m_fJumpTime += DT;
		if (0.4f < m_fJumpTime && 0.7f > m_fJumpTime)
		{
			if (m_pPlayerObject->Animator2D()->GetCurAnim()->IsFinish())
			{
				m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
				ChangeAnim(L"Default_Fall", L"Sword_Fall", L"Bow_Fall", L"Hammer_Fall", true);
				m_bJumpFall = true;
			}
		}
		if (0.6f < m_fJumpTime && m_bJumpFall)
		{
			if (KEY_PRESSED(KEY::A))
			{
				vRotate.y = -3.141592;
				m_pPlayerLookDir = tPlayerLookDir::LEFT;
			}
			else if(KEY_PRESSED(KEY::D))
			{
				vRotate.y = 0;
				m_pPlayerLookDir = tPlayerLookDir::RIGHT;
			}
			if (20.f <= m_fJumpTime)
			{
				if (m_pPlayerObject->Animator2D()->GetCurAnim()->IsFinish())
				{
					m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
					ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
					m_bJumpFall = false;
					m_fJumpTime = 0.f;
					m_bJump = false;
				}
			}
			else if (m_pPlayerObject->GetScript<CGravity>()->IsGround())
			{
				ChangeAnim(L"Default_Fall_End", L"Sword_Fall_End", L"Bow_Fall_End", L"Hammer_Fall_End", false);	
				m_fJumpTime += 20.f;
			}	
			
		}
	}
	//========
	// Attack
	//========
	if (KEY_TAP(KEY::LBTN))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack && !m_bPlayerHit)
						{
							ChangeAnim(L"", L"Sword_Attack", L"Bow_Attack", L"Hammer_Attack", false);
							if (m_pCurWeapon == tCurWeaponType::BOW)
							{
								m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Bow_Attack_Draw_0.wav", L"sound\\SE_Weapon_Bow_Attack_Draw_0.wav");
								m_pPlayerSound->Play(1, 0.3f, true);
							}
							else if (m_pCurWeapon == tCurWeaponType::SWORD)
							{
								m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Sword_Attack_0_0.wav", L"sound\\SE_Weapon_Sword_Attack_0_0.wav");
								m_pPlayerSound->Play(1, 0.3f, true);
							}
							else
							{
								m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Hammer_Attack.wav", L"sound\\SE_Weapon_Hammer_Attack.wav");
								m_pPlayerSound->Play(1, 0.3f, true);
							}
						}
	}
	if (KEY_PRESSED(KEY::LBTN))
	{
		m_fAttackTime += DT;

		if (m_pCurWeapon == tCurWeaponType::BOW)
		{
			if (1.f <= m_fAttackTime && 1.f + DT >= m_fAttackTime)
			{
				m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Bow_Attack_Shoot_0.wav", L"sound\\SE_Weapon_Bow_Attack_Shoot_0.wav");
				m_pPlayerSound->Play(1, 0.3f, true);
				CreateArrow(Vec3(20.f, 0.f, 0.f), false);
			}
			else if (1.5f < m_fAttackTime)
			{
				m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
				ChangeAnim(L"", L"Sword_Attack", L"Bow_Attack", L"Hammer_Attack", false);
				m_fAttackTime = 0.f;
			}
		}
		else if (m_pCurWeapon == tCurWeaponType::HAMMER)
		{
			if (1.2f <= m_fAttackTime && 1.2f + DT * 2.f >= m_fAttackTime)
			{
				m_fAttackColliderOn = false;
				m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Hammer_Attack_2.wav", L"sound\\SE_Weapon_Hammer_Attack_2.wav");
				m_pPlayerSound->Play(1, 0.3f, true);
				if (false == m_fAttackColliderOn)
					CreateWeaponCollider(0.2f, Vec3(80, 0, 0), Vec3(140, 160, 1), m_pCurWeapon, false, false);
				m_fAttackColliderOn = true;
			}
			else if (0.2f <= m_fAttackTime)
			{			
				if (m_pCurWeapon == tCurWeaponType::HAMMER)
				{
					if (false == m_fAttackColliderOn)
						CreateWeaponCollider(0.2f, Vec3(80, 0, 0), Vec3(100, 140, 1), m_pCurWeapon, false, false);
					m_fAttackColliderOn = true;
				}
			}
		}
		else if (m_pCurWeapon == tCurWeaponType::SWORD)
		{
			if (1.f <= m_fAttackTime && 1.f + DT * 1.f >= m_fAttackTime)
			{
				if (m_pCurWeapon == tCurWeaponType::SWORD)
				{
					m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Sword_Attack_3.wav", L"sound\\SE_Weapon_Sword_Attack_3.wav");
					m_pPlayerSound->Play(1, 0.3f, true);
					m_fAttackColliderOn = false;
					if (false == m_fAttackColliderOn)
						CreateWeaponCollider(0.2f, Vec3(50, 0, 0), Vec3(160, 100, 1), m_pCurWeapon, false, false);
					m_fAttackColliderOn = true;
				}
			}
			else if (0.5f <= m_fAttackTime && 0.5f + DT * 2.f >= m_fAttackTime)
			{
				if (m_pCurWeapon == tCurWeaponType::SWORD)
				{
					m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Sword_Attack_0_1.wav", L"sound\\SE_Weapon_Sword_Attack_0_1.wav");
					m_pPlayerSound->Play(1, 0.3f, true);
					m_fAttackColliderOn = false;
					if (false == m_fAttackColliderOn)
						CreateWeaponCollider(0.2f, Vec3(50, 0, 0), Vec3(120, 80, 1), m_pCurWeapon, false, false);
					m_fAttackColliderOn = true;
				}
			}
			else if (0.2f <= m_fAttackTime)
			{
				if (m_pCurWeapon == tCurWeaponType::SWORD)
				{
					if (false == m_fAttackColliderOn)
						CreateWeaponCollider(0.2f, Vec3(50, 0, 0), Vec3(120, 80, 1), m_pCurWeapon, false, false);
					m_fAttackColliderOn = true;
				}				
			}
		}	

	}
	if (KEY_AWAY(KEY::LBTN))
	{
		m_fAttackTime = 0.f;
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack)
					{
						m_fAttackColliderOn = false;
						m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
						m_pPlayerObject->Animator2D()->SetAccTimeZero();
						ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
					}
	}

	//=======
	// Guard
	//=======
	if (KEY_TAP(KEY::RBTN))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack)
					{
						m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_Guard_Up.wav", L"sound\\SE_Player_Guard_Up.wav");
						m_pPlayerSound->Play(1, 0.3f, true);
							ChangeAnim(L"", L"Sword_Guard", L"Bow_Guard", L"Hammer_Guard", true);
							CreatePlayerEffect(L"Effect_Player_ParryTime", Vec3(20.f, 0.f, 0.f), Vec3(200.f, 400.f, 1.f), false);
							CreateWeaponCollider(0.3f, Vec3(40.f, 0.f, 0.f), Vec3(10.f, 40.f, 1.f), m_pCurWeapon, false, false, true);
					}
	}
	if (KEY_AWAY(KEY::RBTN))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack)
						{
							m_pPlayerObject->Animator2D()->SetAccTimeZero();
							ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
						}
	}

	//=====
	// Hit
	//=====
	if (KEY_TAP(KEY::Z))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack)
						{
							ChangeAnim(L"", L"Sword_Hit", L"Bow_Hit", L"Hammer_Hit", true);
						}
	}
	if (KEY_AWAY(KEY::Z))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack)
						{
							m_pPlayerObject->Animator2D()->SetAccTimeZero();
							ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
						}
	}

	//==========
	// Q Attack
	//==========
	if (KEY_TAP(KEY::Q))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall)
					if (!m_bQAttack && !m_bPlayerHit)
						{
							ChangeAnim(L"", L"Sword_SPAttack", L"Bow_SPAttack_Right", L"Hammer_SPAttack", false);
							if (m_pCurWeapon == tCurWeaponType::SWORD)
							{
								m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Sword_SPAttack.wav", L"sound\\SE_Weapon_Sword_SPAttack.wav");
								m_pPlayerSound->Play(1, 0.3f, true);
								m_pQKeyAttack = tQKeyAttack::Q_KEY_SWORD;
								if (m_pPlayerLookDir == tPlayerLookDir::LEFT)
								{
									vPos.x -= 700.f;
									CreateWeaponCollider(1.5f, Vec3(280, 0, 0), Vec3(750, 150, 1), m_pCurWeapon, false, false);
								}
								else
								{
									vPos.x += 700.f;
									CreateWeaponCollider(1.5f, Vec3(280, 0, 0), Vec3(750, 150, 1), m_pCurWeapon, false, false);
								}
								CreatePlayerEffect(L"Effect_Weapon_Sword_SPAttack_Smoke", Vec3(10.f, -20.f, 0.f), Vec3(400.f, 400.f, 1.f), false);
								CreatePlayerEffect(L"Effect_Weapon_Sword_SPAttack", Vec3(350.f, 0.f, 0.f), Vec3(800.f, 300.f, 1.f), false);
							}
							else if (m_pCurWeapon == tCurWeaponType::BOW)
							{								
								m_pQKeyAttack = tQKeyAttack::Q_KEY_BOW;
							}
							else if (m_pCurWeapon == tCurWeaponType::HAMMER)
							{
								CSceneMgr::GetInst()->SetPlayerUseHammerSP(true);
								m_pQKeyAttack = tQKeyAttack::Q_KEY_HAMMER;
							}
							m_bQAttack = true;
						}
	}
	if (KEY_PRESSED(KEY::Q))
	{
		m_fAttackTime += DT;
		if (m_pCurWeapon == tCurWeaponType::BOW)
		{
			if (0.5f <= m_fAttackTime && 0.5f + DT >= m_fAttackTime)
			{
				CreateArrow(Vec3(60.f, 15.f, 0.f), true);
				CreateArrow(Vec3(40.f, 5.f, 0.f), true);
				CreateArrow(Vec3(20.f, -5.f, 0.f), true);
				CreateArrow(Vec3(0.f, -15.f, 0.f), true);
			}
		}
	}
	if (tQKeyAttack::Q_KEY_NONE != m_pQKeyAttack)
	{
		bool finish = m_pPlayerObject->Animator2D()->GetCurAnim()->IsFinish();
		if (finish)
		{
			m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
			ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
			m_pQKeyAttack = tQKeyAttack::Q_KEY_NONE;
			m_bQAttack = false;
			m_fAttackTime = 0.f;
		}
	}


	//==========
	// E Attack
	//==========
	if (KEY_TAP(KEY::E))
	{
		if(m_bOverLoadStart)
			if (!m_bDash)
				if (!m_bJump)
					if (!m_bJumpFall && !m_bQAttack && !m_bPlayerHit)
					{
						if (m_pCurWeapon == tCurWeaponType::HAMMER)
						{

							m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Hammer_Attack_Overload_Ready.wav", L"sound\\SE_Weapon_Hammer_Attack_Overload_Ready.wav");
							m_pPlayerSound->Play(1, 0.3f, true);
							m_pPlayerObject->Animator2D()->Play(L"Hammer_Overload_Attack_Ready", true);
							CreatePlayerEffect(L"Effect_Player_Overload", Vec3(0.f, 0.f, 0.f), Vec3(800.f, 800.f, 1.f), false);

							m_bHammerOverload = true;
						}
						else
						{
							ChangeAnim(L"", L"Sword_Overload_Attack", L"Bow_Overload_Attack", L"Hammer_Overload_Attack_Ready", true);
							CreatePlayerEffect(L"Effect_Player_Overload", Vec3(0.f, 0.f, 0.f), Vec3(800.f, 800.f, 1.f), false);

						}
					}

		if (m_bOverLoadStart == false)
		{
			CreatePlayerEffect(L"Effect_Player_Overload_Start", Vec3(0.f, 0.f, 0.f), Vec3(400.f, 400.f, 1.f), false);

			m_bOverLoadStart = true;
		}
	}
	if (KEY_PRESSED(KEY::E))
	{
		m_fAttackTime += DT;
		if (0.2f <= m_fAttackTime)
		{
			if (m_pCurWeapon == tCurWeaponType::SWORD)
			{
				if (false == m_fAttackColliderOn)
					CreateWeaponCollider(0.2f, Vec3(80, 0, 0), Vec3(200, 90, 1), m_pCurWeapon, true, false);
				m_fAttackColliderOn = true;
				m_bSwordEAttack = true;
				m_fAttackTime = 0;
			}
			else if (m_pCurWeapon == tCurWeaponType::HAMMER)
			{
				if (false == m_fAttackColliderOn)
					CreateWeaponCollider(0.3f, Vec3(0, 0, 0), Vec3(160, 120, 1), m_pCurWeapon, false, false);
				m_fAttackColliderOn = true;
				m_fAttackTime = 0;
			}
			else if (m_pCurWeapon == tCurWeaponType::BOW)
			{
				if (0.6f <= m_fAttackTime)
				{
					CreatePlayerEffect(L"Effect_Weapon_Bow_Overload_Attack", Vec3(0.f, 0.f, 0.f), Vec3(400.f, 400.f, 1.f), true);

					CreateArrow(Vec3(60.f, 15.f, 0.f), false);
					CreateArrow(Vec3(40.f, 5.f, 0.f), false);
					CreateArrow(Vec3(20.f, -5.f, 0.f), false);
					CreateArrow(Vec3(0.f, -15.f, 0.f), false);
					m_fAttackTime = 0;
				}
			}
			m_fAttackColliderOn = false;
		}
	}
	if (KEY_AWAY(KEY::E))
	{
		if (!m_bDash)
			if (!m_bJump)
				if (!m_bJumpFall && !m_bQAttack && !m_bPlayerHit)
				{
					m_pPlayerObject->Animator2D()->SetAccTimeZero();
					if (m_pCurWeapon != tCurWeaponType::HAMMER)
						ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
					m_bSwordEAttack = false;

					if (m_pCurWeapon == tCurWeaponType::BOW)
					{
						CLayer* layer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Effect");
						vector<CGameObject*> pVecObj = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Effect")->GetRootObjects();
						for (size_t i = 0; i < pVecObj.size(); i++)
						{
							if (pVecObj[i]->GetName() == L"Effect_Weapon_Bow_Overload_Attack")
								pVecObj[i]->Destroy();
						}
						CreatePlayerEffect(L"Effect_Weapon_Bow_Overload_Attack_End", Vec3(0.f, 0.f, 0.f), Vec3(400.f, 400.f, 1.f), false);
					}
				}

		// 일단 여기에 둠
		// 오버로드가 끝나면 false 처리 해줌.
		//m_bOverLoadStart = false;
	}

	//========================
	// Hammer Overload Attack
	//========================
	if (m_bHammerOverload)
	{

		if (KEY_PRESSED(KEY::E))
		{
			static float HamOverLoad = 0.01f;
			HamOverLoad += DT;
			if (m_pHammerOverLoadState == tHammerOverLoad::HAM_OVER_NONE)
			{
				m_pPlayerObject->Animator2D()->Play(L"Hammer_Overload_Attack_Ready", false);
				CreatePlayerEffect(L"Effect_Weapon_Hammer_Overload_Attack_Ready", Vec3(0.f, 0.f, 0.f), Vec3(400.f, 200.f, 1.f), false);
				m_pHammerOverLoadState = tHammerOverLoad::HAM_OVER_START;

			}
			if (m_pPlayerObject->Animator2D()->GetCurAnim()->IsFinish() && m_pHammerOverLoadState == tHammerOverLoad::HAM_OVER_START)
			{
				m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Weapon_Hammer_Attack_Overload.wav", L"sound\\SE_Weapon_Hammer_Attack_Overload.wav");
				m_pPlayerSound->Play(1, 0.3f, true);
				m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
				m_pHammerOverLoadState = tHammerOverLoad::HAM_OVER_DOING;
				m_pPlayerObject->Animator2D()->Play(L"Hammer_Overload_Attack", true);
				if (0.1f < HamOverLoad)
				{
					CreatePlayerEffect(L"Effect_Weapon_Hammer_Overload_Attack", Vec3(0.f, 0.f, 0.f), Vec3(400.f, 150.f, 1.f), true);
					HamOverLoad = 0.f;
				}
			}
			
		}
		if (KEY_AWAY(KEY::E))
		{			

			m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
			m_pHammerOverLoadState = tHammerOverLoad::HAM_OVER_END;
			m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
			CLayer* layer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Effect");
			vector<CGameObject*> pVecObj = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Effect")->GetRootObjects();
			for (size_t i = 0; i < pVecObj.size(); i++)
			{
				if (pVecObj[i]->GetName() == L"Effect_Weapon_Hammer_Overload_Attack")
					pVecObj[i]->Destroy();
			}


			m_pPlayerObject->Animator2D()->Play(L"Hammer_Overload_Attack_End", false);
			CreatePlayerEffect(L"Effect_Weapon_Hammer_Overload_Attack_End", Vec3(0.f, 0.f, 0.f), Vec3(400.f, 150.f, 1.f), false);
		}

		if (m_pPlayerObject->Animator2D()->GetCurAnim()->IsFinish() && m_pHammerOverLoadState == tHammerOverLoad::HAM_OVER_END)
		{
			m_pPlayerObject->Animator2D()->GetCurAnim()->Reset();
			m_pHammerOverLoadState = tHammerOverLoad::HAM_OVER_NONE;
			ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
			m_bHammerOverload = false;
		}
	}
	

	//===============
	// Weapon Change
	//===============
	if (KEY_TAP(KEY::_1)) // 검
	{
		m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_WeaponChange.wav", L"sound\\SE_Player_WeaponChange.wav");
		m_pPlayerSound->Play(1, 0.3f, true);
		m_pCurWeapon = tCurWeaponType::SWORD;
		ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
		CreatePlayerEffect(L"Effect_Player_FormChange", Vec3(0.f, 0.f, 0.f), Vec3(400, 400, 1), false);
		m_pUIFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_HP_Frame_Sword.png", L"texture\\UI_HP_Frame_Sword.png"));

	}
	if (KEY_TAP(KEY::_2)) // 활
	{
		m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_WeaponChange.wav", L"sound\\SE_Player_WeaponChange.wav");
		m_pPlayerSound->Play(1, 0.3f, true);
		m_pCurWeapon = tCurWeaponType::BOW;
		ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
		CreatePlayerEffect(L"Effect_Player_FormChange", Vec3(0.f, 0.f, 0.f), Vec3(400, 400, 1), false);
		m_pUIFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_HP_Frame_Bow.png", L"texture\\UI_HP_Frame_Bow.png"));

	}
	if (KEY_TAP(KEY::_3)) // 망치;
	{
		m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_WeaponChange.wav", L"sound\\SE_Player_WeaponChange.wav");
		m_pPlayerSound->Play(1, 0.3f, true);
		m_pCurWeapon = tCurWeaponType::HAMMER;
		ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
		CreatePlayerEffect(L"Effect_Player_FormChange", Vec3(0.f, 0.f, 0.f), Vec3(400, 400, 1), false);
		m_pUIFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI_HP_Frame_Hammer.png", L"texture\\UI_HP_Frame_Hammer.png"));

	}


	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRotate);

	CSceneMgr::GetInst()->SetWeaponType(m_pCurWeapon);
	CSceneMgr::GetInst()->SetHp(m_fHP);
}

void CPlayerScript::lateupdate()
{

}


void CPlayerScript::Burnning()
{
	if (!m_bBurn)
		return;

	m_fBurnStrength += DT * (1.f / 3.f);

	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fBurnStrength);
}

void CPlayerScript::ChangeAnim(wstring _default, wstring _sword, wstring _bow, wstring _hammer, bool _repeat)
{
	if (m_pCurWeapon == tCurWeaponType::NONE)
	{
		if (L"" == _default)
			return;
		m_pPlayerObject->Animator2D()->Play(_default, _repeat);
	}
	else if (m_pCurWeapon == tCurWeaponType::SWORD)
	{
		m_pPlayerObject->Animator2D()->Play(_sword, _repeat);
	}
	else if (m_pCurWeapon == tCurWeaponType::BOW)
	{
		m_pPlayerObject->Animator2D()->Play(_bow, _repeat);
	}
	else if (m_pCurWeapon == tCurWeaponType::HAMMER)
	{
		m_pPlayerObject->Animator2D()->Play(_hammer, _repeat);
	}
}

ContectWallDir CPlayerScript::ContectWall(CGameObject* _wall)
{
	// 벽의 위치와 가로 세로 크기를 가지고 화살이 사방중 어디서 맞았는지를 비교한다.
	Vec3 playerPos = Transform()->GetRelativePos();
	Vec3 wallscale = _wall->Transform()->GetRelativeScale();
	Vec3 pos = _wall->Transform()->GetRelativePos();

	// 사각형의 중심을 0,0 으로 두고 한변의 길이가 2인 정사각형의 충돌체에 맞는다고 비교
	// 
	playerPos -= pos;
	playerPos.x = playerPos.x / wallscale.x * 2.f;
	playerPos.y = playerPos.y / wallscale.y * 2.f;
	ContectWallDir walldir = ContectWallDir::NON_DIR;
	// 왼쪽 벽에 맞았을 때 
	if (1.f <= playerPos.x && -1.f < playerPos.y && playerPos.y < 1.f)
	{
		walldir = ContectWallDir::LEFT;
		m_bContectWallL = true;
	}
	// 위 벽에 맞았을 때
	else if (-1.f >= playerPos.y && -1.f < playerPos.x && playerPos.x < 1.f)
		walldir = ContectWallDir::UP;
	// 밑 벽에 맞았을 때
	else if (1.f <= playerPos.y && -1.f < playerPos.x && playerPos.x < 1.f)
		walldir = ContectWallDir::DOWN;
	// 오른쪽 벽에 맞았을 때
	else if (-1.f >= playerPos.x && -1.f < playerPos.y && playerPos.y < 1.f)
	{
		walldir = ContectWallDir::RIGHT;
		m_bContectWallR = true;
	}

	return walldir;
}


void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{

	if (_OtherObject->GetScript<CSceneChangeScript>() != nullptr)
	{
		stageChangeOn = true;
		return;
	}

	if (m_bGuardOn)
	{
		CreatePlayerEffect(L"Effect_Player_Guard", Vec3(20.f, 0.f, 0.f), Vec3(400.f, 400.f, 1.f), false);
		return;
	}

	// 벽이랑 부딪혔을때 어디랑 부딪힌건지...
	CScript* wallScript = _OtherObject->GetScript<CWall>();
	ContectWallDir walldir = ContectWallDir::NON_DIR;
	if (nullptr != wallScript)
	{
		walldir = ContectWall(_OtherObject);
	}
	// 양 옆에서 벽을 만나면 이동 금지...!
	if (walldir == ContectWallDir::LEFT || walldir == ContectWallDir::RIGHT)
	{
		Vec3 pos = m_pPlayerObject->Transform()->GetRelativePos();
		if (walldir == ContectWallDir::RIGHT)
		{
			pos.x += DT * m_fSpeed;
			m_pPlayerObject->Transform()->SetRelativePos(pos);
		}
		else
		{
			pos.x -= DT * m_fSpeed;
			m_pPlayerObject->Transform()->SetRelativePos(pos);
		}
		//m_pPlayerObject->GetScript<CGravity>()->GetVelocity().x = 0.f;
	}
	if (walldir == ContectWallDir::DOWN)
	{
		ChangeAnim(L"Default_Idle", L"Sword_Idle", L"Bow_Idle", L"Hammer_Idle", true);
	}

	// 맞으면 피 까이고 히트모션
	CScript* MonsterAttackScript = _OtherObject->GetScript<CMonster_Attack_Collider>();
	if (nullptr != MonsterAttackScript)
	{
		m_pPlayerSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Player_Hit.wav", L"sound\\SE_Player_Hit.wav");
		m_pPlayerSound->Play(1, 0.3f, true);

		ChangeAnim(L"", L"Sword_Hit", L"Bow_Hit", L"Hammer_Hit", false);
		m_bPlayerHit = true;
		m_fHP -= _OtherObject->GetScript<CMonster_Attack_Collider>()->GetAttackNum();
		if (0 >= m_fHP)
		{
			ChangeAnim(L"Default_Die", L"Sword_Die", L"Bow_Die", L"Hammer_Die", false);
			m_bPlayerDie = true;
		}
	}

}

void CPlayerScript::OnCollisionExit(CGameObject* _OtherObject)
{
	CScript* wallScript = _OtherObject->GetScript<CWall>();
	ContectWallDir walldir = ContectWallDir::NON_DIR;
	walldir = ContectWall(_OtherObject);
	if (wallScript != nullptr)
	{
		if (_OtherObject->Transform()->GetRelativePos().y < GetOwner()->Transform()->GetRelativePos().y)
		{
			int colli = m_pPlayerObject->Collider2D()->GetCollisonCount();
			if (m_pPlayerObject->Collider2D()->GetCollisonCount() == 0)
			{
				m_pPlayerObject->GetScript<CGravity>()->OnAir();
			}
		}
	}

}

void CPlayerScript::CreateWeaponCollider(float _time, Vec3 _offsetPos, Vec3 _scale, tCurWeaponType _type, bool _swordE, bool _hammerQ, bool _guard)
{
	CGameObject* pColliderObject = new CGameObject;
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	Vec3 Scale = _scale;
	if (m_pPlayerLookDir == tPlayerLookDir::LEFT)
		vPlayerPos.x -= _offsetPos.x;
	else
		vPlayerPos.x += _offsetPos.x;

	vPlayerPos.y += _offsetPos.y;

	if(_swordE || _hammerQ)
		pColliderObject->AddComponent(new CPlayer_Weapon_Collider(_time, _type, _swordE, _hammerQ, 200));
	else
		pColliderObject->AddComponent(new CPlayer_Weapon_Collider(_time, _type, 50));

	pColliderObject->AddComponent(new CTransform);
	pColliderObject->AddComponent(new CMeshRender);
	pColliderObject->AddComponent(new CCollider2D);
	pColliderObject->SetName(L"CPlayer_Weapon_Collider");
	pColliderObject->Transform()->SetRelativePos(vPlayerPos);
	pColliderObject->Transform()->SetRelativeScale(Scale);

	pColliderObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pColliderObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl"));

	pColliderObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pColliderObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pColliderObject->Collider2D()->SetOffsetScale(Vec2(Scale.x, Scale.y));
	if(!_guard)
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pColliderObject, L"PlayerAttack");
	else
	{
		pColliderObject->AddComponent(new CGuardScript(m_pPlayerObject));
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pColliderObject, L"Guard");
	}
}

void CPlayerScript::CreatePlayerEffect(wstring _effectName, Vec3 _offsetPos, Vec3 _scale, bool _repeat)
{
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	Vec3 vPlayerRotation = Transform()->GetRelativeRotation();
	if (m_pPlayerLookDir == tPlayerLookDir::LEFT)
	{
		vPlayerPos.x -= _offsetPos.x;
		vPlayerRotation.y = 3.141592f;
	}
	else
	{
		vPlayerPos.x += _offsetPos.x;
		vPlayerRotation.y = 0.f;
	}
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

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pObject->Animator2D()->LoadAnimFromFile(_effectName, Vec2(300, 300));
	pObject->Animator2D()->Play(_effectName, _repeat);
	Vec3 vMissilePos = Vec3(playerpos.x + 50.f, playerpos.y, 200.f);

	pObject->AddComponent(new CEffectScript(_effectName));
	CSceneMgr::GetInst()->GetCurScene()->AddObject(pObject, L"Effect");
}

void CPlayerScript::CreateArrow(Vec3 _offsetPos, bool _reflect)
{
	CResMgr::GetInst()->Load<CTexture>(L"texture\\Weapon_Bow_Object_Arrow.png", L"texture\\Weapon_Bow_Object_Arrow.png");

	Vec3 vPlayerPos = Transform()->GetRelativePos();
	Vec3 vPlayerRotation = Transform()->GetRelativeRotation();

	if (m_pPlayerLookDir == tPlayerLookDir::LEFT)
	{
		vPlayerPos.x -= _offsetPos.x;
		vPlayerRotation.y = 3.141592f;
	}
	else
	{
		vPlayerPos.x += _offsetPos.x;
		vPlayerRotation.y = 0.f;
	}

	vPlayerPos.y += _offsetPos.y;
	vPlayerPos.z = 100.f;
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Arrow");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerArrow(m_pPlayerLookDir));
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayer_Weapon_Collider(5.f, m_pCurWeapon, 100));
	Vec3 playerpos = Transform()->GetRelativePos();
	pObject->Transform()->SetRelativePos(vPlayerPos);
	pObject->Transform()->SetRelativeScale(Vec3(50.f, 10.f, 1.f));
	pObject->Transform()->SetRelativeRotation(vPlayerRotation);

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(50.f, 10.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\Weapon_Bow_Object_Arrow.png", L"texture\\Weapon_Bow_Object_Arrow.png"));
	if(_reflect)
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pObject, L"ReflectArrow");
	else 
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pObject, L"PlayerAttack");
}

void CPlayerScript::ChangeState()
{
	// Scene Change
	int changeSceneNum = CSceneMgr::GetInst()->GetCurSceneNumber();

	CGameObject* num1Scene = new CGameObject;
	CScene* newScene = new CScene;
	//CSceneMgr::GetInst()->GetCurScene()->SetSceneState(SCENE_STATE::STOP);

	if (changeSceneNum == 0)
	{
		num1Scene->AddComponent(new CSceneScript_Start);
		newScene = num1Scene->GetScript<CSceneScript_Start>()->GetScene();
	}
	if (changeSceneNum == 1)
	{
		num1Scene->AddComponent(new CSceneScript_01);
		newScene = num1Scene->GetScript<CSceneScript_01>()->GetScene();
	}
	if (changeSceneNum == 2)
	{
		num1Scene->AddComponent(new CSceneScript_02);
		newScene = num1Scene->GetScript<CSceneScript_02>()->GetScene();
	}
	if (changeSceneNum == 3)
	{
		num1Scene->AddComponent(new CSceneScript_03);
		newScene = num1Scene->GetScript<CSceneScript_03>()->GetScene();
	}
	if (changeSceneNum == 4)
	{
		num1Scene->AddComponent(new CScene_Boss);
		newScene = num1Scene->GetScript<CScene_Boss>()->GetScene();
	}

	//CSceneMgr::GetInst()->ChangeScene(newScene);
}

void CPlayerScript::SaveToScene(FILE* _pFile)
{
	fwrite(&m_pMissilePrefab, sizeof(Ptr<CPrefab>), 1, _pFile);
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fBurnStrength, sizeof(float), 1, _pFile);
	fwrite(&m_bBurn, sizeof(bool), 1, _pFile);
	fwrite(&m_pPlayerObject, sizeof(CGameObject*), 1, _pFile);
	fwrite(&m_pCurWeapon, sizeof(tCurWeaponType), 1, _pFile);
	fwrite(&m_pPlayerLookDir, sizeof(tPlayerLookDir), 1, _pFile);
	fwrite(&m_pQKeyAttack, sizeof(tQKeyAttack), 1, _pFile);
	fwrite(&m_pHammerOverLoadState, sizeof(tHammerOverLoad), 1, _pFile);
	fwrite(&m_bDash, sizeof(bool), 1, _pFile);
	fwrite(&m_fDashTime, sizeof(float), 1, _pFile);
	fwrite(&m_bJump, sizeof(bool), 1, _pFile);
	fwrite(&m_bJumpFall, sizeof(bool), 1, _pFile);
	fwrite(&m_fJumpTime, sizeof(float), 1, _pFile);
	fwrite(&m_bQAttack, sizeof(bool), 1, _pFile);
	fwrite(&m_bHammerOverload, sizeof(bool), 1, _pFile);
	fwrite(&m_bOverLoadStart, sizeof(bool), 1, _pFile);
	fwrite(&m_bPlayerHit, sizeof(bool), 1, _pFile);
	fwrite(&m_fAttackTime, sizeof(float), 1, _pFile);
	fwrite(&m_fAttackColliderOn, sizeof(bool), 1, _pFile);
	fwrite(&m_bSwordEAttack, sizeof(bool), 1, _pFile);
	fwrite(&m_bGuardOn, sizeof(bool), 1, _pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	fread(&m_pMissilePrefab, sizeof(Ptr<CPrefab>), 1, _pFile);
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_fBurnStrength, sizeof(float), 1, _pFile);
	fread(&m_bBurn, sizeof(bool), 1, _pFile);
	fread(&m_pPlayerObject, sizeof(CGameObject*), 1, _pFile);
	fread(&m_pCurWeapon, sizeof(tCurWeaponType), 1, _pFile);
	fread(&m_pPlayerLookDir, sizeof(tPlayerLookDir), 1, _pFile);
	fread(&m_pQKeyAttack, sizeof(tQKeyAttack), 1, _pFile);
	fread(&m_pHammerOverLoadState, sizeof(tHammerOverLoad), 1, _pFile);
	fread(&m_bDash, sizeof(bool), 1, _pFile);
	fread(&m_fDashTime, sizeof(float), 1, _pFile);
	fread(&m_bJump, sizeof(bool), 1, _pFile);
	fread(&m_bJumpFall, sizeof(bool), 1, _pFile);
	fread(&m_fJumpTime, sizeof(float), 1, _pFile);
	fread(&m_bQAttack, sizeof(bool), 1, _pFile);
	fread(&m_bHammerOverload, sizeof(bool), 1, _pFile);
	fread(&m_bOverLoadStart, sizeof(bool), 1, _pFile);
	fread(&m_bPlayerHit, sizeof(bool), 1, _pFile);
	fread(&m_fAttackTime, sizeof(float), 1, _pFile);
	fread(&m_fAttackColliderOn, sizeof(bool), 1, _pFile);
	fread(&m_bSwordEAttack, sizeof(bool), 1, _pFile);
	fread(&m_bGuardOn, sizeof(bool), 1, _pFile);
}