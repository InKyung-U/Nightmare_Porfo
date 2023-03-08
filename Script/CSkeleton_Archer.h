#pragma once
#include "CMonsterScript.h"
class CSkeleton_Archer :
    public CMonsterScript
{
private:
    float           m_fSpeed;
    float           m_fTraceRange;
    float           m_fAttackRange;
    float           m_fAttackCoolTime;
    UINT            m_iMonsterHP;
    tMonsterState   m_pMonsterState;
    tMonsterIdleState   m_pMonsterIdleState;
    bool            m_bStartFunc;
    bool            m_bDeadAnimationOn;
    float           m_fAttackTime;
    bool            m_bStunOn;

    Ptr<CSound>  m_pMonsterSound;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


public:
    void SetMonsterStateNonTrace() { m_pMonsterState = tMonsterState::MONSTER_NON_TRACE; }

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CSkeleton_Archer)

public:
    CSkeleton_Archer();
    CSkeleton_Archer(const CSkeleton_Archer& _Origin);
    ~CSkeleton_Archer();
};

