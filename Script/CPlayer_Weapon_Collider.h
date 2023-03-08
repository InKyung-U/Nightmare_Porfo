#pragma once
#include <Engine/CScript.h>
#include <CPlayerScript.h>

class CPlayer_Weapon_Collider
	: public CScript
{
private:
    float   m_fLiveTime;
    tCurWeaponType m_pCurWeapon;
    int     m_iAttack;
    bool m_bHammerSpAttack;
    bool m_bSwordOverAttack;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

public:
    void SetHammerSpAttack(bool _OnOff) { m_bHammerSpAttack = _OnOff; }
    void SetSwordOverAttack(bool _OnOff) { m_bSwordOverAttack = _OnOff; }
    bool GetHammerSpAttack() { return m_bHammerSpAttack; }
    bool GetSwordOverAttack() { return m_bSwordOverAttack; }
    int GetAttack() { return m_iAttack; }

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    tCurWeaponType GetWeaponType() { return m_pCurWeapon; }
    Vec3 GetWeaponPos() 
    {
        Vec3 pos = Transform()->GetRelativePos();
        return pos;
    }

    CLONE(CPlayer_Weapon_Collider)

public:
    CPlayer_Weapon_Collider();
    CPlayer_Weapon_Collider(float _liveTime, tCurWeaponType _type, int _attack);
    CPlayer_Weapon_Collider(float _liveTime, tCurWeaponType _type, bool _swordEon, bool _hammerQ, int _attack);
    ~CPlayer_Weapon_Collider();
};

