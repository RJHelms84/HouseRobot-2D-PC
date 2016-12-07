#ifndef _GAME_H
#define _GAME_H

//windows/directx headers
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <dsound.h>
#include <dinput.h>
#include <Windows.h>
#include <d3dx9math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//framework specific headers
#include "dxgraphics.h"
#include "dxaudio.h"
#include "dxinput.h"

#include "Database.h"
#include "NavGraph.h"
#include "FloodFillNG.h"

//custom windows message for restarting game
#define WM_HR_RESTART (WM_USER+1)
#define WM_HR_CAUGHTPLAYER (WM_USER+2)

//application title
#define APPTITLE "House Robot"

//screen setup
#define FULLSCREEN 0
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540

//World dimensions
#define GAME_WIDTH 1500
#define GAME_HEIGHT 1500

//gameplay states
#define TITLESCREEN 0
#define GAMESCREEN 1
#define PAUSESCREEN 2

//move list
#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_DOWN 2
#define MOVE_UP 3

//sprite structure
typedef struct
{
	int x,y;
	int width, height;
	int movex, movey;
	int curframe, lastframe;
	int animdelay, animcount;
	int scalex, scaley;
	int rotation, rotaterate;
	LPDIRECT3DTEXTURE9 image;
	int r;
	int g;
	int b;
	int a;
} SPRITE;

class Element;
class Timer;
class Level;
class PLAYER;
class ROBOT;

class Game
{
private:
	//database object
	Database* dbObject;

	//gameplay state var
	int GAME_STATE;
	//sprite handler
	LPD3DXSPRITE sprite_handler;
	HRESULT result;

	//level object
	Level* level;

	//navigation graph
	NavGraph* robotNavGraph;
	FloodFillNG* filler;

	//timer for fps of game
	Timer* timerObject;

	//background image
	LPDIRECT3DSURFACE9 gameBackground;
	LPDIRECT3DTEXTURE9 fow;//fog of war
	//actors
	PLAYER* playerActor;
	ROBOT* robotActor;
	//offset vars for sprites
	int currentOffsetX;
	int currentOffsetY;

public:
	Game(HWND hwnd, Database* dbObj);
	~Game();

	//collision detection
	int Collision(SPRITE*, SPRITE*);
	int Collision(SPRITE*, Element*);
	void CheckPlayerCollisions();
	void CheckRobotCollisions();

	//lost sprite handle prototpe
	void ResetSpriteDevice();

	int Game_Init(HWND);
	void Init_Elements();
	void Game_Run(HWND);
	void Game_Update();
	void Game_Render();
	void Render_Backgrounds();
	void Render_Actors();
	void Game_End(HWND);
};

#endif