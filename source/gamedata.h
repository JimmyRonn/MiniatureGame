#pragma once

#include <vector>

class CMiniature;

class CGameData
{
public:
	CGameData();

	void Init();

	void SelectMiniature(int nIndex);
	void DeselectMiniature();
	const CMiniature* GetSelectedMiniature() const;

	const std::vector<CMiniature*>& GetMiniatures() const { return _Miniatures; }

private:
	void InitMiniatures();

	std::vector<CMiniature*> _Miniatures;
	int _nSelectedMiniature;
};