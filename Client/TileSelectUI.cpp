#include "pch.h"
#include "TileSelectUI.h"

#include "TileMapUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>

TileSelectUI::TileSelectUI()
	: UI("##TileSelectUI")
	, m_pTileAtlas(nullptr)
	, IndexPos(ImVec2(-1, -1))
{
	m_pTileAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Tileset_1.png").Get();
}

TileSelectUI::~TileSelectUI()
{
}

void TileSelectUI::update()
{
}

void TileSelectUI::render_update()
{
	Ptr<CTexture> m_img = CResMgr::GetInst()->Load<CTexture>(L"texture\\Tileset_1.png", L"texture\\Tileset_1.png");

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 MousePos = ImGui::GetMousePos();
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // 이 코드 후에 나오는 이미지나 버튼 위치

	ImGui::Image(m_img->GetSRV().Get(), ImVec2(m_img->Width(), m_img->Height())
		, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

	ImGui::PushItemWidth(150);

	// atlas 의 절대 위치는 canvas_p0 - MousePos
	static ImVec2 pos;
	pos.x = MousePos.x - canvas_p0.x;
	pos.y = MousePos.y - canvas_p0.y;

	ImGui::InputFloat("xPos", &pos.x); ImGui::SameLine();
	ImGui::InputFloat("yPos", &pos.y);

	if (KEY_TAP(KEY::LBTN))
	{
		if (0 <= pos.x && m_img->Width() >= pos.x && 0 <= pos.y && m_img->Height() >= pos.y)
		{
			IndexPos.x = pos.x;
			IndexPos.y = pos.y;
		}
		else
		{	// 올바른 영역안에서 클릭 안하면 indexPos 는 -1값을 가짐
			IndexPos.x = -1;
			IndexPos.y = -1;
		}
	}
	static int pTIndex = 0;
	if (0 <= IndexPos.x && 0 <= IndexPos.y)
	{
		int a1 = m_img->Width() / 32;
		int a2 = (IndexPos.y / 32);
		int a3 = (IndexPos.x / 32);
		int a4 = a1 * a2 + a3 + 1;
		pTIndex = a4;
		m_iTileIndex = pTIndex;
		//pTIndex = (((IndexPos.y / 64) * (m_img->Width() / 64))) + (IndexPos.x / 64) + 1;
	}
	ImGui::Text("Selected Tile Index"); ImGui::SameLine(200);
	ImGui::InputInt("##SelecteTileIndex", &pTIndex, 0);

	if (ImGui::IsMouseDoubleClicked(0))
	{
		Deactivate();

		(m_Inst->*m_DBCEvent)(pTIndex-1);
	}

}

void TileSelectUI::Activate()
{
	UI::Activate();
	SetSize(Vec2(500.f, 600.f));
}
