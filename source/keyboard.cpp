#include "keyboard.h"

#include <string.h>
#include "freeglut_includes.h"

CKeyBoard::CKeyBoard()
{
	memset( &_normal_keys_states, 0, sizeof(char) * 128);
	memset( &_glut_special_keys_states, 0, sizeof(char) * MAX_SPECIAL_KEYS);
}

bool CKeyBoard::IsKeyDown(int key) const
{
	if (key < 0 || key >= 128)
		return false;
	return _normal_keys_states[key] != 0;
}

bool CKeyBoard::IsSpecialKeyDown(int key) const
{
	if (key < 0 || key >= MAX_SPECIAL_KEYS)
		return false;
	return _glut_special_keys_states[key] != 0;
}

void CKeyBoard::SetNormalKeyDown(int key)
{
	_normal_keys_states[key] = 1;
}

void CKeyBoard::SetNormalKeyUp(int key)
{
	_normal_keys_states[key] = 0;
}

void CKeyBoard::SetSpecialKeyDown(int key)
{
	_glut_special_keys_states[key] = 1;
}

void CKeyBoard::SetSpecialKeyUp(int key)
{
	_glut_special_keys_states[key] = 0;
}