#include "pch.h"
#include "CScriptMgr.h"

#include "CBossMonsterUIScript.h"
#include "CBoss_Wolf.h"
#include "CCameraMoveScript.h"
#include "CEffectScript.h"
#include "CGravity.h"
#include "CGuardScript.h"
#include "CMissileScript.h"
#include "CMonsterArrow.h"
#include "CMonsterScript.h"
#include "CMonsterUIScript.h"
#include "CMonster_Attack_Collider.h"
#include "CNotMoveScript.h"
#include "CParticleScript.h"
#include "CPlayerArrow.h"
#include "CPlayerScript.h"
#include "CPlayerUIScript.h"
#include "CPlayer_Weapon_Collider.h"
#include "CSceneChangeScript.h"
#include "CSceneScript_01.h"
#include "CSceneScript_02.h"
#include "CSceneScript_03.h"
#include "CSceneScript_Start.h"
#include "CScene_Boss.h"
#include "CShadowWolfScript.h"
#include "CSkeleton_Archer.h"
#include "CSkeleton_Lizard.h"
#include "CStoneEye.h"
#include "CWall.h"
#include "CWolf.h"
#include "CWoodGolem.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBossMonsterUIScript");
	_vec.push_back(L"CBoss_Wolf");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CGravity");
	_vec.push_back(L"CGuardScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterArrow");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMonsterUIScript");
	_vec.push_back(L"CMonster_Attack_Collider");
	_vec.push_back(L"CNotMoveScript");
	_vec.push_back(L"CParticleScript");
	_vec.push_back(L"CPlayerArrow");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerUIScript");
	_vec.push_back(L"CPlayer_Weapon_Collider");
	_vec.push_back(L"CSceneChangeScript");
	_vec.push_back(L"CSceneScript_01");
	_vec.push_back(L"CSceneScript_02");
	_vec.push_back(L"CSceneScript_03");
	_vec.push_back(L"CSceneScript_Start");
	_vec.push_back(L"CScene_Boss");
	_vec.push_back(L"CShadowWolfScript");
	_vec.push_back(L"CSkeleton_Archer");
	_vec.push_back(L"CSkeleton_Lizard");
	_vec.push_back(L"CStoneEye");
	_vec.push_back(L"CWall");
	_vec.push_back(L"CWolf");
	_vec.push_back(L"CWoodGolem");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBossMonsterUIScript" == _strScriptName)
		return new CBossMonsterUIScript;
	if (L"CBoss_Wolf" == _strScriptName)
		return new CBoss_Wolf;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CGravity" == _strScriptName)
		return new CGravity;
	if (L"CGuardScript" == _strScriptName)
		return new CGuardScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterArrow" == _strScriptName)
		return new CMonsterArrow;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMonsterUIScript" == _strScriptName)
		return new CMonsterUIScript;
	if (L"CMonster_Attack_Collider" == _strScriptName)
		return new CMonster_Attack_Collider;
	if (L"CNotMoveScript" == _strScriptName)
		return new CNotMoveScript;
	if (L"CParticleScript" == _strScriptName)
		return new CParticleScript;
	if (L"CPlayerArrow" == _strScriptName)
		return new CPlayerArrow;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerUIScript" == _strScriptName)
		return new CPlayerUIScript;
	if (L"CPlayer_Weapon_Collider" == _strScriptName)
		return new CPlayer_Weapon_Collider;
	if (L"CSceneChangeScript" == _strScriptName)
		return new CSceneChangeScript;
	if (L"CSceneScript_01" == _strScriptName)
		return new CSceneScript_01;
	if (L"CSceneScript_02" == _strScriptName)
		return new CSceneScript_02;
	if (L"CSceneScript_03" == _strScriptName)
		return new CSceneScript_03;
	if (L"CSceneScript_Start" == _strScriptName)
		return new CSceneScript_Start;
	if (L"CScene_Boss" == _strScriptName)
		return new CScene_Boss;
	if (L"CShadowWolfScript" == _strScriptName)
		return new CShadowWolfScript;
	if (L"CSkeleton_Archer" == _strScriptName)
		return new CSkeleton_Archer;
	if (L"CSkeleton_Lizard" == _strScriptName)
		return new CSkeleton_Lizard;
	if (L"CStoneEye" == _strScriptName)
		return new CStoneEye;
	if (L"CWall" == _strScriptName)
		return new CWall;
	if (L"CWolf" == _strScriptName)
		return new CWolf;
	if (L"CWoodGolem" == _strScriptName)
		return new CWoodGolem;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BOSSMONSTERUISCRIPT:
		return new CBossMonsterUIScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_WOLF:
		return new CBoss_Wolf;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITY:
		return new CGravity;
		break;
	case (UINT)SCRIPT_TYPE::GUARDSCRIPT:
		return new CGuardScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERARROW:
		return new CMonsterArrow;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERUISCRIPT:
		return new CMonsterUIScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER_ATTACK_COLLIDER:
		return new CMonster_Attack_Collider;
		break;
	case (UINT)SCRIPT_TYPE::NOTMOVESCRIPT:
		return new CNotMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLESCRIPT:
		return new CParticleScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERARROW:
		return new CPlayerArrow;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERUISCRIPT:
		return new CPlayerUIScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER_WEAPON_COLLIDER:
		return new CPlayer_Weapon_Collider;
		break;
	case (UINT)SCRIPT_TYPE::SCENECHANGESCRIPT:
		return new CSceneChangeScript;
		break;
	case (UINT)SCRIPT_TYPE::SCENESCRIPT_01:
		return new CSceneScript_01;
		break;
	case (UINT)SCRIPT_TYPE::SCENESCRIPT_02:
		return new CSceneScript_02;
		break;
	case (UINT)SCRIPT_TYPE::SCENESCRIPT_03:
		return new CSceneScript_03;
		break;
	case (UINT)SCRIPT_TYPE::SCENESCRIPT_START:
		return new CSceneScript_Start;
		break;
	case (UINT)SCRIPT_TYPE::SCENE_BOSS:
		return new CScene_Boss;
		break;
	case (UINT)SCRIPT_TYPE::SHADOWWOLFSCRIPT:
		return new CShadowWolfScript;
		break;
	case (UINT)SCRIPT_TYPE::SKELETON_ARCHER:
		return new CSkeleton_Archer;
		break;
	case (UINT)SCRIPT_TYPE::SKELETON_LIZARD:
		return new CSkeleton_Lizard;
		break;
	case (UINT)SCRIPT_TYPE::STONEEYE:
		return new CStoneEye;
		break;
	case (UINT)SCRIPT_TYPE::WALL:
		return new CWall;
		break;
	case (UINT)SCRIPT_TYPE::WOLF:
		return new CWolf;
		break;
	case (UINT)SCRIPT_TYPE::WOODGOLEM:
		return new CWoodGolem;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BOSSMONSTERUISCRIPT:
		return L"CBossMonsterUIScript";
		break;

	case SCRIPT_TYPE::BOSS_WOLF:
		return L"CBoss_Wolf";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::GRAVITY:
		return L"CGravity";
		break;

	case SCRIPT_TYPE::GUARDSCRIPT:
		return L"CGuardScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERARROW:
		return L"CMonsterArrow";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MONSTERUISCRIPT:
		return L"CMonsterUIScript";
		break;

	case SCRIPT_TYPE::MONSTER_ATTACK_COLLIDER:
		return L"CMonster_Attack_Collider";
		break;

	case SCRIPT_TYPE::NOTMOVESCRIPT:
		return L"CNotMoveScript";
		break;

	case SCRIPT_TYPE::PARTICLESCRIPT:
		return L"CParticleScript";
		break;

	case SCRIPT_TYPE::PLAYERARROW:
		return L"CPlayerArrow";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERUISCRIPT:
		return L"CPlayerUIScript";
		break;

	case SCRIPT_TYPE::PLAYER_WEAPON_COLLIDER:
		return L"CPlayer_Weapon_Collider";
		break;

	case SCRIPT_TYPE::SCENECHANGESCRIPT:
		return L"CSceneChangeScript";
		break;

	case SCRIPT_TYPE::SCENESCRIPT_01:
		return L"CSceneScript_01";
		break;

	case SCRIPT_TYPE::SCENESCRIPT_02:
		return L"CSceneScript_02";
		break;

	case SCRIPT_TYPE::SCENESCRIPT_03:
		return L"CSceneScript_03";
		break;

	case SCRIPT_TYPE::SCENESCRIPT_START:
		return L"CSceneScript_Start";
		break;

	case SCRIPT_TYPE::SCENE_BOSS:
		return L"CScene_Boss";
		break;

	case SCRIPT_TYPE::SHADOWWOLFSCRIPT:
		return L"CShadowWolfScript";
		break;

	case SCRIPT_TYPE::SKELETON_ARCHER:
		return L"CSkeleton_Archer";
		break;

	case SCRIPT_TYPE::SKELETON_LIZARD:
		return L"CSkeleton_Lizard";
		break;

	case SCRIPT_TYPE::STONEEYE:
		return L"CStoneEye";
		break;

	case SCRIPT_TYPE::WALL:
		return L"CWall";
		break;

	case SCRIPT_TYPE::WOLF:
		return L"CWolf";
		break;

	case SCRIPT_TYPE::WOODGOLEM:
		return L"CWoodGolem";
		break;
	}
	return nullptr;
}