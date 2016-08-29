#pragma once

class CGameData;
class CKeyBoard;
#include <Eigen/Eigen>

class CCamera
{
public:
	CCamera( const CGameData* pGameData, const CKeyBoard* pKeyBoard );
	void Update();
	
	float GetX() const { return _pos_x; }
	float GetY() const { return _pos_y; }
	float GetZ() const { return _pos_z; }

	float GetCameraRotationX() const;
	float GetCameraRotationY() const;

	void MouseButton(int button, int state, int x, int y);
	void ProcessNormalKeys(unsigned char key, int xx, int yy);
	void ProcessNormalKeysUp(unsigned char key, int xx, int yy);
	void PressKey(int key, int xx, int yy);
	void ReleaseKey(int key, int x, int y);
	void MouseMove(int x, int y);

	void GetObjectUnderPos(int x, int y);
	float*	AccessViewMatrix() { return _view_matrix; }

	Eigen::Vector3f GetLastClickPosOnMap() const 
	{
		Eigen::Vector3f returnvalue;
		returnvalue << _last_mouse_click_pos_on_map_x, _last_mouse_click_pos_on_map_y, _last_mouse_click_pos_on_map_z;
		return returnvalue;
	}

private:
	const CGameData* _p_gamedata;
	const CKeyBoard* _p_keyboard;
	// Position

	float _view_matrix[16];

	float _pos_x;
	float _pos_y;
	float _pos_z;

	float _last_mouse_click_pos_on_map_x;
	float _last_mouse_click_pos_on_map_y;
	float _last_mouse_click_pos_on_map_z;

	float _key_pan_speed;
	float _mouse_zoom_speed;

	float _key_pan_since_last_update_x;
	float _key_pan_since_last_update_z;

	// Rotation
	bool _is_changing_rotation;
	float _rotation_x;
	float _rotation_change_x;
	float _rotation_change_origin_y;
	float _rotation_y;
	float _rotation_change_y;
	float _rotation_change_origin_x;
};