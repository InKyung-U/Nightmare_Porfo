#pragma once
#include <Engine/CScript.h>

class CWall :
    public CScript
{
private:
    bool m_bThrough;
    
public:
    bool GetThrough() { return m_bThrough; }

public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

    CLONE(CWall);
public:
    CWall();
    CWall(bool _through);
    ~CWall();
};

