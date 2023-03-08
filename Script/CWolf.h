#pragma once
#include "CMonsterScript.h"
class CWolf :
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
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CWolf)

public:
    CWolf();
    CWolf(const CWolf& _origin);
    ~CWolf();
};

