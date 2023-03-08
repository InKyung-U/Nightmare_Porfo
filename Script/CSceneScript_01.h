#pragma once
#include <Engine/CScript.h>

class CSceneScript_01 :
    public CScript
{
private:
    CScene* m_pScene01;
    CGameObject* m_pPlayer;

public:
    virtual void update();

    CScene* GetScene() { return m_pScene01; }

    CLONE(CSceneScript_01);
public:
    CSceneScript_01();
    ~CSceneScript_01();
};

