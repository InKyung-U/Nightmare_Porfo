#pragma once
#include <Engine/CScript.h>

class CScene_Boss :
    public CScript
{
private:
    CScene* m_pScene01;

public:
    virtual void update();
    CScene* GetScene() { return m_pScene01; }
    CLONE(CScene_Boss);

public:
    CScene_Boss();
    ~CScene_Boss();
};

