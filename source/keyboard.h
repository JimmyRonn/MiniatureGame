#pragma once

#include "GL/freeglut.h"

const int MAX_SPECIAL_KEYS = 0x0075;

class CKeyBoard
{
public:
	CKeyBoard();

	bool IsKeyDown( int key ) const;
	bool IsSpecialKeyDown(int key) const;

	void SetNormalKeyDown(int key);
	void SetNormalKeyUp(int key);
	void SetSpecialKeyDown(int key);
	void SetSpecialKeyUp(int key);

private:
	char _normal_keys_states[128];
	char _glut_special_keys_states[MAX_SPECIAL_KEYS];
};