#pragma once
#include "UI.h"

class TreeUI;
class CGameObject;
class CPrefab;

class ResourceUI :
    public UI
{
private:
    TreeUI* m_TreeUI;

    vector<wstring> m_vecResPath;
    CGameObject* m_pObject;
    CPrefab* m_pPrefab;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();

private:
    void Reload();
    void Renew();
    void FindFileName(const wstring& _strFolderPath);

    void ItemClicked(DWORD_PTR _dwNode);
    void ItemDBClicked(DWORD_PTR _dwNode);
    void ItemDragAndDrop(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop);

    RES_TYPE GetResTypeFromExt(const wstring& _strExt);


public:
    ResourceUI();
    ~ResourceUI();
};

