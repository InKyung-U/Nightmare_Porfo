#pragma once
#include <Engine/CScript.h>
class CNotMoveScript :
    public CScript
{
public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

    CLONE(CNotMoveScript);
public:
    CNotMoveScript();
    ~CNotMoveScript();
};

