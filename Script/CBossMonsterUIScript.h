#pragma once
#include <Engine/CScript.h>

class CBossMonsterUIScript :
    public CScript
{
private:
    Vec2 m_vStartPos;
    int m_iFullHP;
    float m_fOriginHpWidth;
    CGameObject* m_pBoss;
    bool m_bStartFunc;

public:
    virtual void update();
    virtual void start();


    CLONE(CBossMonsterUIScript);
public:
    CBossMonsterUIScript();
    ~CBossMonsterUIScript();
};

