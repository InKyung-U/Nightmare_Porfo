#pragma once
#include <Engine/CScript.h>
class CGuardScript :
    public CScript
{
private:
    CGameObject* m_pPlayer;

public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CGuardScript);
public:
    CGuardScript();
    CGuardScript(CGameObject* _playerobj);
    ~CGuardScript();
};

