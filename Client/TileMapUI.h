#pragma once
#include "UI.h"

class CTexture;

struct TileInfo
{
    ImTextureID  pTex;
    ImVec2       LTPos;
    ImVec2       RBPos;
    int            TileIdx;
    Vec2	vLTUV;
    int          index;
};

struct BoxPos
{
    Vec2    pLTpos;
    Vec2    pRBpos;
};

class TileMapUI :
    public UI
{
private:
    CTexture*           m_pTileAtlas;
    vector<TileInfo>    m_pTileVec;
    vector<tTileData>   m_vecTileData;
    int                 m_iTileIndex;
    CTileMap*            m_pTargetTileMap;
    CTileMap*           m_pMakingTileMap;
    bool                 m_bTileDataSave;
    float            m_vTileSize;

    bool                m_bMakeThrouhColliderBox;
    bool                m_bMakeNormalColliderBox;
    vector<BoxPos>      m_vNormalTileCollider;
    vector<BoxPos>      m_vThroughTileCollider;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void TileInfoTotTileData(vector<TileInfo> _info, vector<tTileData> _data);
    void tTileDataToTileInfo(vector<tTileData> _data, vector<TileInfo> _info);


public:
    CTexture* GetAtlas() { return m_pTileAtlas; }
    void SetTileIndex(int _index);

    void clear();

public:
    TileMapUI();
    ~TileMapUI();
};

