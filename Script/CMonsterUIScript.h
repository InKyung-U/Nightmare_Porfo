#pragma once
#include <Engine/CScript.h>

class CMonsterUIScript :
    public CScript
{
private:
    Vec2 m_vStartPos;
    int m_iFullHP;
    float m_fOriginHpWidth;

    bool m_bStartFunc;

public:
    virtual void update();
    virtual void start();


    CLONE(CMonsterUIScript);
public:
    CMonsterUIScript();
    ~CMonsterUIScript();
};

