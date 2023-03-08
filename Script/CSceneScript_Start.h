#pragma once
#include <Engine/CScript.h>

class CSceneScript_Start
    :public CScript
{
private:
    CScene* m_pScene01;

public:
    virtual void update();
    CScene* GetScene() { return m_pScene01; }
    CLONE(CSceneScript_Start);

public:
    CSceneScript_Start();
    ~CSceneScript_Start();
};

