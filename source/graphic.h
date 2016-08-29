#pragma once

#include "vertexPoint.h"

class CTexture;
class CMiniature;
class CGameData;
class CCamera;

class CGraphic
{
public: 
	CGraphic( CCamera* pCamera );

	void Render( const CGameData& GameData );
	void RenderMiniature(const CMiniature& Miniature, int nColorOverload = 0);

	void ProcessSelection(int x, int y, CGameData& GameData);

	void ChangeSize(int w, int h);

private:
	void LoadMap();
	void RenderDebugStuff();

	void TranslateToCameraPosition();
	void DrawGround();
	void DrawMiniatures(const CGameData& GameData);

	CTexture* _p_map_texture; //File loaded texture
	CCamera* _p_camera;
							
	//Quad vertices
	LVertexPos3D _quad_vertices[8];
	LVertexPos3D _quad_vertices_helper[4];
	GLUquadricObj* _p_pillar_quad;
};