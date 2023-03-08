#pragma once
#include <Engine/CScript.h>

class CEffectScript :
    public CScript
{
private:
    wstring m_sEffectName;
    CGameObject* m_pScriptObject;

    float   m_fLifeTime;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CEffectScript)

public:
    CEffectScript();
    CEffectScript(wstring _effectName);
    CEffectScript(wstring _effectName, float _lifeTime);
    ~CEffectScript();
};

