#include "pch.h"
#include "CGravity.h"

#include <Engine/CTransform.h>
CGravity::CGravity()
	:CScript((int)SCRIPT_TYPE::GRAVITY)
    , m_vCurForce()
    , m_vVelocity()
    , m_vGravityForce(Vec2(0.f, -600.f))
    , m_fSpeed(0.f)
    , m_fMass(1.f)
    , m_fMaxXSpeed(300.f)
    , m_fMaxYSpeed(300.f)
    , m_fFriction(500.f)
    , m_bGravityActive(false)
    , m_bGround(false)
{
    SetMaxXSpeed(300.f);
    SetMaxYSpeed(600.f);
    SetMass(1.f);
    EnableGravity();
}

CGravity::CGravity(const CGravity& _origin)
    :CScript((int)SCRIPT_TYPE::GRAVITY)
    , m_vCurForce()
    , m_vVelocity()
    , m_vGravityForce(_origin.m_vGravityForce)
    , m_fSpeed(_origin.m_fSpeed)
    , m_fMass(_origin.m_fMass)
    , m_fMaxXSpeed(_origin.m_fMaxXSpeed)
    , m_fMaxYSpeed(_origin.m_fMaxYSpeed)
    , m_fFriction(_origin.m_fFriction)
    , m_bGravityActive(_origin.m_bGravityActive)
    , m_bGround(_origin.m_bGround)
{
    SetMaxXSpeed(300.f);
    SetMaxYSpeed(600.f);
    SetMass(1.f);
    EnableGravity();
}

CGravity::~CGravity()
{
}

void CGravity::start()
{

}

void CGravity::update()
{
    // 중력 체크
    if (m_bGravityActive && !m_bGround)
    {
        m_vCurForce += m_vGravityForce;
    }

    // 이번 프레임의 가속도 ( F = M * A )
    Vec2 vAccel = m_vCurForce / m_fMass;

    // 속도
    m_vVelocity += vAccel * DT;

    // 최대 속도 축별 제한
    if (m_fMaxXSpeed < abs(m_vVelocity.x))
    {
        m_vVelocity.x > 0 ? m_vVelocity.x = m_fMaxXSpeed : m_vVelocity.x = -m_fMaxXSpeed;
    }

    if (m_fMaxYSpeed < abs(m_vVelocity.y))
    {
        m_vVelocity.y > 0 ? m_vVelocity.y = m_fMaxYSpeed : m_vVelocity.y = -m_fMaxYSpeed;
    }
    
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    // 속도로 이동
    //Vec2 vPos = GetOwner()->GetPos();
    vPos.x += m_vVelocity.x * DT;
    vPos.y += m_vVelocity.y * DT;
    GetOwner()->Transform()->SetRelativePos(vPos);
    m_vCurForce = Vec2(0.f, 0.f);
}

void CGravity::lateupdate()
{
}

void CGravity::OnGround()
{
    m_bGround = true;
    m_vVelocity.y = 0.f;

    //GetOwner()->OnGround();
}

void CGravity::OnAir()
{
    m_bGround = false;
}
