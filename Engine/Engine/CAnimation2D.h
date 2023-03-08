#pragma once

#include "CEntity.h"

#include "CTexture.h"

class CAnimator2D;

class CAnimation2D
	: public CEntity
{
private:
	vector<tAnim2DFrame>	m_vecFrm;
	Ptr<CTexture>			m_pAtlasTex;
	Vec2					m_vBackgroundSize;

	CAnimator2D*			m_pOwner;
	int						m_iCurFrmIdx;
	float					m_fAccTime;
	bool					m_bFinish;


public:
	tAnim2DFrame& GetFrame(int _iFrmIdx) { return m_vecFrm[_iFrmIdx]; }
	vector<tAnim2DFrame> GetvecFrm() { return m_vecFrm; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

	void SetBackGroundSize(Vec2 _size) { m_vBackgroundSize = _size; }
	void SetAccTimeZero() { m_fAccTime = 0.f; m_iCurFrmIdx = 0; }

public:
	void finalupdate();

	virtual void UpdateData() override;

public:
	vector<tAnim2DFrame> GetVecFrm() { return m_vecFrm; }
	Vec2 GetBackGroundSize() { return m_vBackgroundSize; }
	Ptr<CTexture> GetAtlas() { return m_pAtlasTex; }

public:
	void Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep, float _fDuration, int _iFrameCount);
	void Create(Ptr<CTexture> _Atlas, vector<AnimToolData> _vec);
	void Create(Ptr<CTexture> _Atlas, CAnimation2D* _anim);

	bool IsFinish() { return m_bFinish; }
	void Reset()
	{
		m_fAccTime = 0.f;
		m_bFinish = false;
		m_iCurFrmIdx = 0;			
	}


public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;
	CLONE(CAnimation2D)

public:
	void SaveAnimFile(FILE* _pFile);
	void LoadAnimFile(FILE* _pFile);

public:
	CAnimation2D();
	~CAnimation2D();

	friend class CAnimator2D;
};