#include "gamedata.h"
#include "miniature.h"

CGameData::CGameData()
	: _nSelectedMiniature(-1)
{

}

void CGameData::Init()
{
	InitMiniatures();
}

void CGameData::InitMiniatures()
{
	_Miniatures.push_back(new CMiniature(-10.0f, 20.0f, true));
	_Miniatures.push_back(new CMiniature( 10.0f, 20.0f, true));

	_Miniatures.push_back(new CMiniature( 0.0f, -20.0f, false));

}

void CGameData::SelectMiniature(int nIndex)
{
	_nSelectedMiniature = nIndex;
}

void CGameData::DeselectMiniature()
{
	_nSelectedMiniature = -1;
}

const CMiniature* CGameData::GetSelectedMiniature() const
{
	if (_nSelectedMiniature >= 0 && _nSelectedMiniature < _Miniatures.size() )
		return _Miniatures[_nSelectedMiniature];
	else
		return 0;
}