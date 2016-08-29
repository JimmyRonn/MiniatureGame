#include "graphic.h"

#include <stdlib.h>
#include <vector>
#include "freeglut_includes.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <Eigen/eigen>

#include "defines.h"
#include "texture.h"
#include "miniature.h"
#include "gamedata.h"
#include "camera.h"

CGraphic::CGraphic( CCamera* pCamera )
	: _p_map_texture(0), _p_camera( pCamera )
{	
	_p_pillar_quad = gluNewQuadric();

	LoadMap();
}

void CGraphic::RenderDebugStuff()
{
	/*glPushMatrix();

	Eigen::Vector3f position = _p_camera->GetLastClickPosOnMap();

	glTranslatef(position[0], position[1], position[2]);
	gluSphere(_p_pillar_quad, 1.0f, 16, 16);

	glPopMatrix();*/

}

void CGraphic::TranslateToCameraPosition()
{
	glTranslatef(0, 0, -_p_camera->GetY());
	glRotatef(_p_camera->GetCameraRotationX(), 1.0f, 0.0f, 0.0f);
	glRotatef(_p_camera->GetCameraRotationY(), 0.0f, 1.0f, 0.0f);
	glTranslatef(-_p_camera->GetX(), -10.0f, -_p_camera->GetZ());
}

void CGraphic::DrawGround()
{
	float vGroundSize = 100.0f;
	_p_map_texture->render(-vGroundSize / 2.0f, 0.0f, -vGroundSize / 2.0f, vGroundSize, vGroundSize);
}

void CGraphic::DrawMiniatures(const CGameData& GameData)
{
	const std::vector<CMiniature*>& Miniatures = GameData.GetMiniatures();
	for (int i = 0, nSize = Miniatures.size(); i < nSize; ++i)
	{
		const CMiniature& Miniature = *Miniatures[i];
		RenderMiniature(Miniature);
	}
}

void CGraphic::Render(const CGameData& GameData)
{
	// Clear Color and Depth Buffers
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// Translate to camera position
	TranslateToCameraPosition();

	glGetFloatv(GL_MODELVIEW_MATRIX, _p_camera->AccessViewMatrix() );
	
	// Draw debug stuff
	RenderDebugStuff();

	// Draw ground
	DrawGround();
	
	// Draw miniatures
	DrawMiniatures(GameData);

	glutSwapBuffers();
}

void CGraphic::RenderMiniature( const CMiniature& Miniature, int nColorOverload )
{
	glPushMatrix();
	glTranslatef(Miniature.GetX(), 0, Miniature.GetY());

	const float vHalfSize = Miniature.GetSize() * 0.5f;
	const float vHeight = Miniature.GetSize() * 0.25f;

	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

	gluCylinder(_p_pillar_quad, vHeight * 0.25f, vHeight * 0.25f, 2.5f, 16, 16);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	if (nColorOverload > 0)
		glColor3ub(nColorOverload, nColorOverload, nColorOverload);
	else if (Miniature.IsRed())
		glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
	else
		glColor4f(0.0f, 0.0f, 1.0f, 0.0f);

	_quad_vertices[0].x = -vHalfSize;
	_quad_vertices[0].y = vHeight;
	_quad_vertices[0].z = -vHalfSize;
	_quad_vertices[1].x = vHalfSize;
	_quad_vertices[1].y = vHeight;
	_quad_vertices[1].z = -vHalfSize;
	_quad_vertices[2].x = vHalfSize;
	_quad_vertices[2].y = vHeight;
	_quad_vertices[2].z = vHalfSize;
	_quad_vertices[3].x = -vHalfSize;
	_quad_vertices[3].y = vHeight;
	_quad_vertices[3].z = vHalfSize;
	_quad_vertices[4].x = -vHalfSize;
	_quad_vertices[4].y = 0.0f;
	_quad_vertices[4].z = vHalfSize;
	_quad_vertices[5].x = vHalfSize;
	_quad_vertices[5].y = 0.0f;
	_quad_vertices[5].z = vHalfSize;
	_quad_vertices[6].x = vHalfSize;
	_quad_vertices[6].y = 0.0f;
	_quad_vertices[6].z = -vHalfSize;
	_quad_vertices[7].x = -vHalfSize;
	_quad_vertices[7].y = 0.0f;
	_quad_vertices[7].z = -vHalfSize;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _quad_vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glDrawArrays(GL_QUADS, 4, 4);

	glVertexPointer(3, GL_FLOAT, 0, _quad_vertices_helper);

	//Bottom
	_quad_vertices_helper[0] = _quad_vertices[3];
	_quad_vertices_helper[1] = _quad_vertices[2];
	_quad_vertices_helper[2] = _quad_vertices[5];
	_quad_vertices_helper[3] = _quad_vertices[4];
	glDrawArrays(GL_QUADS, 0, 4);

	//Right
	_quad_vertices_helper[0] = _quad_vertices[2];
	_quad_vertices_helper[1] = _quad_vertices[1];
	_quad_vertices_helper[2] = _quad_vertices[6];
	_quad_vertices_helper[3] = _quad_vertices[5];
	glDrawArrays(GL_QUADS, 0, 4);

	//Top
	_quad_vertices_helper[0] = _quad_vertices[1];
	_quad_vertices_helper[1] = _quad_vertices[0];
	_quad_vertices_helper[2] = _quad_vertices[7];
	_quad_vertices_helper[3] = _quad_vertices[6];
	glDrawArrays(GL_QUADS, 0, 4);

	//Left
	_quad_vertices_helper[0] = _quad_vertices[0];
	_quad_vertices_helper[1] = _quad_vertices[3];
	_quad_vertices_helper[2] = _quad_vertices[4];
	_quad_vertices_helper[3] = _quad_vertices[7];
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

unsigned char pixeldata[4];

void CGraphic::ProcessSelection(int x, int y, CGameData& GameData)
{
	// Clear Color and Depth Buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Translate to camera position
	TranslateToCameraPosition();

	// Draw miniatures
	int nColor = 1;
	int nColorInc = 1;
	const std::vector<CMiniature*>& Miniatures = GameData.GetMiniatures();
	for (int i = 0, nSize = Miniatures.size(); i < nSize; ++i)
	{
		const CMiniature& Miniature = *Miniatures[i];
		RenderMiniature(Miniature, nColor);
		nColor += nColorInc;
	}

	memset(pixeldata, 0, 4);

	glReadPixels(x, (int)SCREEN_HEIGHT - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixeldata);
	glReadBuffer(GL_BACK);

	if (pixeldata[0] != 255)
		GameData.SelectMiniature(pixeldata[0] - 1);
	else
		GameData.DeselectMiniature();
}

void CGraphic::LoadMap()
{
	if (_p_map_texture)
	{
		printf("Map texture already initialized.\n");
		return;
	}
	_p_map_texture = new CTexture();

	//Load texture
	if (!_p_map_texture->loadTextureFromFile("resources/space4.png"))
	{
		printf("Unable to load file texture!\n");
		delete _p_map_texture;
		_p_map_texture = nullptr;
	}
}

void CGraphic::ChangeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(FOV, ASPECT, Z_NEAR, Z_FAR);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

