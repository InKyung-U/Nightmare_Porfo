#pragma once

#include <Engine/CScript.h>
#include <CMonsterScript.h>


class CMonsterArrow
	:public CScript
{
public:
    float           m_fSpeed;
    float           m_fLifeTime;
    tMonsterRorL        m_pState;

public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CMonsterArrow);
public:
    CMonsterArrow();
    CMonsterArrow(tMonsterRorL _RorL);
    ~CMonsterArrow();
};

