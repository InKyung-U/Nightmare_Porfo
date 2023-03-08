#pragma once
#include <Engine/CScript.h>

class CMonster_Attack_Collider :
    public CScript
{
private:
    float   m_fLiveTime;
    float   m_fAttack;
    CGameObject* m_pOwnerMonster;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    CGameObject* GetOwnerMonster() { return m_pOwnerMonster; }
    float GetAttackNum() { return m_fAttack; }

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CMonster_Attack_Collider)

public:
    CMonster_Attack_Collider();
    CMonster_Attack_Collider(float _liveTime, CGameObject* _ownerMonster, float _attack);
    ~CMonster_Attack_Collider();
};

