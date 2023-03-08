#pragma once
#include "UI.h"

class CTexture;
class CAnimator2D;
class CAnimation2D;

class AnimationUI :
    public UI
{
public:
    bool* b_mAnimTool;
    CTexture* m_Atlas;
    CTexture* m_pLoadAtlas;
    wstring      m_sAtlasName;
    UINT    i_curFrameCount;
    Vec2    v_mAnimBackGroundSize;
    bool    b_PlayAnimation;
    float   f_AnimAddTime;
    string  m_sAnimationName;

    bool    m_bAnimationRepeat;

    bool    m_bSaveAnimation;
    bool    m_bLoadAnimation;

    bool  m_bBoxOnlyMove;

    CAnimator2D* m_pAnimator;

    ID3D11ShaderResourceView* m_texture;

    vector<AnimToolData> v_mAnimationData;

    ImVec2 rect1 = ImVec2(200, 200);
    ImVec2 rect2 = ImVec2(400, 400);

public:
    virtual void update() override;
    virtual void render_update() override;

    void Task();

public:
    void ReverseAnimTool() { b_mAnimTool = (bool*)!b_mAnimTool; }
    void AddAnimationVector(Vec2 _BGSize, Vec2 _LTPos, Vec2 _imgScale, float _duration, UINT _maxFrame, UINT _curFrame);
    void ShowAnimationFrame(UINT _width, UINT _height, ID3D11ShaderResourceView* _tex);
    void BoxMovingCheck(int _width, int _height, ImVec2 _pos);

    void tAnim2DFrameTransAnimToolData(tAnim2DFrame _frm, AnimToolData& _data);

public:
    void AtlasSelect(DWORD_PTR _param);
    void AnimationSelect(DWORD_PTR _param);


public:
    AnimationUI();
    ~AnimationUI();

};

