#include "pch.h"
#include "TileMapUI.h"

#include "TileSelectUI.h"
#include "CImGuiMgr.h"

#include <Engine/CTexture.h>
#include  <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTileMap.h>
#include <Engine/CTransform.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CComponent.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>

#include <Script/CWall.h>


TileMapUI::TileMapUI()
    : UI("##TileMapUI")
    , m_pTileAtlas(nullptr)
    , m_iTileIndex(0)
    , m_pTileVec{}
    , m_bMakeThrouhColliderBox(false)
	, m_bMakeNormalColliderBox(false)
    , m_vNormalTileCollider()
    , m_vThroughTileCollider()
    , m_bTileDataSave(true)
    , m_vTileSize(0.f)
    , m_pMakingTileMap(nullptr)
{

}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::update()
{
    // To Do


    UI::update();
}

void TileMapUI::render_update()
{
    
    //m_pTileVec.clear();
    m_pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\Tileset_1.png", L"texture\\Tileset_1.png").Get();
    static int pXCount = 0;
    static int pYCount = 0;

    CGameObject* pTileMapObject = CSceneMgr::GetInst()->FindObjectByName(L"TileMap");
    CComponent* comp = pTileMapObject->GetComponent(COMPONENT_TYPE::TILEMAP);
    Vec2 pTileCount = ((CTileMap*)comp)->GetTileCount();
    vector<tTileData> tileData = ((CTileMap*)comp)->GetTileData();
    //m_vecTileData.resize((size_t)(pTileCount.x * pTileCount.y));
    //m_pTileVec.resize((size_t)(pTileCount.x * pTileCount.y));
    if (pTileCount != Vec2(0, 0))
    {
        pXCount = pTileCount.x;
        pYCount = pTileCount.y;
        m_vecTileData.resize((size_t)(pXCount * pYCount));

    }

    static float pTileSize = 32.f;
    m_vTileSize = pTileSize;
    static char pTileName[255] = {};
    ImGui::PushItemWidth(150);
    ImGui::Text("Tile Name"); ImGui::SameLine(100); ImGui::InputText("##TileName", pTileName, 255, 0, 0, 0); ImGui::SameLine();
    ImGui::Text("Tile size"); ImGui::SameLine(); ImGui::InputFloat("##TileSize", &pTileSize, 0);
    ImGui::Text("Tile X Count"); ImGui::SameLine(100); ImGui::InputInt("##TileXCount", &pXCount, 0); ImGui::SameLine();
    ImGui::Text("Tile Y Count"); ImGui::SameLine(); ImGui::InputInt("##TileYCount", &pYCount, 0);

    ImGui::Text("Set Current Tile"); ImGui::SameLine();
    int AtlasxCount = m_pTileAtlas->Width() / pTileSize;
    int AtlasyCount = m_pTileAtlas->Height() / pTileSize;

    ImVec2 pTilePos = ImVec2((m_iTileIndex - 1) % AtlasxCount * pTileSize, (m_iTileIndex - 1) / AtlasxCount * pTileSize);

    ImVec2 uv0 = ImVec2(pTilePos.x / m_pTileAtlas->Width(), pTilePos.y / m_pTileAtlas->Height());
    ImVec2 uv1 = ImVec2((pTilePos.x + pTileSize) / m_pTileAtlas->Width(), (pTilePos.y + pTileSize) / m_pTileAtlas->Height());
    ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
    if (ImGui::ImageButton(m_pTileAtlas->GetSRV().Get(), ImVec2(pTileSize, pTileSize), uv0, uv1, 2, bg_col, tint_col))
    {
        // TileSelectUI 활성화한다.

        TileSelectUI* pTileSelectUI = (TileSelectUI*)CImGuiMgr::GetInst()->FindUI("##TileSelectUI");
        pTileSelectUI->Clear();
        pTileSelectUI->SetTitle("Tile Select");

        pTileSelectUI->Activate();
        pTileSelectUI->SetDBCEvent(this, (DBCLKED)&TileMapUI::SetTileIndex);
    }
    //if (ImGui::Button("Get Current Tile"))
    //{
    //    m_pTileVec.clear();
    //    // 화면에서 타일 데이터를 받아온다. 씬매니저에서?
    //    CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
    //    CLayer* layer = pScene->GetLayer(L"Tile");
    //    vector<CGameObject*> object = layer->GetObjects();
    //    CComponent* com = object[0]->GetComponent(COMPONENT_TYPE::TILEMAP);
    //    m_pTargetTileMap = com->TileMap();
    //    
    //    if (m_pTileAtlas != m_pTargetTileMap->GetAtlas())
    //    {
    //        m_pTileAtlas = m_pTargetTileMap->GetAtlas();
    //    }
    //    vector<tTileData> data = m_pTargetTileMap->GetTileData();
    //    tTileDataToTileInfo(data, m_pTileVec);
    //
    //    m_vecTileData = data;
    //    int i = 0;
    //}
    if (ImGui::Button("Apply"))
    {
        CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
        CLayer* layer = pScene->GetLayer(L"Tile");
        vector<CGameObject*> object = layer->GetObjects();
        CComponent* com = object[0]->GetComponent(COMPONENT_TYPE::TILEMAP);
        m_pTargetTileMap = com->TileMap();
        //CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Tile")->GetObjects()[0]->GetComponent(COMPONENT_TYPE::TILEMAP)->TileMap()->SetTileDataVector(m_vecTileData);
        
        // tile data
        string name(pTileName);
        wstring wname;
        wname.assign(name.begin(), name.end());
        
        CGameObject* pObject = new CGameObject;
        if(wname == L"")
            pObject->SetName(L"TileMap_02");
        else
            pObject->SetName(wname);
        pObject->AddComponent(new CTransform);
        pObject->AddComponent(new CTileMap);
        pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
        pObject->Transform()->SetRelativeScale(1000, 1000, 1.f);
        
        Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\Tileset_1.png", L"texture\\Tileset_1.png");
        pObject->TileMap()->SetAtlasTex(pTileAtlas);
        pObject->TileMap()->SetSlicePixel(Vec2(32, 32));
        pObject->TileMap()->SetTileSize(Vec2(32.f, 32.f));
        pObject->TileMap()->SetTileMapCount(pXCount, pYCount);
        pObject->TileMap()->SetTileDataVector(m_vecTileData);
        pScene->AddObject(pObject, L"Tile");

        for (size_t i = 0; i < m_vNormalTileCollider.size(); i++)
        {
            Vec2 tTileSize = Vec2(1000.f / pXCount, 1000.f / pYCount);

            Vec2 LTpos = m_vNormalTileCollider[i].pLTpos / 32;
            Vec2 RBpos = m_vNormalTileCollider[i].pRBpos / 32;
            LTpos = Vec2(LTpos.x * tTileSize.x, LTpos.y * tTileSize.y);
            RBpos = Vec2(RBpos.x * tTileSize.x, RBpos.y * tTileSize.y);
            Vec2 BoxScale = Vec2(RBpos.x - LTpos.x, LTpos.y - RBpos.y);
            LTpos = Vec2(LTpos.x - 500.f, (LTpos.y - 500.f) * -1);
            RBpos = Vec2(RBpos.x - 500.f, (RBpos.y - 500.f) * -1);
            //
            //if (0 < BoxScale.x)
            //{
            //    BoxScale.x *= -1;
            //}
            //if (0 < BoxScale.y)
            //{
            //    BoxScale.y *= -1;
            //}

            CGameObject* pCollider = new CGameObject;
            wstring colliderNum = std::to_wstring(i);
            pCollider->SetName(L"Tile_Collider02" + colliderNum);
            pCollider->AddComponent(new CTransform);
            pCollider->AddComponent(new CCollider2D);
            pCollider->AddComponent(new CMeshRender);
            pCollider->AddComponent(new CWall);
            pCollider->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
            pCollider->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl"));

            pCollider->Transform()->SetRelativePos(Vec3(LTpos.x + BoxScale.x / 2, LTpos.y + BoxScale.y / 2, 200.f));
            pCollider->Transform()->SetRelativeScale(BoxScale.x, BoxScale.y, 1.f);
            pCollider->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
            pCollider->Collider2D()->SetOffsetPos(Vec2(0, 0));
            pCollider->Collider2D()->SetOffsetScale(Vec2(BoxScale.x, BoxScale.y));
            pScene->AddObject(pCollider, L"Wall");
        }

        for (size_t i = 0; i < m_vThroughTileCollider.size(); i++)
        {
            Vec2 tTileSize = Vec2(1000.f / pXCount, 1000.f / pYCount);

            Vec2 LTpos = m_vThroughTileCollider[i].pLTpos / 32;
            Vec2 RBpos = m_vThroughTileCollider[i].pRBpos / 32;
            LTpos = Vec2(LTpos.x * tTileSize.x, LTpos.y * tTileSize.y);
            RBpos = Vec2(RBpos.x * tTileSize.x, RBpos.y * tTileSize.y);
            Vec2 BoxScale = Vec2(RBpos.x - LTpos.x, LTpos.y - RBpos.y);
            LTpos = Vec2(LTpos.x - 500.f, (LTpos.y - 500.f) * -1);
            RBpos = Vec2(RBpos.x - 500.f, (RBpos.y - 500.f) * -1);
            // if (0 < BoxScale.x)
            //{
            //    BoxScale.x * -1;
            //}
            //if (0 < BoxScale.y)
            //{
            //    BoxScale.y * -1;
            //}
            CGameObject* pCollider = new CGameObject;
            wstring colliderNum = std::to_wstring(i);
            pCollider->SetName(L"Tile_Through_Collider02" + colliderNum);
            pCollider->AddComponent(new CTransform);
            pCollider->AddComponent(new CCollider2D);
            pCollider->AddComponent(new CMeshRender);
            pCollider->AddComponent(new CWall(true));
            pCollider->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
            pCollider->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl"));

            pCollider->Transform()->SetRelativePos(Vec3(LTpos.x + BoxScale.x / 2, LTpos.y + BoxScale.y / 2, 200.f));
            pCollider->Transform()->SetRelativeScale(BoxScale.x, BoxScale.y, 1.f);

            pCollider->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
            pCollider->Collider2D()->SetOffsetPos(Vec2(0,0));
            pCollider->Collider2D()->SetOffsetScale(Vec2(BoxScale.x, BoxScale.y));
            pScene->AddObject(pCollider, L"Wall");
        }
    }
    if (ImGui::Button("clear"))
    {
        clear();
    }
    ImGui::Checkbox("Draw Normal Collider Box", &m_bMakeNormalColliderBox);
    ImGui::Checkbox("Draw Through Collider Box", &m_bMakeThrouhColliderBox);
    if (m_bMakeNormalColliderBox)
        m_bMakeThrouhColliderBox = false;
    if (m_bMakeThrouhColliderBox)
        m_bMakeNormalColliderBox = false;

    // Canvas 그리기
    static ImVector<ImVec2> points;
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;
    static bool opt_enable_context_menu = true;
    static bool adding_line = false;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);

    // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;

    // canvas size
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    float originx = origin.x;
    float originy = origin.y;
    float xpos = mouse_pos_in_canvas.x;
    float ypos = mouse_pos_in_canvas.y;

    const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
    {
        scrolling.x += io.MouseDelta.x;
        scrolling.y += io.MouseDelta.y;
    }

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid)
    {
        const float GRID_STEP = pTileSize;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
    }
    for (int n = 0; n < points.Size; n += 2)
        draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
    draw_list->PopClipRect();

    if (pXCount != 0 && pYCount != 0)
    {
        draw_list->AddRect(ImVec2(origin.x, origin.y), ImVec2(origin.x + pTileSize * pXCount, origin.y + pTileSize * pYCount)
            , IM_COL32(255, 0, 0, 255));
    }

    for (size_t i = 0; i < m_vecTileData.size(); ++i)
	{

		float width = m_pTileAtlas->Width();
		float height = m_pTileAtlas->Height();

        m_vecTileData[i].iImgIdx;
        if (m_vecTileData[i].iImgIdx != -1)
        {
            int j = 0;
            Vec2 LTpos = Vec2(0,0);
            while (true)
            {
                if (m_pTileVec[j].index == i)
                {
                    //LTpos = m_pTileVec[j].LTPos;
                    LTpos.x = m_pTileVec[j].LTPos.x;
                    LTpos.y = m_pTileVec[j].LTPos.y;
                    break;
                }
                j++;
            }


            pTilePos = ImVec2((m_vecTileData[i].iImgIdx ) % AtlasxCount * m_vTileSize, (m_vecTileData[i].iImgIdx) / AtlasxCount * m_vTileSize);

            uv0 = ImVec2(pTilePos.x / m_pTileAtlas->Width(), pTilePos.y / m_pTileAtlas->Height());
            uv1 = ImVec2((pTilePos.x + m_vTileSize) / m_pTileAtlas->Width(), (pTilePos.y + m_vTileSize) / m_pTileAtlas->Height());
            ImVec2 vec1 = ImVec2(i % pXCount * m_vTileSize + origin.x, i / pXCount * m_vTileSize + origin.y);
            ImVec2 vec2 = ImVec2(vec1.x + m_vTileSize, vec1.y + m_vTileSize);
            draw_list->AddImage(m_pTileAtlas->GetSRV().Get(), ImVec2(LTpos.x + origin.x, LTpos.y + origin.y)
                , ImVec2(LTpos.x + m_vTileSize + origin.x, LTpos.y + m_vTileSize + origin.y), uv0, uv1, 0xFFFFFFFF);
        }
    }

    // Box Collider
    if (0 < m_vNormalTileCollider.size())
    {
        for (size_t i = 0; i < m_vNormalTileCollider.size(); i++)
        {
            Vec2 LT = m_vNormalTileCollider[i].pLTpos;
            Vec2 RB = m_vNormalTileCollider[i].pRBpos;
            draw_list->AddRect(ImVec2(origin.x + LT.x, origin.y + LT.y), ImVec2(origin.x + RB.x, origin.y + RB.y)
                , IM_COL32(255, 255, 0, 255));
        }
    }if (0 < m_vThroughTileCollider.size())
    {
        for (size_t i = 0; i < m_vThroughTileCollider.size(); i++)
        {
            Vec2 LT = m_vThroughTileCollider[i].pLTpos;
            Vec2 RB = m_vThroughTileCollider[i].pRBpos;
            draw_list->AddRect(ImVec2(origin.x + LT.x, origin.y + LT.y), ImVec2(origin.x + RB.x, origin.y + RB.y)
                , IM_COL32(0, 255, 255, 255));
        }
    }

    //ImGui::InputFloat("originxPos", &originx);
    //ImGui::InputFloat("originyPos", &originy);
    ImGui::InputFloat("xPos", &xpos); ImGui::SameLine();
    ImGui::InputFloat("yPos", &ypos);

    if (0 != pXCount && 0 != pYCount)
    {
        if (false == m_bMakeNormalColliderBox && false == m_bMakeThrouhColliderBox)
        {
            int xcount = (xpos / pTileSize);
            int ycount = (ypos / pTileSize);

            // 우클릭 => 타일 지우기, 
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
				ImGui::IsMouseDragging(ImGuiMouseButton_Right))
			{
                m_vecTileData[ycount * pYCount + xcount].iImgIdx = -1;
			}

            if (KEY_TAP(KEY::LBTN) || KEY_PRESSED(KEY::LBTN))
            {
                if (0 <= xpos && pXCount * pTileSize >= xpos && 0 <= ypos && pYCount * pTileSize >= ypos)
                {
                    TileInfo tInfo;
                    tInfo.pTex = m_pTileAtlas->GetSRV().Get();
                    tInfo.LTPos = ImVec2(xcount * pTileSize, ycount * pTileSize);
                    tInfo.RBPos = ImVec2(tInfo.LTPos.x + pTileSize, tInfo.LTPos.y + pTileSize);
                    tInfo.TileIdx = m_iTileIndex-1;
                    tInfo.index = ycount * pYCount + xcount;

                    m_pTileVec.push_back(tInfo);


                    //tTileData data;
                    //data.iImgIdx;
                    //data.vLTUV;

                    pTilePos = ImVec2((m_iTileIndex-1) % AtlasxCount * m_vTileSize, (m_iTileIndex-1) / AtlasxCount * m_vTileSize);

                    uv0 = ImVec2(pTilePos.x / m_pTileAtlas->Width(), pTilePos.y / m_pTileAtlas->Height());
                    uv1 = ImVec2((pTilePos.x + m_vTileSize) / m_pTileAtlas->Width(), (pTilePos.y + m_vTileSize) / m_pTileAtlas->Height());

                    m_vecTileData[tInfo.index].iImgIdx = m_iTileIndex;                    
                    int iRow = m_vecTileData[tInfo.index].iImgIdx / pXCount;
                    int iCol = m_vecTileData[tInfo.index].iImgIdx % pXCount;
                    Vec2 m_vSliceUV = Vec2(m_vTileSize, m_vTileSize) / Vec2(m_pTileAtlas->Width(), m_pTileAtlas->Height());
                    m_vecTileData[tInfo.index].vLTUV = Vec2(m_vSliceUV.x * iCol, m_vSliceUV.y * iRow);

                }
            }
        }
        else if(true == m_bMakeNormalColliderBox)
        {
            static Vec2 LTpos;
            static Vec2 RBpos;

            if (KEY_TAP(KEY::LBTN))
            {
                if (0 <= xpos && pXCount * pTileSize >= xpos && 0 <= ypos && pYCount * pTileSize >= ypos)
                {
                    int xcount = (xpos / pTileSize);
                    int ycount = (ypos / pTileSize);                   
                    
                    LTpos = Vec2(xcount * pTileSize, ycount * pTileSize);
                }
            }
            if (KEY_PRESSED(KEY::LBTN))
            {
                if (0 <= xpos && pXCount * pTileSize >= xpos && 0 <= ypos && pYCount * pTileSize >= ypos)
                {
                    int xcount = (xpos / pTileSize);
                    int ycount = (ypos / pTileSize);

                    RBpos = Vec2((xcount+1) * pTileSize, (ycount+1) * pTileSize);
                }
            }
            if (KEY_AWAY(KEY::LBTN))
            {
                BoxPos box;
                box.pLTpos = LTpos;
                box.pRBpos = RBpos;

                if(box.pLTpos.x != box.pRBpos.x && box.pLTpos.y != box.pRBpos.y)
                    m_vNormalTileCollider.push_back(box);
            }
        }
        else if (true == m_bMakeThrouhColliderBox)
        {
            static Vec2 LTpos;
            static Vec2 RBpos;

            if (KEY_TAP(KEY::LBTN))
            {
                if (0 <= xpos && pXCount * pTileSize >= xpos && 0 <= ypos && pYCount * pTileSize >= ypos)
                {
                    int xcount = (xpos / pTileSize);
                    int ycount = (ypos / pTileSize);

                    LTpos = Vec2(xcount * pTileSize, ycount * pTileSize);
                }
            }
            if (KEY_PRESSED(KEY::LBTN))
            {
                if (0 <= xpos && pXCount * pTileSize >= xpos && 0 <= ypos && pYCount * pTileSize >= ypos)
                {
                    int xcount = (xpos / pTileSize);
                    int ycount = (ypos / pTileSize);

                    RBpos = Vec2((xcount + 1) * pTileSize, (ycount + 1) * pTileSize);
                }
            }
            if (KEY_AWAY(KEY::LBTN))
            {
                BoxPos box;
                box.pLTpos = LTpos;
                box.pRBpos = RBpos;

                if (box.pLTpos.x != box.pRBpos.x && box.pLTpos.y != box.pRBpos.y)
                    m_vThroughTileCollider.push_back(box);
            }
        }
    }

}

//struct TileInfo
//{                                                 struct tTileData
//    ImTextureID  pTex;                            {
//    ImVec2       LTPos;                           	int		iImgIdx;
//    ImVec2       RBPos;                           	Vec2	vLTUV;
//    int            imgIndex;                      	int		iPadding;
//
//    int          index;                           	tTileData()
//};                                                		: iImgIdx(-1)
//                                                  		, iPadding(0)
//                                                  	{}
//                                                  };


void TileMapUI::TileInfoTotTileData(vector<TileInfo> _info, vector<tTileData >_data)
{
    for (size_t i = 0; i < _info.size(); i++)
    {
        tTileData data;
        data.iImgIdx = _info[i].TileIdx;
        data.vLTUV = _info[i].vLTUV;
        int pTileIndex = _info[i].index;

        _data[pTileIndex] = data;
    }  
}

void TileMapUI::tTileDataToTileInfo(vector<tTileData> _data, vector<TileInfo> _info)
{
    m_pTileVec.clear();
    for (size_t i = 0; i < _data.size(); i++)
    {
        if (_data[i].iImgIdx == -1)
            continue;
        else
        {
            TileInfo info;
            info.TileIdx = _data[i].iImgIdx;
            info.index = i;
            info.LTPos = Vec2(_data[i].vLTUV.x * m_pTileAtlas->Width(), _data[i].vLTUV.y * m_pTileAtlas->Height());
            info.pTex = m_pTileAtlas;
            info.RBPos = Vec2(info.LTPos.x + m_vTileSize, info.LTPos.y + m_vTileSize);
            info.vLTUV = _data[i].vLTUV;
            m_pTileVec.push_back(info);
        }
    }
}


void TileMapUI::SetTileIndex(int _index)
{
    m_iTileIndex = _index;
}

void TileMapUI::clear()
{
    m_pTargetTileMap = nullptr;
    m_pTileVec.clear();
    m_vNormalTileCollider.clear();
    m_vThroughTileCollider.clear();
}
