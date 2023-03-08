#pragma once
#include "CMonsterScript.h"

enum class BossStartMotion
{
    S_START1,   // ������� ���� ���� -> AIR ATTACK ���� ����
    S_START2,   // �������� ���� ���� -> LAND ATTACK ���� ����
    S_DONE,

};

enum class BossAttackMotion
{
    A_AIRATTACK1,   // �Ϲ� �������
    A_AIRATTACK2,   // ������ �߾ӿ��� �������
    A_AIRATTACK3,   // �� 50�� ���� �߾ӿ��� ������� ���� ���극��
    A_LANDATTACK1,  // ������ ��Ÿ���� �극��
    A_LANDATTACK2,  // ������ ��Ÿ���� ����
    A_LANDATTACK3,  // �� 50�� ���� ������ ��Ÿ���� ���� + �׸��� ����
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

