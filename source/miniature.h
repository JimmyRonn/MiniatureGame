#pragma once

class CMiniature
{
public:
	CMiniature()
		: _vX(0), _vY(0), _bIsRed(false), _vSize(1.0f)
	{}
	CMiniature(float nX, float nY)
		: _vX(nX), _vY(nY), _bIsRed(false), _vSize(1.0f)
	{}
	CMiniature(float nX, float nY, bool bIsRed)
		: _vX(nX), _vY(nY), _bIsRed(bIsRed), _vSize(1.0f)
	{}

	bool IsRed() const { return _bIsRed; }
	void SetIsRed(bool bIsRed) { _bIsRed = bIsRed; }

	float GetX() const { return _vX; }
	float GetY() const { return _vY; }

	float GetSize() const { return _vSize; }

private:
	bool	_bIsRed;
	float	_vX;
	float	_vY;
	float	_vSize;
};