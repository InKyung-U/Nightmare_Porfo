#pragma once
#include "UI.h"

typedef void (UI::* DBCLKED)(DWORD_PTR _param);

class CTexture;

class TileSelectUI :
    public UI
{
private:
    vector<string>  m_vecList;
    CTexture* m_pTileAtlas;

    ImVec2  IndexPos;
    int     m_iTileIndex;

    // 더블클릭 이벤트   
    UI* m_Inst;
    DBCLKED         m_DBCEvent;

public:
    void Clear() { m_vecList.clear(); }

    void SetDBCEvent(UI* _pUI, DBCLKED _func)
    {
        m_Inst = _pUI;
        m_DBCEvent = _func;
    }

public:
    virtual void update() override;
    virtual void render_update() override;
    virtual void Activate() override;

public:
    TileSelectUI();
    ~TileSelectUI();
};

