#pragma once
#include "CComponent.h"

#include "CTexture.h"
#include "CAnimation2D.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;
    bool                        m_bRepeat;

public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    static void Clear();

public:
    CAnimation2D* FindAnim(const wstring& _strName);
    void CreateAnim(const wstring& _strName, Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel, Vec2 _vLeftTopPixel, Vec2 _vSlicePixel, Vec2 _vStepPixel, float _fDuration, int _iFrameCount);
    void Play(const wstring& _strName, bool _bRepeat);

    void CreateAnim(const wstring& _strName, Ptr<CTexture> _pAtlas, vector<AnimToolData> _vec);

    void SetAccTimeZero() { m_pCurAnim->SetAccTimeZero(); }

public:
    CAnimation2D* GetCurAnim() { return m_pCurAnim; }
    map<wstring, CAnimation2D*> GetMapAnim() { return m_mapAnim; }
    size_t GetmapAnimSize() { return m_mapAnim.size(); }

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CAnimator2D)

public:
    void SaveAnimFile(FILE* _pFile);
    void LoadAnimFile(FILE* _pFile);

    void LoadAnimFromFile(wstring _strAnimName, Vec2 _backgroundSize);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};

