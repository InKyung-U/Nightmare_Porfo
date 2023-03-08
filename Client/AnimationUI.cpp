#include "pch.h"
#include "AnimationUI.h"

#include "CImGuiMgr.h"
#include"ListUI.h"

#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CCore.h>

#include <Script/CSceneSaveLoad.h>

AnimationUI::AnimationUI()
	:UI("##AnimationUI")
	, b_mAnimTool((bool*)false)
	, m_Atlas(nullptr)
	, v_mAnimationData()
	, v_mAnimBackGroundSize()
	, i_curFrameCount(0)
	, b_PlayAnimation(false)
	, f_AnimAddTime(0.f)
	, m_sAnimationName()
	, m_sAtlasName()
	, m_bSaveAnimation(false)
	, m_bLoadAnimation(false)
	, m_pAnimator(nullptr)
	, m_bAnimationRepeat(true)
	, m_bBoxOnlyMove(false)
	, m_pLoadAtlas(nullptr)
{
}

AnimationUI::~AnimationUI()
{
}

void AnimationUI::update()
{
	Task();
}

void AnimationUI::render_update()
{
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::Text("Atlas List "); ImGui::SameLine();

	Ptr<CTexture> m_img = CResMgr::GetInst()->Load<CTexture>(L"texture\\player\\Weapon_Default_Die.png"
															, L"texture\\player\\Weapon_Default_Die.png");

	if (ImGui::Button("##AtlasListBtn", Vec2(15, 15)))
	{
		// ListUI 활성화한다.
		const vector<wstring>& atlasList = CResMgr::GetInst()->GetAtlasList();

		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Atlas List");

		for (size_t i = 0; i < atlasList.size(); i++)
		{
			pListUI->AddList(string(atlasList[i].begin(), atlasList[i].end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&AnimationUI::AtlasSelect);

	}

	ImGui::Text("Animation List "); ImGui::SameLine();
	if (ImGui::Button("##AnimationListBtn", Vec2(15, 15)))
	{
		// ListUI 활성화한다.
		const vector<wstring>& AnimationList = CResMgr::GetInst()->GetAnimationList();

		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Animation List");

		for (size_t i = 0; i < AnimationList.size(); i++)
		{
			pListUI->AddList(string(AnimationList[i].begin(), AnimationList[i].end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&AnimationUI::AnimationSelect);

	}
	ImGuiIO& io = ImGui::GetIO();

	if (m_sAtlasName != m_img.Get()->GetKey())
	{
		if (m_sAtlasName == L"")
		{
			m_sAtlasName = m_img.Get()->GetKey();
		}
		else
		{
			m_img = CResMgr::GetInst()->FindRes<CTexture>(m_sAtlasName);
		}
		m_sAtlasName = m_img.Get()->GetKey();
	}
	m_texture = CResMgr::GetInst()->FindRes<CTexture>(m_sAtlasName)->GetSRV().Get();
	

	ImTextureID my_tex_id = io.Fonts->TexID;
	float my_tex_w = (float)io.Fonts->TexWidth;
	float my_tex_h = (float)io.Fonts->TexHeight;

	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

	ImGui::Image((void*)m_texture, ImVec2(m_img->Width(), m_img->Height()), uv_min, uv_max, tint_col, border_col);
	Vec2 imgMousePosition = Vec2(io.MousePos.x - pos.x, io.MousePos.y - pos.y);


	if (nullptr != m_pLoadAtlas)
	{
		m_texture = m_pLoadAtlas->GetSRV().Get();
	}
	static UINT index = 0;;
	if (b_PlayAnimation == false)
	{
		if (0 != i_curFrameCount)
		{
			index = i_curFrameCount - 1;
			ImVec2 min = ImVec2(pos.x + 850 - v_mAnimationData[index].ImgScale.x / 2 + v_mAnimationData[index].Offset.x
								, pos.y + 250 - v_mAnimationData[index].ImgScale.y / 2 + v_mAnimationData[index].Offset.y);
			ImVec2 max = ImVec2(pos.x + 850 + v_mAnimationData[index].ImgScale.x / 2 + v_mAnimationData[index].Offset.x
								, pos.y + 250 + v_mAnimationData[index].ImgScale.y / 2 + v_mAnimationData[index].Offset.y);
			ImVec2 uv0 = ImVec2(v_mAnimationData[index].LTPos.x / m_Atlas->Width()
				, v_mAnimationData[index].LTPos.y / m_Atlas->Height());
			ImVec2 uv1 = ImVec2((v_mAnimationData[index].LTPos.x + v_mAnimationData[index].ImgScale.x) / m_Atlas->Width(),
				(v_mAnimationData[index].LTPos.y + v_mAnimationData[index].ImgScale.y) / m_Atlas->Height());

			ImGui::GetWindowDrawList()->AddImage(m_texture, min, max, uv0, uv1);
		}
	}
	else
	{
		// 애니메이션 paly를 누른 상태.
		f_AnimAddTime += DT;
		if (v_mAnimationData[index].duration < f_AnimAddTime)
		{
			f_AnimAddTime = 0.f;
			i_curFrameCount++;

			if (i_curFrameCount > v_mAnimationData.size())
				i_curFrameCount = 1;
		}
		if (i_curFrameCount == 0)
			i_curFrameCount = 1;
		index = i_curFrameCount - 1;
		ImVec2 min = ImVec2(pos.x + 850 - v_mAnimationData[index].ImgScale.x / 2 + v_mAnimationData[index].Offset.x
						, pos.y + 250 - v_mAnimationData[index].ImgScale.y / 2 + v_mAnimationData[index].Offset.y);
		ImVec2 max = ImVec2(pos.x + 850 + v_mAnimationData[index].ImgScale.x / 2 + v_mAnimationData[index].Offset.x
						, pos.y + 250 + v_mAnimationData[index].ImgScale.y / 2 + v_mAnimationData[index].Offset.y);
		ImVec2 uv0 = ImVec2(v_mAnimationData[index].LTPos.x / m_Atlas->Width()
			, v_mAnimationData[index].LTPos.y / m_Atlas->Height());
		ImVec2 uv1 = ImVec2((v_mAnimationData[index].LTPos.x + v_mAnimationData[index].ImgScale.x) / m_Atlas->Width(),
			(v_mAnimationData[index].LTPos.y + v_mAnimationData[index].ImgScale.y) / m_Atlas->Height());

		ImGui::GetWindowDrawList()->AddImage(m_texture, min, max, uv0, uv1);
	}


	if (0 > imgMousePosition.x)
		imgMousePosition.x = 0;
	if (0 > imgMousePosition.y)
		imgMousePosition.y = 0;
	if (m_img->Width() < imgMousePosition.x)
		imgMousePosition.x = m_img->Width();
	if (m_img->Height() < imgMousePosition.y)
		imgMousePosition.y = m_img->Height();
	// 같은 줄에 뭐 띄움.. 애니메이션 보여주는 박스
	ImU32 whiteColor = IM_COL32(255, 255, 255, 255);
	ImU32 grayColor = IM_COL32(50, 50, 50, 255);


	ImGui::GetWindowDrawList()->AddRect(rect1, rect2, whiteColor/*0xFF0000FF*/, 1.0f, 0, 1.f);
	// 박스 무빙무빙
	if (ImGui::IsMouseDragging(0))
		BoxMovingCheck(m_img->Width(), m_img->Height(), pos);

	if (ImGui::Button("Add Animation Frame", ImVec2(200, 20)))
	{
		Vec2 BGSize = Vec2(rect2.x - rect1.x, rect2.y - rect1.y);
		Vec2 LTPos = Vec2(rect1.x - pos.x, rect1.y - pos.y);
		Vec2 _imgScale = Vec2(rect2.x - rect1.x, rect2.y - rect1.y);
		float _duration = 0.1f;
		UINT _maxFrame = v_mAnimationData.size();
		UINT _curFrame = v_mAnimationData.size(); // 계속 추가한다는 조건 하에 max frame설정과 동일
													// 입력값이 존재하면 입력 값으로 조절 하는 기능 추가

		AddAnimationVector(BGSize, LTPos, _imgScale, _duration, _maxFrame, _curFrame);
	}	ImGui::SameLine();
	ImGui::Text("Set Animation Name");	ImGui::SameLine();
	ImGui::PushItemWidth(150.f);
	static char animationName[20] = {};
	ImGui::InputText("##Animation Name", animationName, IM_ARRAYSIZE(animationName));
	if (animationName != nullptr)
		if (animationName != m_sAnimationName)
			m_sAnimationName = animationName;

	ImGui::Checkbox("Animation Repeat", &m_bAnimationRepeat); ImGui::SameLine();
	ImGui::Checkbox("Box Only Move", &m_bBoxOnlyMove);

	//=============
	// Offset
	//=============
	static int offsetX = 0;
	static int offsetY = 0;
	if (i_curFrameCount == 0)
	{
		offsetX = 0;
		offsetY = 0;
	}
	if (i_curFrameCount != 0)
	{
		if (offsetX != v_mAnimationData[i_curFrameCount - 1].Offset.x)
			offsetX = v_mAnimationData[i_curFrameCount - 1].Offset.x;
		if (offsetY != v_mAnimationData[i_curFrameCount - 1].Offset.y)
			offsetY = v_mAnimationData[i_curFrameCount - 1].Offset.y;
	}
	if (ImGui::Button("-##offSetX_Minus"))
	{
		v_mAnimationData[i_curFrameCount-1].Offset.x -= 1;
		offsetX -= 1;

	}ImGui::SameLine();
	ImGui::InputInt("##offsetX", &offsetX,1, 100, 1); ImGui::SameLine();
	if (ImGui::Button("+##offSetX_Plus"))
	{
		v_mAnimationData[i_curFrameCount-1].Offset.x += 1;
		offsetX += 1;
	}
	if (ImGui::Button("-##offSetY_Minus"))
	{
		v_mAnimationData[i_curFrameCount-1].Offset.y += 1;
		offsetY += 1;
	}ImGui::SameLine();
	ImGui::InputInt("##offsetY", &offsetY); ImGui::SameLine();
	if (ImGui::Button("+##offSetY_Plus"))
	{
		v_mAnimationData[i_curFrameCount-1].Offset.y -= 1;
		offsetY -= 1;
	}ImGui::SameLine();
	if (i_curFrameCount != 0)
	{
		if (offsetX != v_mAnimationData[i_curFrameCount - 1].Offset.x)
			v_mAnimationData[i_curFrameCount - 1].Offset.x = offsetX;
		if (offsetY != v_mAnimationData[i_curFrameCount - 1].Offset.y)
			v_mAnimationData[i_curFrameCount - 1].Offset.y = offsetY;
	}

	//================
	// 애니메이션 옵션
	//================
	Vec2 centerPreViewBox = Vec2(850, 250);
	//ImGui::SameLine(ImGui::GetWindowWidth() - 400);
	if (ImGui::Button("Apply PreView Animation", ImVec2(200, 20)))
	{
		i_curFrameCount = 1;
	}ImGui::SameLine();
	if (ImGui::Button("Clean Box", ImVec2(100, 20)))
	{
		i_curFrameCount = 0;
		b_PlayAnimation = false;
	}

	ImGui::Text("Input Duration vlaue");	ImGui::SameLine();
	static float duration = 0.1f;
	ImGui::InputFloat("##InputFloat : Duration", &duration, 0.f, 0.f, "%.3f", 0); ImGui::SameLine();
	if (0 != v_mAnimationData.size())
		if (duration != v_mAnimationData[0].duration)
			for (size_t i = 0; i < v_mAnimationData.size(); i++)
				v_mAnimationData[i].duration = duration;

	if (ImGui::Button("Play Animation", ImVec2(200, 20)))
		b_PlayAnimation = true;
	//ImGui::SameLine(ImGui::GetWindowWidth() - 400);


	// 애니메이션 저장
	if (ImGui::Button("Save", ImVec2(100, 20)))
	{
		if (i_curFrameCount != 0)
		{
			m_bSaveAnimation = true;
			const wstring atlasName(animationName, &animationName[20]);

			CAnimator2D* pAnim = new CAnimator2D;
			pAnim->SetName(atlasName);
			pAnim->CreateAnim(atlasName, m_img, v_mAnimationData);
			m_pAnimator = pAnim;
			//================================
			// 애니메이션 생성하면
			// 이거 릭 남음ㅋㅋㅋ
			//================================
			//delete pAnim;		
		}
		
	}ImGui::SameLine();
	if (ImGui::Button("Load", ImVec2(100, 20)))
	{
		m_bLoadAnimation = true;
	}

	
	ImGui::SameLine();

	if (ImGui::Button("Erase", ImVec2(100, 20)))
	{
		b_PlayAnimation = false;
		i_curFrameCount = 0;
		v_mAnimationData.clear();
		m_pLoadAtlas = nullptr;
		m_Atlas = nullptr;
		m_sAnimationName = {};
		m_pAnimator = nullptr;
		m_texture = nullptr;
		f_AnimAddTime = 0.f;
		index = 0;
	}

	if (nullptr != m_pAnimator)
	{
		int i = 0;
	}

	//-------------------------
	// 추가된 애니메이션 그리기
	//-------------------------
	if (0 != v_mAnimationData.size())
	{
		if(nullptr == m_Atlas)
			m_Atlas = CResMgr::GetInst()->FindRes<CTexture>(m_sAtlasName).Get();

		ShowAnimationFrame(m_Atlas->Width(), m_Atlas->Height(), m_Atlas->GetSRV().Get());
	}

	// 미리보기 이미지를 그리는 박스
	ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + 700, pos.y + 250), ImVec2(pos.x + 1000, pos.y + 250), grayColor, 1.f);
	ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + 850, pos.y + 100), ImVec2(pos.x + 850, pos.y + 400), grayColor, 1.f);
	ImGui::GetWindowDrawList()->AddRect(ImVec2(pos.x + 700, pos.y + 100), ImVec2(pos.x + 1000, pos.y + 400), whiteColor, 0.0f, 0, 4.f);
}

void AnimationUI::Task()
{
	if (m_bSaveAnimation)
	{
		wstring wsName = wstring(m_sAnimationName.begin(), m_sAnimationName.end());
		LPCWSTR str = (LPCWSTR)wsName.c_str();


		wchar_t szName[256] = {};
		OPENFILENAME ofn = {};

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
		ofn.lpstrFile = szName;
		ofn.nMaxFile = sizeof(szName);
		ofn.lpstrFilter = L"ALL\0*.*\0Anim\0*.anim\0";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;

		wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
		strTileFolder += L"anim";

		ofn.lpstrInitialDir = strTileFolder.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Modal
		if (GetSaveFileName(&ofn))
		{
			FILE* pFile = nullptr;
			_wfopen_s(&pFile, szName, L"wb");

			assert(pFile);
			if (nullptr == pFile)
				return ;

			m_pAnimator->SaveAnimFile(pFile);

			fclose(pFile);
		}


		m_bSaveAnimation = false;
	}

	map<wstring, CAnimation2D*> mapAnim;
	CAnimation2D* animation;
	if (m_bLoadAnimation)
	{
		v_mAnimationData.clear();
		wchar_t szName[256] = {};

		OPENFILENAME ofn = {};

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
		ofn.lpstrFile = szName;
		ofn.nMaxFile = sizeof(szName);
		ofn.lpstrFilter = L"ALL\0*.*\0Anim\0*.anim\0";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;

		wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
		strTileFolder += L"anim";

		ofn.lpstrInitialDir = strTileFolder.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Modal
		if (GetOpenFileName(&ofn))
		{
			wstring strKey = CPathMgr::GetInst()->GetRelativePath(szName);
			CAnimator2D* pLoadAnim = new CAnimator2D;
			pLoadAnim->SetName(strKey);

			FILE* pFile = nullptr;
			_wfopen_s(&pFile, szName, L"rb");

			assert(pFile);
			if (nullptr == pFile)
				return;

			pLoadAnim->LoadAnimFile(pFile);

			fclose(pFile);

			m_pAnimator = pLoadAnim;

			v_mAnimationData;
			mapAnim = m_pAnimator->GetMapAnim();
		}
		if (1 == mapAnim.size())
		{
			animation = mapAnim.begin()->second;
			m_pLoadAtlas = animation->GetAtlas().Get();
			m_Atlas = animation->GetAtlas().Get();
			vector<tAnim2DFrame> Frame = animation->GetVecFrm();
			for (size_t i = 0; i < Frame.size(); i++)
			{
				AnimToolData data;
				data.BackGroundSize = animation->GetBackGroundSize();
				data.CurFrameCount = (int)i;
				data.MaxFrameCount = Frame.size();
				tAnim2DFrameTransAnimToolData(Frame[i], data);

				v_mAnimationData.push_back(data);
			}
		}
		m_bLoadAnimation = false;
	}
}

void AnimationUI::AddAnimationVector(Vec2 _BGSize, Vec2 _LTPos, Vec2 _imgScale, float _duration, UINT _maxFrame, UINT _curFrame)
{
	AnimToolData data;
	data.BackGroundSize = _BGSize;
	data.LTPos = _LTPos;
	data.ImgScale = _imgScale;
	data.duration = _duration;
	for (size_t i = 0; i < v_mAnimationData.size(); i++)
	{
		v_mAnimationData[i].MaxFrameCount = _maxFrame;
	}
	data.CurFrameCount = _curFrame;
	data.Offset = Vec2(0,0);
	v_mAnimationData.push_back(data);
}

void AnimationUI::ShowAnimationFrame(UINT _width, UINT _height, ID3D11ShaderResourceView* _tex)
{
	for (size_t i = 0; i < v_mAnimationData.size(); i++)
	{
		//m_pAnimator->GetMapAnim().begin()->second;
		if (i%8 == 0)
			ImGui::NewLine();
		ImGui::PushID(i);
		if (i == 0)
			v_mAnimBackGroundSize = v_mAnimationData[i].BackGroundSize;
		if (v_mAnimBackGroundSize.x < v_mAnimationData[i].BackGroundSize.x)
			v_mAnimBackGroundSize.x = v_mAnimationData[i].BackGroundSize.x;
		if (v_mAnimBackGroundSize.y < v_mAnimationData[i].BackGroundSize.y)
			v_mAnimBackGroundSize.y = v_mAnimationData[i].BackGroundSize.y;

		ImVec2 size = ImVec2(v_mAnimationData[i].BackGroundSize.x, v_mAnimationData[i].BackGroundSize.y);
		ImVec2 uv0 = ImVec2(v_mAnimationData[i].LTPos.x / _width, v_mAnimationData[i].LTPos.y / _height);
		ImVec2 uv1 = ImVec2((v_mAnimationData[i].LTPos.x + v_mAnimationData[i].ImgScale.x) / _width,
			(v_mAnimationData[i].LTPos.y + v_mAnimationData[i].ImgScale.y) / _height);
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
		if (ImGui::ImageButton(_tex, size, uv0, uv1, 2, bg_col, tint_col))
		{
			i_curFrameCount = i + 1;
			b_PlayAnimation = false;
		}
		if (i != v_mAnimationData.size() - 1)
			ImGui::SameLine();
		ImGui::PopID();
	}
}

void AnimationUI::BoxMovingCheck(int _width, int _height, ImVec2 _pos)
{
	int lineTargetRange = 10;
	ImVec2 mouse_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

	if (rect1.x - lineTargetRange <= mouse_pos.x && rect2.x + lineTargetRange >= mouse_pos.x
		&& rect1.y - lineTargetRange <= mouse_pos.y && rect2.y + lineTargetRange >= mouse_pos.y)
	{
		if (false == m_bBoxOnlyMove)
		{
			// 박스 움직이는거
			rect1.x += ImGui::GetIO().MouseDelta.x;
			rect1.y += ImGui::GetIO().MouseDelta.y;
			rect2.x += ImGui::GetIO().MouseDelta.x;
			rect2.y += ImGui::GetIO().MouseDelta.y;

			if (0 + _pos.x >= rect1.x)
				rect1.x = rect1.x;
			if (_width + _pos.x + 1 <= rect2.x)
				rect1.x = rect1.x;
			if (0 + _pos.y >= rect1.y)
				rect1.y = rect1.y;
			if (_height + _pos.y + 1 <= rect2.y)
				rect1.y = rect1.y;
		}
		else
		{
			if (rect1.x + lineTargetRange >= mouse_pos.x)
				rect1.x += ImGui::GetIO().MouseDelta.x;
			else if (rect2.x - lineTargetRange <= mouse_pos.x)
				rect2.x += ImGui::GetIO().MouseDelta.x;
			else if (rect1.y + lineTargetRange >= mouse_pos.y)
				rect1.y += ImGui::GetIO().MouseDelta.y;
			else if (rect2.y - lineTargetRange <= mouse_pos.y)
				rect2.y += ImGui::GetIO().MouseDelta.y;
			else
			{
				// 박스 움직이는거
				rect1.x += ImGui::GetIO().MouseDelta.x;
				rect1.y += ImGui::GetIO().MouseDelta.y;
				rect2.x += ImGui::GetIO().MouseDelta.x;
				rect2.y += ImGui::GetIO().MouseDelta.y;

				if (0 + _pos.x >= rect1.x)
					rect1.x = rect1.x;
				if (_width + _pos.x + 1 <= rect2.x)
					rect1.x = rect1.x;
				if (0 + _pos.y >= rect1.y)
					rect1.y = rect1.y;
				if (_height + _pos.y + 1 <= rect2.y)
					rect1.y = rect1.y;
			}
		}
	}
}

void AnimationUI::tAnim2DFrameTransAnimToolData(tAnim2DFrame _frm, AnimToolData& _data)
{
	_data.duration = _frm.fDuration;
	_data.ImgScale = _frm.vSlice;
	_data.LTPos = _frm.vLT;
	_data.Offset = _frm.vOffset;
}

void AnimationUI::AtlasSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;
	wstring strMeshKey = wstring(strSelectedName.begin(), strSelectedName.end());
	m_sAtlasName = strMeshKey;

	Ptr<CTexture> newAtlas = CResMgr::GetInst()->FindRes<CTexture>(strMeshKey);

	//m_img = newAtlas;
	m_texture = newAtlas->GetSRV().Get();
}

void AnimationUI::AnimationSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;
	wstring strMeshKey = wstring(strSelectedName.begin(), strSelectedName.end());
}
