#pragma once
#include <Engine/CScript.h>
#include "CPlayerScript.h"

class CGameObject;

enum class tMonsterState
{
    MONSTER_TRACE, 
    MONSTER_ATTACK,
    MONSTER_FARAWAY_ATTACK,
    MONSTER_NON_TRACE,
    MONSTER_DEAD,
    MONSTER_STUN,
    MONSTER_HIT,
    MONSTER_END
};

enum class tMonsterIdleState
{
    MONSTER_RIGHT_RUN,
    MONSTER_IDLE,
    MONSTER_LEFT_RUN
};

enum class tMonsterRorL
{
    MONSTER_RIGHT,
    MONSTER_LEFT,
};

class CMonsterScript :
    public CScript
{
private:
    float           m_fSpeed;
    float           m_fTraceRange;
    float           m_fAttackRange;

    bool            m_bStartFunc;

    float           m_fAttackCoolTime;
    float           m_fAddCoolTime;
    float            m_iMonsterHP;
    tMonsterState   m_pMonsterState;
    tMonsterRorL   m_pMonsterRorL;
    tMonsterIdleState   m_pMonsterIdleState;
    float           m_fMonsterIdelTime;
    int             m_iMonsterIdel;
 
    CGameObject* m_pMonsterObject;
    Ptr<CSound>  m_pMonsterSound;

public:
    void SetMonsterSpeed(float _speed) { m_fSpeed = _speed; }
    void SetMonsterHP(UINT _hp) { m_iMonsterHP = _hp; }
    void SetMonsterState(tMonsterState _state) { m_pMonsterState = _state; }
    void SetTraceRange(float _range) { m_fTraceRange = _range; }
    void SetAttackRange(float _range) { m_fAttackRange = _range; }
    void SetAttackCoolTime(float _time) { m_fAttackCoolTime = _time; }
    void SetMonsterRorL(tMonsterRorL _state) { m_pMonsterRorL = _state; }
    void SetMonsterIdleStat(tMonsterIdleState _idle) { m_pMonsterIdleState = _idle; }

public:
    CGameObject* GetMonsterObject() { return m_pMonsterObject; }
    tMonsterState GetCurMonsterState() { return m_pMonsterState; }
    tMonsterIdleState GetMonsterIdleState() { return m_pMonsterIdleState; }
    tMonsterRorL GetMonsterRorL() { return m_pMonsterRorL; }
    float GetMonsterHP() { return ((float)m_iMonsterHP); }
public:
    void CreateMonsterAttackCollider(float _time, Vec3 _offsetPos, Vec3 _scale, Vec3 _rotate, float _attack);
    void CreateMonsterEffect(wstring _effectName, Vec3 _offsetPos, Vec3 _scale, float _yRotation);
    void CreateArrow(Vec3 _offsetPos);

    ContectWallDir MonsterContectWall(CGameObject* _wall);

    void CreateRewardsParticle(Vec3 _MonsterPos);

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CMonsterScript)

public:
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _origin);
    CMonsterScript(int _iScriptType);
    ~CMonsterScript();
};

