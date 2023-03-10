#pragma once
#include "CRenderComponent.h"

#include "CTexture.h"
#include "CStructuredBuffer.h"

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>       m_pAtlasTex;
    Vec2                m_vSlicePixel;
    Vec2                m_vSliceUV;

    UINT                m_iRowCount;
    UINT                m_iColCount;   

    UINT                m_iTileCountX;
    UINT                m_iTileCountY;
    vector<tTileData>   m_vecTileData;
    CStructuredBuffer*  m_pBuffer;

    bool                m_bBufferUpdated;


public:
    void SetAtlasTex(Ptr<CTexture> _pAtlasTex) 
    { 
        m_pAtlasTex = _pAtlasTex; 
        m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileSize(Vec2 _vPixelSize) 
    {  
        m_vSlicePixel = _vPixelSize; 
        if(nullptr != m_pAtlasTex)
            m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileMapCount(UINT _iCountX, UINT _iCountY);
    void SetTileData(int _iTileIdx, int _iImgIdx);
    void SetSlicePixel(Vec2 _pixel) { m_vSlicePixel = _pixel; }
    void SetSliceUV(Vec2 _uv) { m_vSliceUV = _uv; }
    void SetRowColCount(UINT _row, UINT _col) { m_iRowCount = _row; m_iColCount = _col; }
    void SetTileDataVector(vector<tTileData> _vec) {
        m_vecTileData = _vec; 
        int i = 0;
    }

    CTexture* GetAtlas() { return m_pAtlasTex.Get(); }

public:
    vector<tTileData> GetTileData() { return m_vecTileData; }
    Vec2 GetTileCount() { return Vec2(m_iTileCountX, m_iTileCountY); }

    void ClearTileData();


public:
    virtual void finalupdate() override;
    virtual void render() override;
    virtual void UpdateData() override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CTileMap)
public:
    CTileMap();
    CTileMap(const CTileMap& _origin);
    ~CTileMap();
};

