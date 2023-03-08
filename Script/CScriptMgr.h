#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	BOSSMONSTERUISCRIPT,
	BOSS_WOLF,
	CAMERAMOVESCRIPT,
	EFFECTSCRIPT,
	GRAVITY,
	GUARDSCRIPT,
	MISSILESCRIPT,
	MONSTERARROW,
	MONSTERSCRIPT,
	MONSTERUISCRIPT,
	MONSTER_ATTACK_COLLIDER,
	NOTMOVESCRIPT,
	PARTICLESCRIPT,
	PLAYERARROW,
	PLAYERSCRIPT,
	PLAYERUISCRIPT,
	PLAYER_WEAPON_COLLIDER,
	SCENECHANGESCRIPT,
	SCENESCRIPT_01,
	SCENESCRIPT_02,
	SCENESCRIPT_03,
	SCENESCRIPT_START,
	SCENE_BOSS,
	SHADOWWOLFSCRIPT,
	SKELETON_ARCHER,
	SKELETON_LIZARD,
	STONEEYE,
	WALL,
	WOLF,
	WOODGOLEM,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
