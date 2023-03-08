#pragma once
#include <Engine/CScript.h>
class CSceneChangeScript :
    public CScript
{
public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CSceneChangeScript);
public:
    CSceneChangeScript();
    ~CSceneChangeScript();
};

