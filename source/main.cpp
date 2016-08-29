#pragma warning (disable : 4244)

#include <stdlib.h>
#include <math.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include "defines.h"
#include "gamedata.h"
#include "graphic.h"
#include "camera.h"
#include "keyboard.h"
#include "miniature.h"
#include "texture.h"
#include "vertexPoint.h"

CGameData* p_gamedata = 0;
CGraphic* p_graphic = 0;
CCamera* p_camera = 0;
CKeyBoard* p_keyboard = 0;
int mouse_pos_x = 0;
int mouse_pos_y = 0;

void changeSize(int w, int h) 
{
	p_graphic->ChangeSize(w, h);
}

const int BUF_SIZE = 1024;
GLuint buffer[BUF_SIZE];

void renderScene(void) 
{
	p_camera->Update();
	p_graphic->Render(*p_gamedata);
}

void pressKey(int key, int xx = 0, int yy = 0); 

void processNormalKeys(unsigned char key, int xx, int yy) 
{
	p_keyboard->SetNormalKeyDown(key);
	//pCamera->ProcessNormalKeys(key, xx, yy);
}

void processNormalKeysUp(unsigned char key, int xx, int yy) 
{
	p_keyboard->SetNormalKeyUp(key);
	//pCamera->ProcessNormalKeysUp(key, xx, yy);
}

void pressKey(int key, int xx, int yy) 
{
	p_keyboard->SetSpecialKeyDown(key);
	//pCamera->PressKey(key, xx, yy);
}

void releaseKey(int key, int x, int y) 
{
	p_keyboard->SetSpecialKeyUp(key);
	//pCamera->ReleaseKey(key, x, y);
}


void mouseMove(int x, int y) 
{	
	p_camera->MouseMove(x, y);
	mouse_pos_x = x - (SCREEN_WIDTH/2);
	mouse_pos_y = SCREEN_HEIGHT - y - (SCREEN_HEIGHT/2);
}

void mouseButton(int button, int state, int x, int y) {

	p_camera->MouseButton(button, state, x, y);
	if(button != 3 && button != 4)
		p_graphic->ProcessSelection(x, y, *p_gamedata);
}

int main(int argc, char **argv) {

	p_gamedata = new CGameData();
	p_gamedata->Init();

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	//Initialize DevIL
	ilInit();
	ilClearColour(255, 255, 255, 000);

	//Check for error
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		printf("Error initializing DevIL! %s\n", iluErrorString(ilError));
		return false;
	}


	//Enable texturing
	glEnable(GL_TEXTURE_2D);


	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	p_keyboard = new CKeyBoard();
	p_camera = new CCamera( p_gamedata, p_keyboard );
	p_graphic = new CGraphic( p_camera );

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}