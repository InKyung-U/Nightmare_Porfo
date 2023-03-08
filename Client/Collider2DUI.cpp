#include "pch.h"
#include "Collider2DUI.h"
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>


Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::update()
{
	ComponentUI::update();

	CGameObject* pTargetObj = GetTargetObject();
	if (nullptr == pTargetObj)
		return;
	CCollider2D* pCollider = pTargetObj->Collider2D();

	m_vRelativePos = Vec3(pCollider->GetOffSetPos().x, pCollider->GetOffSetPos().y, 0);
	m_vRelativeScale = Vec3(pCollider->GetOffSetSclae().x, pCollider->GetOffSetSclae().y, 1);
}

void Collider2DUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CCollider2D* pColl = pTargetObject->Collider2D();
	Vec3 vPos = Vec3(pColl->GetOffSetPos().x, pColl->GetOffSetPos().y, 0);
	Vec3 vScale = Vec3(pColl->GetOffSetSclae().x, pColl->GetOffSetSclae().y, 1);


	ImGui::PushItemWidth(200); // Float3 위젯 간격 설정

	ImGui::Text("Relative Position");
	ImGui::SameLine();
	ImGui::InputFloat3("##Position", vPos);
	pColl->SetOffsetPos(vPos.x, vPos.y);

	ImGui::Text("Relative Scale   ");
	ImGui::SameLine();
	ImGui::InputFloat3("##Scale", vScale);
	pColl->SetOffsetScale(vScale.x, vScale.y);

}
