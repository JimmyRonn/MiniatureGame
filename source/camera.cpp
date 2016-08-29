#include "camera.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <Eigen/Eigen>

#include "freeglut_includes.h"
#include "defines.h"
#include "keyboard.h"

CCamera::CCamera( const CGameData* pGameData, const CKeyBoard* pKeyBoard )
	: _p_gamedata(pGameData), _p_keyboard( pKeyBoard )
{
	_pos_x = 0.0f;
	_pos_z = 5.0f;
	_pos_y = 10.0f;
	
	_key_pan_speed = 0.1f;
	_mouse_zoom_speed = 2.0f;

	_key_pan_since_last_update_x = 0;
	_key_pan_since_last_update_z = 0;

	_is_changing_rotation = false;
	_rotation_x = 90.0f;
	_rotation_change_x = 0.0f;
	_rotation_change_origin_y = 0;
	_rotation_y = 0.0f;
	_rotation_change_y = 0.0f;
	_rotation_change_origin_x = 0;
}

float CCamera::GetCameraRotationX() const 
{ 
	return _rotation_x - _rotation_change_x; 
}

float CCamera::GetCameraRotationY() const
{
	return _rotation_y - _rotation_change_y;
}

void CCamera::Update() 
{
	if (_p_keyboard->IsKeyDown(27))
		exit(0);

	if (_p_keyboard->IsSpecialKeyDown(GLUT_KEY_UP) || _p_keyboard->IsKeyDown(119) )
	{
		if(_key_pan_since_last_update_z == 0 )
			_key_pan_since_last_update_z = -_key_pan_speed;
	}
	else if (_p_keyboard->IsSpecialKeyDown(GLUT_KEY_DOWN) || _p_keyboard->IsKeyDown(115))
	{
		if (_key_pan_since_last_update_z == 0)
			_key_pan_since_last_update_z = _key_pan_speed;
	}
	else
	{
		_key_pan_since_last_update_z = 0;
	}

	if (_p_keyboard->IsSpecialKeyDown(GLUT_KEY_LEFT) || _p_keyboard->IsKeyDown(97) )
	{
		if (_key_pan_since_last_update_x == 0)
			_key_pan_since_last_update_x = -_key_pan_speed;
	}
	else if (_p_keyboard->IsSpecialKeyDown(GLUT_KEY_RIGHT) || _p_keyboard->IsKeyDown(100))
	{
		if (_key_pan_since_last_update_x == 0)
			_key_pan_since_last_update_x = _key_pan_speed;
	}
	else
	{
		_key_pan_since_last_update_x = 0;
	}

	if ( !_is_changing_rotation && _p_keyboard->IsSpecialKeyDown(SPECIAL_KEY_SHIFT_L))
	{
		_rotation_change_origin_x = 0;
		_rotation_change_origin_y = 0;
		_is_changing_rotation = true;
	}
	else if( _is_changing_rotation && !_p_keyboard->IsSpecialKeyDown(SPECIAL_KEY_SHIFT_L))
	{
		_is_changing_rotation = false;
		_rotation_x -= _rotation_change_x;
		_rotation_change_x = 0;
		_rotation_y -= _rotation_change_y;
		_rotation_change_y = 0;
	}


	if (_key_pan_since_last_update_x || _key_pan_since_last_update_z)
	{
		_pos_x += _key_pan_since_last_update_x;
		_pos_z += _key_pan_since_last_update_z;
	}
}

void CCamera::MouseButton(int button, int state, int x, int y) {

	if (button == 3 || button == 4)
	{
		if (state == GLUT_DOWN)
		{
			if (button == 3)
				_pos_y -= _mouse_zoom_speed;
			else if (button == 4)
				_pos_y += _mouse_zoom_speed;
		}
	}
}

void CCamera::MouseMove(int x, int y)
{
	if (_is_changing_rotation)
	{
		if (_rotation_change_origin_y == 0)
			_rotation_change_origin_y = (float)y;
		_rotation_change_x = _rotation_change_origin_y - y;

		if (_rotation_change_origin_x == 0)
			_rotation_change_origin_x = (float)x;
		_rotation_change_y = _rotation_change_origin_x - x;
	}
}

void CCamera::GetObjectUnderPos(int mouse_x, int mouse_y)
{
	float x = (2.0f * (float)mouse_x) / (float)SCREEN_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * (float)mouse_y) / (float)SCREEN_HEIGHT;
	float z = GetZ();

	Eigen::Vector3f ray_nds(x, y, z);
	Eigen::Vector4f ray_clip(ray_nds[0], ray_nds[1], -1.0f, 1.0f);

	Eigen::Matrix4f projection_matrix;

	float aspect = 1028.0f / 768.0f;
	float f = 1.0f / tan( (FOV / 2.0f) * (M_PI / 180.0f) );
	projection_matrix <<
		f, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, -Z_FAR / (Z_FAR - Z_NEAR), -1.0f,
		0.0f, 0.0f, -((Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR)), 0.0f;

	/*projection_matrix << 1.f / (float)SCREEN_WIDTH, 0, 0, 0,
		0, 1.f / (float)SCREEN_HEIGHT, 0, 0,
		0, 0, -2.f / (float)(Z_FAR - Z_NEAR), -(float)(Z_FAR + Z_NEAR) / (float)(Z_FAR - Z_NEAR),
		0, 0, 0, 1.f;*/


	Eigen::Vector4f ray_eye = projection_matrix.inverse() * ray_clip;
	ray_eye[2] = -1.0f;
	ray_eye[3] = 0.0f;

	Eigen::Vector4f view_matrix(Eigen::Matrix4f(_view_matrix).inverse() * ray_eye);
	Eigen::Vector3f ray_wor;
	ray_wor << view_matrix[0], view_matrix[1], view_matrix[2];
	ray_wor.normalize();

	Eigen::Vector3f last_mouse_click_on_map;
	last_mouse_click_on_map << _pos_x, _pos_y, _pos_z;

	float interval = 0.1f;
	while (last_mouse_click_on_map[1] > 0)
	{

		last_mouse_click_on_map += ray_wor * interval;
		//interval += 1.0f;
	}

	//last_mouse_click_on_map[0] *= aspect;
	//last_mouse_click_on_map[1] *= aspect;

	float vx = last_mouse_click_on_map[0];
	float vy = last_mouse_click_on_map[1];
	float vz = last_mouse_click_on_map[2];

	_last_mouse_click_pos_on_map_x = last_mouse_click_on_map[0];
	_last_mouse_click_pos_on_map_y = last_mouse_click_on_map[1];
	_last_mouse_click_pos_on_map_z = last_mouse_click_on_map[2];
	
	int nTmp;
	nTmp = 0;
}
