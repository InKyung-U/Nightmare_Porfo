#pragma once
#include <Engine/CScript.h>

class CParticleScript :
    public CScript
{
public:
    float  m_fLifeTime;

public:
    virtual void update();
    virtual void lateupdate();

    CLONE(CParticleScript)

public:
    CParticleScript();
    ~CParticleScript();
};

