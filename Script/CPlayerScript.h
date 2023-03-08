#pragma once

#include <Engine/CScript.h>

class CGameObject;

enum tPlayerLookDir
{
    RIGHT,
    LEFT
};

enum tQKeyAttack
{
    Q_KEY_NONE,
    Q_KEY_SWORD,
    Q_KEY_HAMMER,
    Q_KEY_BOW
};

enum tHammerOverLoad
{
    HAM_OVER_NONE,
    HAM_OVER_START,
    HAM_OVER_DOING,
    HAM_OVER_END
};

enum class ContectWallDir
{
    RIGHT,
    LEFT,
    UP,
    DOWN,
    NON_DIR
};

class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;
    int             m_fHP;
    int             m_iFullHP;
    float           m_fBurnStrength;
    bool            m_bBurn;

    CGameObject* m_pPlayerObject;

    tCurWeaponType m_pCurWeapon;
    tPlayerLookDir  m_pPlayerLookDir;
    tQKeyAttack     m_pQKeyAttack;
    tHammerOverLoad m_pHammerOverLoadState;

    bool        m_bDash;
    float       m_fDashTime;
    bool        m_bJump;
    bool        m_bJumpFall;
    float       m_fJumpTime;
    bool        m_bQAttack;
    bool        m_bHammerOverload;
    bool        m_bOverLoadStart;
    bool        m_bPlayerHit;

    float       m_fAttackTime;
    bool        m_fAttackColliderOn;

    bool        m_bSwordEAttack;
    bool        m_bGuardOn;
    bool        m_bContectWallR;
    bool        m_bContectWallL;

    bool        m_bPlayerDie;

    CGameObject* m_pUIFrame;
    bool stageChangeOn;

    Ptr<CSound> m_pPlayerSound;
public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

private:
    void CreateWeaponCollider(float _time, Vec3 _offsetPos,  Vec3 _scale, tCurWeaponType _type, bool _swordE, bool _hammerQ, bool _guard = false);
    void CreatePlayerEffect(wstring _effectName, Vec3 _offsetPos, Vec3 _scale, bool _repeat);
    void CreateArrow(Vec3 _offsetPos, bool _reflect);

    void ChangeState();

public:
    void SetGuardOn(bool _guard) { m_bGuardOn = _guard; }

public:
    tCurWeaponType GetPlayerCurWeaponType() { return m_pCurWeapon; }
    ContectWallDir ContectWall(CGameObject* _wall);
    int GetHP() { return m_fHP; }
    int GetFullHP() { return m_iFullHP; }

private:
    void Burnning();
    void ChangeAnim(wstring _default, wstring _sword, wstring _bow, wstring _hammer, bool _repeat);
   

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CPlayerScript)

public:
    CPlayerScript();
    CPlayerScript(const CPlayerScript& _Origin);
    ~CPlayerScript();

};

