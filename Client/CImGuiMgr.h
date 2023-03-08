#pragma once

#include "UI.h"

typedef void (UI::* PARAM_1)(DWORD_PTR);

struct tUIDelegate
{
	UI*			pInst;
	PARAM_1		pFunc;
	DWORD_PTR	dwParam;
};

class TreeNode;

class CImGuiMgr
	: public CSingleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)
private:
	map<string, UI*>	m_mapUI;
	vector<tUIDelegate>	m_vecDelegate;

	HANDLE				m_hNotify;

	TreeNode* m_pRememDragNode;
	TreeNode* m_pSelectedNode;
	bool m_bScriptAdd;

public:
	void init(HWND _hwnd);

	void progress();
	void render();
	void clear();

private:
	void CreateUI();
	void ObserveContent();

public:
	void SetRememberDragNode(TreeNode* _node) { m_pRememDragNode = _node; }
	TreeNode* GetRememberDragNode() { return m_pRememDragNode; }
	void SetSelectedNode(TreeNode* _node) { m_pSelectedNode = _node; }
	TreeNode* GetSelectedNode() { return m_pSelectedNode; }

	bool IsScriptAdded() { return m_bScriptAdd; }
	void SetScriptAdd(bool _bool) { m_bScriptAdd = _bool; }

public:
	UI* FindUI(const string& _strKey);
	void AddDelegate(tUIDelegate _del){m_vecDelegate.push_back(_del);}
};

