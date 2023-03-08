#pragma once

class CScene;
class CGameObject;

class CSceneMgr
	: public CSingleton<CSceneMgr>
{	
	friend class CSingleton<CSceneMgr>;
private:
	CSceneMgr();
	~CSceneMgr();

private:
	CScene*				m_pCurScene;
	int					m_iCurSceneNum;
	bool				m_pPlayerUseHammerSP;
	Ptr<CSound> m_pBackgroudSound;

	bool collider;

	CScene* m_pReadyScene;
	float  m_pPlayerHp;
	tCurWeaponType m_pWeaponType;

public:
	CScene* GetCurScene() { return m_pCurScene; }
	void ChangeScene(CScene* _pNextScene);
	CScene* GetReadyScene() { return m_pReadyScene; }
	void SetReadyScene(CScene* _scene) { m_pReadyScene = _scene; m_pPlayerUseHammerSP = false; }

	Ptr<CSound> GetBGM() { return m_pBackgroudSound; }
	void SetBGM(Ptr<CSound> _sound) { m_pBackgroudSound = _sound; }

	void SetColliderRender(bool _TF) { collider = _TF; }
	bool GetColliderRender() { return collider; }

	void SetHp(float _hp) { m_pPlayerHp = _hp; }
	float GetHp() { return m_pPlayerHp; }
	void SetWeaponType(tCurWeaponType _type) { m_pWeaponType = _type; }
	tCurWeaponType GetWeaponType() { return m_pWeaponType; }


	void ChangeSceneNumber(int _num) { m_iCurSceneNum = _num; }
	int GetCurSceneNumber() { return m_iCurSceneNum; }

	void SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx);
	void SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx);
	void AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject);	
	void DisconnectParent(CGameObject* _pObject);
	void ClearLayer();

	CGameObject* FindObjectByName(const wstring& _strName);
	void FindObjectsByName(wstring& _strName, vector<CGameObject*>& _vecOut);

	void SetPlayerUseHammerSP(bool _HamSP) { m_pPlayerUseHammerSP = _HamSP; }
	bool IsPlayerUseHammerSp() { return m_pPlayerUseHammerSP; }

public:
	void init();
	void progress();	

};

