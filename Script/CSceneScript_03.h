#pragma once
#include <Engine/CScript.h>
class CSceneScript_03 :
    public CScript
{
private:
    CScene* m_pScene01;

public:
    virtual void update();
    CScene* GetScene() { return m_pScene01; }
    CLONE(CSceneScript_03);

public:
    CSceneScript_03();
    ~CSceneScript_03();
};

