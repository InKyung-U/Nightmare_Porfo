#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:
    string  m_strPlayPause;

    bool    m_bPackaging;
    bool    m_bSceneSave;
    bool    m_bSceneLoad;
    bool    m_bScenePlayPause;
    bool    m_bSceneStop;

    bool    m_bTileMapUI;
    bool    m_bAnimationUI;

    string m_sScriptName;
    bool    m_bScript;

public:
    virtual void update() override;
    virtual void render() override;
    virtual void render_update() override;

private:
    void Task();

public:
    MenuUI();
    ~MenuUI();
};

