#include "pch.h"
#include "CMonsterScript.h"
#include "CMonster_Attack_Collider.h"
#include "CEffectScript.h"
#include "CMonsterArrow.h"
#include "CParticleScript.h"
#include "CMonsterUIScript.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>
#include <Engine/CParticleSystem.h>
#include <random>
#include <CPlayer_Weapon_Collider.h>

CMonsterScript::CMonsterScript()
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_fSpeed(0.f)
	, m_iMonsterHP(0)
	, m_fTraceRange(0)
	, m_fAttackRange(0)
	, m_fAddCoolTime(0.f)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_pMonsterObject(nullptr)
	, m_fAttackCoolTime(0.f)
	, m_fMonsterIdelTime(0.f)
	, m_iMonsterIdel(2)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_pMonsterSound(nullptr)
{
	m_pMonsterObject = GetOwner();
}

CMonsterScript::CMonsterScript(const CMonsterScript& _origin)
	: CScript(_origin)
	, m_fSpeed(_origin.m_fSpeed)
	, m_iMonsterHP(		(float)_origin.m_iMonsterHP)
	, m_fTraceRange(	_origin.m_fTraceRange)
	, m_fAttackRange(	_origin.m_fAttackRange)
	, m_fAddCoolTime(	_origin.m_fAddCoolTime)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_pMonsterObject(_origin.m_pMonsterObject)
	, m_fAttackCoolTime(0.f)
	, m_fMonsterIdelTime(0.f)
	, m_iMonsterIdel(_origin.m_iMonsterIdel)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_pMonsterRorL(tMonsterRorL::MONSTER_RIGHT)
	, m_bStartFunc(false)
	, m_pMonsterSound(nullptr)
{
}

CMonsterScript::CMonsterScript(int _iScriptType)
	: CScript(_iScriptType)
	, m_fSpeed(0.f)
	, m_iMonsterHP(0)
	, m_fTraceRange(0)
	, m_fAttackRange(0)
	, m_fAddCoolTime(0.f)
	, m_pMonsterState(tMonsterState::MONSTER_NON_TRACE)
	, m_pMonsterObject(nullptr)
	, m_fAttackCoolTime(0.f)
	, m_fMonsterIdelTime(0.f)
	, m_iMonsterIdel(2)
	, m_pMonsterIdleState(tMonsterIdleState::MONSTER_IDLE)
	, m_pMonsterRorL(tMonsterRorL::MONSTER_RIGHT)
	, m_bStartFunc(false)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	if (pScene != nullptr)
	{
		vector<CGameObject*> pvector = pScene->GetLayer(L"Monster")->GetRootObjects();
		if (pvector.size() != 0)
			m_pMonsterObject = CSceneMgr::GetInst()->FindObjectByName(L"Monster");
	}

	//m_pMonsterObject = GetOwner();
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::start()
{
	m_pMonsterObject = GetOwner();
	m_pMonsterState=tMonsterState::MONSTER_NON_TRACE;
	m_pMonsterIdleState = tMonsterIdleState::MONSTER_IDLE;
	m_bStartFunc = true;
}

void CMonsterScript::update()
{
	if (nullptr == m_pMonsterObject)
		start();
	if (!m_bStartFunc)
		start();
	static float HamSp = 0.f;
	if (CSceneMgr::GetInst()->IsPlayerUseHammerSp())
	{
		HamSp += DT;
		if (1.5 < HamSp)
		{
			if (m_pMonsterState != tMonsterState::MONSTER_DEAD)
				CreateMonsterEffect(L"Effect_Weapon_Hammer_SPAttack_Ground", Vec3(0.f, 0.f, 0.f), Vec3(200, 200, 1), 0);
			m_pMonsterState = tMonsterState::MONSTER_DEAD;
			CSceneMgr::GetInst()->GetCurScene()->SubMonsterCount();

		}
		return;
	}

	if (m_pMonsterState == tMonsterState::MONSTER_STUN)
	{
		if (m_pMonsterObject->Animator2D()->GetCurAnim()->IsFinish() == false)
		{
			return;
		}
	}


	Vec3 vpos = Transform()->GetRelativePos();
	Vec3 vRotate = Transform()->GetRelativeRotation();


	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CGameObject*> pvector = pScene->GetLayer(L"Player")->GetRootObjects();
	CGameObject* tPlayerObject;
	Vec3 playerpos;
	float monsterdistance = -1;
	if (pvector.size() != 0)
	{
		tPlayerObject = pvector[0];;
		playerpos = tPlayerObject->Transform()->GetRelativePos();
		// 플레이어와의 거리를 계산한다
		monsterdistance = playerpos.Distance(playerpos, vpos);
	}
	else
	{
		m_pMonsterState == tMonsterState::MONSTER_NON_TRACE;
	}

	
	//===========
	// COOL TIME
	//===========
	if (0 < m_fAddCoolTime)
		m_fAddCoolTime -= DT;


	//==============
	// Player State
	//==============
	if (m_pMonsterState == tMonsterState::MONSTER_ATTACK)
	{
		// 공격이 끝나면 다시 trace 로 상태변경 추가.
		// 공격중에는 몬스터 이동 x (보통) 아닌경우 알아서 구현
		//m_pMonsterState = tMonsterState::MONSTER_TRACE;
	}
	else if (m_pMonsterState == tMonsterState::MONSTER_FARAWAY_ATTACK)
	{
		
	}
	else if(m_pMonsterState == tMonsterState::MONSTER_TRACE)
	{
		if (vpos.x < playerpos.x)
		{
			vRotate.y = 0;
			vpos.x += DT * m_fSpeed;
			m_pMonsterRorL = tMonsterRorL::MONSTER_RIGHT;
		}
		else
		{
			vRotate.y = -3.141592;
			vpos.x -= DT * m_fSpeed;
			m_pMonsterRorL = tMonsterRorL::MONSTER_LEFT;
		}

		// 추적 범위에 들어오면 플레이어를 향해 원거리 공격을 우선적으로 한다.
		// 쿨타임을 비교하여 원거리 공격 상태로 바꾼다.
		// 그리고 쿨타임을 공격 상태로 전환할 때만 값을 올려준다.
		// 공격은 늘 trace 상태에서만 진행

		if (0 >= m_fAddCoolTime)
		{
			if (monsterdistance <= m_fAttackRange) // 근거리 공격
			{
				m_pMonsterState = tMonsterState::MONSTER_ATTACK;
				m_fAddCoolTime = m_fAttackCoolTime;
			}
			else // 원거리 공격
			{
				m_pMonsterState = tMonsterState::MONSTER_FARAWAY_ATTACK;
				m_fAddCoolTime = m_fAttackCoolTime;
			}
		}		
	}
	else if (m_pMonsterState == tMonsterState::MONSTER_NON_TRACE)
	{
		// 기본 이동
		if (2 == m_iMonsterIdel)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(-1, 1);
			m_iMonsterIdel = dis(gen);

			std::uniform_int_distribution<int> diis(0, 3);
			m_fMonsterIdelTime = diis(gen);
		}
		if (-1 == m_iMonsterIdel) // 왼쪽 이동
		{
			m_pMonsterIdleState = tMonsterIdleState::MONSTER_LEFT_RUN;
			m_fMonsterIdelTime -= DT;
			vpos.x -= DT * m_fSpeed/2;
			vRotate.y = -3.141592;
		}
		else if (0 == m_iMonsterIdel) // Idle
		{
			m_pMonsterIdleState = tMonsterIdleState::MONSTER_IDLE;
			m_fMonsterIdelTime -= DT;
		}
		else if (1 == m_iMonsterIdel) // 오른쪽 이동
		{
			m_pMonsterIdleState = tMonsterIdleState::MONSTER_RIGHT_RUN;
			m_fMonsterIdelTime -= DT;
			vpos.x += DT * m_fSpeed/2;
			vRotate.y = 0;
		}
		
		if (m_fMonsterIdelTime <= 0)
		{
			std::random_device rrd;
			std::mt19937 geen(rrd());
			std::uniform_int_distribution<int> dis(-1, 1);
			m_iMonsterIdel = dis(geen);

			std::uniform_int_distribution<int> diis(2, 4);
			m_fMonsterIdelTime = diis(geen);
		}

		if (monsterdistance > 0 && monsterdistance <= m_fTraceRange)
		{
			m_pMonsterState = tMonsterState::MONSTER_TRACE;
		}
	}


	Transform()->SetRelativePos(vpos);
	Transform()->SetRelativeRotation(vRotate);
}

void CMonsterScript::lateupdate()
{

}


void CMonsterScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CScript* script = _OtherObject->GetScript<CPlayer_Weapon_Collider>();
	if (script == nullptr)
		return;
	tCurWeaponType type = ((CPlayer_Weapon_Collider*)script)->GetWeaponType();
	Vec3 GetWeaponPos = ((CPlayer_Weapon_Collider*)script)->GetWeaponPos();

	Vec3 vHitEffectRotation = Transform()->GetRelativeRotation();
	Vec3 MonsterPos = Transform()->GetRelativePos();
	Vec3 effectSize = Vec3(300.f, 300.f, 1.f);
	if (type == tCurWeaponType::BOW)
	{
		m_pMonsterSound = CResMgr::GetInst()->Load<CSound>(L"sound\\SE_Enemy_Hit.wav", L"sound\\SE_Enemy_Hit.wav");
		m_pMonsterSound->Play(1, 0.3f, true);
		if (GetWeaponPos.x <= MonsterPos.x) // 무기 -> 몬스터
		{
			vHitEffectRotation.y = 0.f;
			CreateMonsterEffect(L"Effect_Weapon_Bow_Hit_Arrow", Vec3(-20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
			m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
		}
		else								// 몬스터 <- 무기
		{
			vHitEffectRotation.y = 3.141592f;
			CreateMonsterEffect(L"Effect_Weapon_Bow_Hit_Arrow", Vec3(20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
			m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
		}
	}
	else if (type == tCurWeaponType::HAMMER)
	{
		if (GetWeaponPos.x <= MonsterPos.x) // 무기 -> 몬스터
		{
			vHitEffectRotation.y = 0.f;
			CreateMonsterEffect(L"Effect_Weapon_Hammer_Hit", Vec3(-20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
			m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
		}
		else								// 몬스터 <- 무기
		{
			vHitEffectRotation.y = 3.141592f;
			CreateMonsterEffect(L"Effect_Weapon_Hammer_Hit", Vec3(20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
			m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
		}

	}
	else if (type == tCurWeaponType::SWORD)
	{
		if (((CPlayer_Weapon_Collider*)script)->GetSwordOverAttack())
		{
			if (GetWeaponPos.x <= MonsterPos.x) // 무기 -> 몬스터
			{
				vHitEffectRotation.y = 0.f;
				CreateMonsterEffect(L"Effect_Weapon_Sword_SPAttack_Hit", Vec3(-20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
				m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
			}
			else								// 몬스터 <- 무기
			{
				vHitEffectRotation.y = 3.141592f;
				CreateMonsterEffect(L"Effect_Weapon_Sword_SPAttack_Hit", Vec3(20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
				m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
			}
		}
		else
		{
			if (GetWeaponPos.x <= MonsterPos.x) // 무기 -> 몬스터
			{
				vHitEffectRotation.y = 0.f;
				CreateMonsterEffect(L"Effect_Weapon_Sword_Hit", Vec3(-20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
				m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
			}
			else								// 몬스터 <- 무기
			{
				vHitEffectRotation.y = 3.141592f;
				CreateMonsterEffect(L"Effect_Weapon_Sword_Hit", Vec3(20.f, 0.f, 0.f), effectSize, vHitEffectRotation.y);
				m_iMonsterHP -= _OtherObject->GetScript<CPlayer_Weapon_Collider>()->GetAttack();
			}
		}
		m_pMonsterState = tMonsterState::MONSTER_HIT;
	}
	if (0 >= m_iMonsterHP)
	{
		m_pMonsterState = tMonsterState::MONSTER_DEAD;
	}

}

void CMonsterScript::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fTraceRange, sizeof(float), 1, _pFile);	
	fwrite(&m_fAttackRange, sizeof(float), 1, _pFile);
	fwrite(&m_bStartFunc, sizeof(bool), 1, _pFile);
	fwrite(&m_fAttackCoolTime, sizeof(float), 1, _pFile);
	fwrite(&m_fAddCoolTime, sizeof(float), 1, _pFile);
	fwrite(&m_iMonsterHP, sizeof(UINT), 1, _pFile);
	fwrite(&m_pMonsterState, sizeof(tMonsterState), 1, _pFile);
	fwrite(&m_pMonsterRorL, sizeof(tMonsterRorL), 1, _pFile);
	fwrite(&m_pMonsterIdleState, sizeof(tMonsterIdleState), 1, _pFile);
	fwrite(&m_fMonsterIdelTime, sizeof(float), 1, _pFile);
	fwrite(&m_iMonsterIdel, sizeof(int), 1, _pFile);
	fwrite(&m_pMonsterObject, sizeof(CGameObject*), 1, _pFile);
}

void CMonsterScript::LoadFromScene(FILE* _pFile)
{
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_fTraceRange, sizeof(float), 1, _pFile);
	fread(&m_fAttackRange, sizeof(float), 1, _pFile);
	fread(&m_bStartFunc, sizeof(bool), 1, _pFile);
	fread(&m_fAttackCoolTime, sizeof(float), 1, _pFile);
	fread(&m_fAddCoolTime, sizeof(float), 1, _pFile);
	fread(&m_iMonsterHP, sizeof(UINT), 1, _pFile);
	fread(&m_pMonsterState, sizeof(tMonsterState), 1, _pFile);
	fread(&m_pMonsterRorL, sizeof(tMonsterRorL), 1, _pFile);
	fread(&m_pMonsterIdleState, sizeof(tMonsterIdleState), 1, _pFile);
	fread(&m_fMonsterIdelTime, sizeof(float), 1, _pFile);
	fread(&m_iMonsterIdel, sizeof(int), 1, _pFile);
	fread(&m_pMonsterObject, sizeof(CGameObject*), 1, _pFile);
}


void CMonsterScript::CreateMonsterAttackCollider(float _time, Vec3 _offsetPos, Vec3 _scale, Vec3 _rotate, float _attack)
{
	CGameObject* pColliderObject = new CGameObject;
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	Vec3 Scale = _scale;
	if (m_pMonsterRorL == tMonsterRorL::MONSTER_LEFT)
		vPlayerPos.x -= _offsetPos.x;
	else
		vPlayerPos.x += _offsetPos.x;

	vPlayerPos.y += _offsetPos.y;

	pColliderObject->AddComponent(new CMonster_Attack_Collider(_time, m_pMonsterObject, _attack));

	pColliderObject->AddComponent(new CTransform);
	pColliderObject->AddComponent(new CMeshRender);
	pColliderObject->AddComponent(new CCollider2D);

	pColliderObject->Transform()->SetRelativePos(vPlayerPos);
	pColliderObject->Transform()->SetRelativeScale(Scale);

	pColliderObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pColliderObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl"));

	pColliderObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pColliderObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pColliderObject->Collider2D()->SetOffsetScale(Vec2(Scale.x, Scale.y));
	CSceneMgr::GetInst()->SpawnObject(pColliderObject, vPlayerPos, L"MonsterAttackCollider", 5);
}

void CMonsterScript::CreateMonsterEffect(wstring _effectName, Vec3 _offsetPos, Vec3 _scale, float _yRotation)
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
	Vec2 backgroundSize = Vec2(400, 400);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pObject->Animator2D()->LoadAnimFromFile(_effectName, backgroundSize);
	pObject->Animator2D()->Play(_effectName, false);
	Vec3 vMissilePos = Vec3(playerpos.x + 50.f, playerpos.y, 200.f);

	pObject->AddComponent(new CEffectScript(_effectName));
	CSceneMgr::GetInst()->GetCurScene()->AddObject(pObject, L"Effect");
}

void CMonsterScript::CreateArrow(Vec3 _offsetPos)
{
	CResMgr::GetInst()->Load<CTexture>(L"texture\\Enemy_SkeletonArcher_Object_Arrow.png", L"texture\\Enemy_SkeletonArcher_Object_Arrow.png");

	Vec3 vPlayerPos = Transform()->GetRelativePos();

	if (GetOwner()->GetScript<CMonsterScript>()->GetMonsterRorL() == tMonsterRorL::MONSTER_LEFT)
	{
		vPlayerPos.x -= _offsetPos.x;
	}
	else
	{
		vPlayerPos.x += _offsetPos.x;
	}

	vPlayerPos.y += _offsetPos.y;
	vPlayerPos.z = 100.f;
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Arrow");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMonster_Attack_Collider(5.f, m_pMonsterObject, 7.f));
	pObject->AddComponent(new CMonsterArrow(m_pMonsterRorL));
	Vec3 playerpos = Transform()->GetRelativePos();
	pObject->Transform()->SetRelativePos(vPlayerPos);
	pObject->Transform()->SetRelativeScale(Vec3(50.f, 10.f, 1.f));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(50.f, 10.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\Enemy_SkeletonArcher_Object_Arrow.png", L"texture\\Enemy_SkeletonArcher_Object_Arrow.png"));

	CSceneMgr::GetInst()->GetCurScene()->AddObject(pObject, L"MonsterAttack");
}

ContectWallDir CMonsterScript::MonsterContectWall(CGameObject* _wall)
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
	}

	return walldir;
}

void CMonsterScript::CreateRewardsParticle(Vec3 _MonsterPos)
{
	// particle 1
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject_01");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);
	pParticleObj->AddComponent(new CParticleScript);

	pParticleObj->Transform()->SetRelativePos(_MonsterPos);

	Ptr<CTexture> pParticleTex1 = CResMgr::GetInst()->Load<CTexture>(L"texture\\popupItem\\PopItem_Gold.png", L"texture\\popupItem\\PopItem_Gold.png");
	pParticleObj->ParticleSystem()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex1);

	CSceneMgr::GetInst()->GetCurScene()->AddObject(pParticleObj, L"Default");


}
