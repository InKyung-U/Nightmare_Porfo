#pragma once
#include "CMonsterScript.h"

enum class BossStartMotion
{
    S_START1,   // 내려찍기 공격 연계 -> AIR ATTACK 으로 연계
    S_START2,   // 구석등장 공격 연계 -> LAND ATTACK 으로 연계
    S_DONE,

};

enum class BossAttackMotion
{
    A_AIRATTACK1,   // 일반 내려찍기
    A_AIRATTACK2,   // 무조건 중앙에서 내려찍기
    A_AIRATTACK3,   // 피 50퍼 이하 중앙에서 내려찍기 이후 독브레스
    A_LANDATTACK1,  // 벽에서 나타나서 브레스
    A_LANDATTACK2,  // 벽에서 나타나서 돌진
    A_LANDATTACK3,  // 피 50퍼 이하 벽에서 나타나서 돌진 + 그림자 돌진
    A_DONE,
};

enum MotionState
{
    MOTION_BEFORE_START,
    MOTION_DURING,
    MOTION_END
};

struct CurBossMotion
{
    BossStartMotion StartMotion;
    MotionState     StartState;
    BossAttackMotion AttackMotion;
    MotionState     AttackState;
};

class CBoss_Wolf :
    public CMonsterScript
{
private:
    float           m_fSpeed;
    float           m_fTraceRange;
    float           m_fAttackRange;
    float           m_fAttackCoolTime;
    UINT            m_iMonsterHP;
    UINT            m_iMonsterHP_Fix;
    tMonsterState   m_pMonsterState;
    tMonsterIdleState   m_pMonsterIdleState;
    bool            m_bStartFunc;

    bool            m_bDuringAttack;
    bool            m_bBossDisappear;
    bool            m_bBossFlip;

    bool            m_bDeadAnimationOn;
    float           m_fAttackTime;
    bool            m_bStunOn;

    BossStartMotion     m_pStartMotion;
    BossAttackMotion    m_pAttackMotion;
    CurBossMotion       m_pMotionState;

    bool    m_bHowlingOn;
    UINT    m_iHowlingCount;
    bool    m_bHowlingEndOn;
    bool    m_bBreathOn;
    bool    m_bDashOn;
    bool    m_bShadowDashOn;
    Vec3    m_vShadowStartPos;

    bool m_bBreath1;
    bool m_bBreath2;

    CGameObject* m_pMonsterObject;
    Ptr<CSound>  m_pMonsterSound;

public:
    void SetAttackMotion();
    void AttackEnd();

public:
    float GetBossMonsterHP() { return (float)m_iMonsterHP; }
    void SetBossMonsterHP(float _HP) { m_iMonsterHP = (UINT)_HP; }

public:
    void CreateBossMonsterEffect(wstring _effectName, Vec3 _offsetPos, Vec3 _scale, float _yRotation, float _time);
    void CreateShadow();

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CBoss_Wolf)

public:
    CBoss_Wolf();
    CBoss_Wolf(const CBoss_Wolf& _Origin);
    ~CBoss_Wolf();
};

