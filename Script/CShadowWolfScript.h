#pragma once
#include <Engine/CScript.h>

#include "CMonsterScript.h"

class CShadowWolfScript :
    public CScript
{
private :
    Vec3 m_vStartPos;
    tMonsterRorL m_pRorL;
    float m_fAttackTime;
    float m_fRotate;

public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CShadowWolfScript);
public:
    CShadowWolfScript(Vec3 _startPos, tMonsterRorL _state, float _rotateY);
    CShadowWolfScript();
    ~CShadowWolfScript();
};

