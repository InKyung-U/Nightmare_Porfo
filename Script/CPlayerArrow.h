#pragma once
#include <Engine/CScript.h>

#include <CPlayerScript.h>

class CPlayerArrow :
    public CScript
{
private:
    float       m_fSpeed;
    float       m_fAccTime;
    tPlayerLookDir m_pPlayerDir;
    Vec3        m_vDir;
    float       m_fAngle;
public:
    Vec3 GetWeaponPos() { return Transform()->GetRelativePos(); }
    Vec3 GetWeaponDir() { return m_vDir; }

    void SetArrowDir(Vec3 _dir) { m_vDir = _dir; }
    Vec3 Reflect(Vec3 _dir, CGameObject* _pOtherObj);
public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CPlayerArrow);
public:
    CPlayerArrow(tPlayerLookDir _dir);
    CPlayerArrow();
    ~CPlayerArrow();
};

