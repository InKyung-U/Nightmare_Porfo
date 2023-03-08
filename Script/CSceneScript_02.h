#pragma once
#include <Engine/CScript.h>
class CSceneScript_02 :
    public CScript
{
private:
    CScene* m_pScene01;

    CGameObject* mainCam;
    bool    m_pStartFunc;

public:
    virtual void update();
    virtual void start();

    CScene* GetScene() { return m_pScene01; }
    CLONE(CSceneScript_02);

public:
    CSceneScript_02();
    ~CSceneScript_02();
};

