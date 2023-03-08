#include "pch.h"
#include "CPlayerArrow.h"
#include "CWall.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <random>


CPlayerArrow::CPlayerArrow(tPlayerLookDir _dir)
	: CScript((int)SCRIPT_TYPE::PLAYERARROW)
	, m_fSpeed(500.f)
	, m_fAccTime(0.f)
	, m_pPlayerDir(_dir)
	, m_vDir(Vec3(0.f, 0.f, 0.f))
	, m_fAngle(0.f)
{
	if (m_pPlayerDir == tPlayerLookDir::RIGHT)
		m_vDir.x = 1.f;
	else
		m_vDir.x = -1.f;
}

CPlayerArrow::CPlayerArrow()
	: CScript((int)SCRIPT_TYPE::PLAYERARROW)
	, m_fSpeed(500.f)
	, m_fAccTime(0.f)
	, m_pPlayerDir(tPlayerLookDir::RIGHT)
	, m_vDir(Vec3(0.f, 0.f, 0.f))
	, m_fAngle(0.f)
{
}

CPlayerArrow::~CPlayerArrow()
{
}

void CPlayerArrow::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRotate = Transform()->GetRelativeRotation();

	if (m_pPlayerDir == tPlayerLookDir::RIGHT)
	{
		vPos.x += DT * m_fSpeed * m_vDir.x;
	}
	else
	{
		vPos.x -= DT * m_fSpeed * (-m_vDir.x);
	}
	vPos.y += DT * m_fSpeed * m_vDir.y;

	m_fAccTime += DT;

	if (m_fAccTime >= 5.f)
	{
		GetOwner()->Destroy();
	}
	vRotate.z = m_fAngle;
	Transform()->SetRelativeRotation(vRotate);
	Transform()->SetRelativePos(vPos);
}

void CPlayerArrow::OnCollisionEnter(CGameObject* _pOtherObj)
{
	//GetOwner()->Destroy();
	/*if (_pOtherObj->GetName() == L"ChildObject")
	{
		_pOtherObj->Destroy();
	}*/

	CScript* script = _pOtherObj->GetScript<CWall>();
	if (nullptr != script)
		m_vDir = Reflect(m_vDir, _pOtherObj);
}

Vec3 CPlayerArrow::Reflect(Vec3 _dir, CGameObject* _pOtherObj)
{
	Vec3 vRotate = Transform()->GetRelativeRotation();

	// ������ �� �����ϰ� �������� ������ �����Ͽ� �ݻ��Ų��.
// �������� -60 ~ 60���� ���� �Է� �޴´�.
	Vec3 normal = _dir.Normalize();
	float angle;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-60, 60);
	angle = dis(gen);

	// ���� ��ġ�� ���� ���� ũ�⸦ ������ ȭ���� ����� ��� �¾Ҵ����� ���Ѵ�.
	Vec3 arrowPos = Transform()->GetRelativePos();
	Vec3 scale = _pOtherObj->Transform()->GetRelativeScale();
	Vec3 pos = _pOtherObj->Transform()->GetRelativePos();

	// �簢���� �߽��� 0,0 ���� �ΰ� �Ѻ��� ���̰� 2�� ���簢���� �浹ü�� �´´ٰ� ��
	// 
	arrowPos -= pos;
	arrowPos.x = arrowPos.x / scale.x * 2.f;
	arrowPos.y = arrowPos.y / scale.y * 2.f;
	Vec3 reflectDir;

	// ���� ���� �¾��� �� 
	if (1.f <= arrowPos.x && -1.f< arrowPos.y && arrowPos.y <1.f)
	{
		reflectDir.x = cos(angle * 3.141592f / 180);
		reflectDir.y = sin(angle * 3.141592f / 180);
		reflectDir.Normalize();
		m_fAngle = angle * 3.141592f / 180;

		if (vRotate.y > 1)
			vRotate.y = 0;
		else
			vRotate.y = 3.141592f;
	}
	// �� ���� �¾��� �� - ������
	else if (-1.f >= arrowPos.y && -1.f < arrowPos.x && arrowPos.x < 1.f)
	{
		reflectDir.x = cos((-angle + 360) * 3.141592f / 180);
		reflectDir.y = sin((-angle + 360) * 3.141592f / 180);
		if (0 < reflectDir.y)
		{
			reflectDir.y *= -1;
			m_fAngle = (-angle + 360) * 3.141592f / 180 * -1;
		}
		else
			m_fAngle = (-angle + 360) * 3.141592f / 180;

		reflectDir.Normalize();
	}
	else if (1.f <= arrowPos.y && -1.f < arrowPos.x && arrowPos.x < 1.f)
	{
		reflectDir.x = cos((-angle+360) * 3.141592f / 180);
		reflectDir.y = sin((-angle+360) * 3.141592f / 180);
		if (0 > reflectDir.y)
		{
			reflectDir.y *= -1;
			m_fAngle = (-angle + 360) * 3.141592f / 180 * -1;
		}
		else
			m_fAngle = (-angle + 360) * 3.141592f / 180;
		reflectDir.Normalize();
	}
	// ������ ���� �¾��� ��
	else if (-1.f >= arrowPos.x && - 1.f < arrowPos.y && arrowPos.y < 1.f)
	{
		reflectDir.x = cos((180 - angle) * 3.141592f / 180);
		reflectDir.y = sin((180 - angle) * 3.141592f / 180);
		reflectDir.Normalize();
		m_fAngle = (180 - angle) * 3.141592f / 180;

		if (vRotate.y > 1)
			vRotate.y = 0;
		else
			vRotate.y = 3.141592f;
	}

	vRotate.z = m_fAngle;
	Transform()->SetRelativeRotation(vRotate);

	return reflectDir;
}
