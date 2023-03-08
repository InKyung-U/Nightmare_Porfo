#pragma once
#include <Engine/CScript.h>

class CPlayerUIScript :
    public CScript
{
private:
    Vec2 m_vStartPos;
    int m_iFullHP;
    float m_fOriginHpWidth;
    CGameObject* m_pPlayer;

    bool m_bStartFunc;

public:
    virtual void update();
    virtual void start();

    CLONE(CPlayerUIScript);
public:
    CPlayerUIScript();
    ~CPlayerUIScript();
};

