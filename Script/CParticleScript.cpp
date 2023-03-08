#include "pch.h"
#include "CParticleScript.h"

CParticleScript::CParticleScript()
	:CScript((int)SCRIPT_TYPE::PARTICLESCRIPT)
	, m_fLifeTime(0.f)
{
}

CParticleScript::~CParticleScript()
{
}

void CParticleScript::update()
{
	m_fLifeTime += DT;

	if (3.f <= m_fLifeTime)
		GetOwner()->Destroy();
}

void CParticleScript::lateupdate()
{
}
