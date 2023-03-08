#include "pch.h"
#include "SceneOutliner.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CAnimator2D.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include <Script/CSceneSaveLoad.h>



SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
	, m_sNewObjName()
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutliner");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);
	m_TreeUI->UseDragDropOuter(true);
	m_TreeUI->UseDragDropSelf(true);


	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutliner::ObjectClicked);

	// Drag and Drop Delegate 등록
	m_TreeUI->SetDragAndDropDelegate(this, (DRAG_DROP)&SceneOutliner::DragAndDropDelegate);

	// Key Delegate 등록
	m_TreeUI->SetKeyBinding(KEY::DEL, this, (CLICKED)&SceneOutliner::PressDelete);
	

	Reset();
}

SceneOutliner::~SceneOutliner()
{

}

void SceneOutliner::update()
{
	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Reset();
	}
	if (CImGuiMgr::GetInst()->IsScriptAdded())
	{
		CImGuiMgr::GetInst()->SetScriptAdd(false);
		Reset();
	}

	UI::update();
}

void SceneOutliner::render_update()
{
	if(ImGui::Button("Make Prefab"))
	{
		TreeNode* pSelecteNode = CImGuiMgr::GetInst()->GetSelectedNode();
		if (nullptr != pSelecteNode)
		{
			wstring pNodeName;
			pNodeName.assign(pSelecteNode->GetName().begin(), pSelecteNode->GetName().end());
			// 선택한 노드 프리펩 화	

			CGameObject* pObject = CSceneMgr::GetInst()->FindObjectByName(pNodeName);
			if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(pNodeName))
			{
				CGameObject* pClonObject = pObject->Clone();
				CPrefab* pPrefab = new CPrefab;
				pPrefab->SetProto(pObject->Clone());
				wstring path = CPathMgr::GetInst()->GetContentPath();
				pNodeName = L"prefab\\" + pNodeName + L".pref";
				path += L"prefab\\" + pNodeName;
				//pNodeName = L"prefab\\" + pNodeName;
				CResMgr::GetInst()->AddRes<CPrefab>(pNodeName, pPrefab);

				//wstring path = CPathMgr::GetInst()->GetContentPath();
				//path += L"prefab\\" + pNodeName;
				//pPrefab->Save(path);
				//CSceneSaveLoad::SavePrefab(pPrefab, path);
			}
		}

	}ImGui::SameLine();
	if (ImGui::Button("Make new Object"))
	{
		ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
		vWinSize.y -= 27;
		ImGui::OpenPopup("MakeObject");	
	}
	if (ImGui::BeginPopupModal("MakeObject", nullptr, 0))
	{
		static char pNewObjectName[255];
		ImGui::InputText("NewObjectName", pNewObjectName, 255);
		m_sNewObjName = string(pNewObjectName);
		wstring name = wstring(m_sNewObjName.begin(), m_sNewObjName.end());

		static bool collider2D = false;
		ImGui::Checkbox("Collider2D", &collider2D); ImGui::SameLine();
		static bool animation2D = false;
		ImGui::Checkbox("Animation2D", &animation2D); 

		if (ImGui::Button("Make"))
		{
			CGameObject* pObject = new CGameObject;
			CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);
			pObject->SetName(name);			
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));


			if (collider2D)
			{
				pObject->AddComponent(new CCollider2D);
				pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
				pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
				pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
			}
			if (animation2D)
			{
				pObject->AddComponent(new CAnimator2D);
			}

			pScene->AddObject(pObject, L"Default");
			Reset();
		}
		ImGui::SameLine();
		if(ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
	
		ImGui::EndPopup();
	}
	ImGui::Separator();

}

void SceneOutliner::Reset()
{
	m_TreeUI->Clear();

	// 현재 Scene 을 가져온다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);

		vector<CGameObject*>& vecRoots = pLayer->GetRootObjects();

		for (size_t i = 0; i < vecRoots.size(); ++i)
		{
			AddGameObjectToTree(vecRoots[i], m_TreeUI->GetDummyNode());
		}
	}

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}

void SceneOutliner::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	CGameObject* pObject = (CGameObject*)pNode->GetData();
		
	assert(pObject);		

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(pObject);
}

void SceneOutliner::AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
											, string(_pObject->GetName().begin(), _pObject->GetName().end())
											, (DWORD_PTR)_pObject);

	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(vecChild[i], pNode);
	}
}

void SceneOutliner::PressDelete(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	if (nullptr == pNode)
		return;

	CGameObject* pTargetObj = (CGameObject*)pNode->GetData();
	pTargetObj->Destroy();

	// InspectorUI 를 찾아서 Object 를 nullptr 로 세팅한다.
		
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}

void SceneOutliner::DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop)
{
	CGameObject* pChildObject = (CGameObject*)_dwDrag;
	CGameObject* pDropTargetObject = (CGameObject*)_dwDrop;


	// 드롭 목적지가 제대로 들어 온 경우
	if (nullptr != pDropTargetObject)
	{
		if (pChildObject == pDropTargetObject
			|| pDropTargetObject->IsAncestor(pChildObject))
		{
			return;
		}

		CSceneMgr::GetInst()->AddChild(pDropTargetObject, pChildObject);
	}

	// 자식 오브젝트의 목적지가 nullptr 인 경우
	else
	{
		// 이미 최상위 부모 오브젝트는 이벤트 처리가 필요없다.
		if (nullptr == pChildObject->GetParent())
		{
			return;
		}

		CSceneMgr::GetInst()->DisconnectParent(pChildObject);		
	}
}

void SceneOutliner::ResDrop(DWORD_PTR _resPtr)
{
	if (ImGui::BeginDragDropTarget())
	{
		DWORD_PTR dwData = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource"))
		{
			memcpy(&dwData, payload->Data, sizeof(DWORD_PTR));
		}

		ImGui::EndDragDropTarget();
	}

}