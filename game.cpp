#include <tchar.h>
#include <algorithm>
#include <functional>
#include "game.h"
#include "timer.h"
#include "ParticleEmitter.h"
#include "player.h"
#include "robot.h"
#include "Element.h"
#include "Level.h"

using namespace std;

Game::Game(HWND hwnd, Database* dbObj)
{
	dbObject = dbObj;

	Game_Init(hwnd);
}

Game::~Game()
{
	delete robotActor;
	robotActor = NULL;

	delete playerActor;
	playerActor = NULL;

	delete timerObject;
	timerObject = NULL;

	level->ClearElements();
	delete level;
	level = NULL;

	dbObject = NULL;//memory is freed in winmain
}

int Game::Collision(SPRITE* sprite1, SPRITE* sprite2)	//collision detection
{
	RECT rect1;
	rect1.left = sprite1->x+1+15;
	rect1.top = sprite1->y+1+5;
	rect1.right = sprite1->x + sprite1->width - 1 - 15;
	rect1.bottom = sprite1->y + sprite1->height - 1 - 5;

	RECT rect2;
	rect2.left = sprite2->x + 1+ 15;
	rect2.top = sprite2->y + 1 + 5;
	rect2.right = sprite2->x + sprite2->width - 1 - 15;
	rect2.bottom = sprite2->y + sprite2->height - 1 - 5;

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}

int Game::Collision(SPRITE* sprite1, Element* sprite2)	//collision detection
{
	RECT rect1;
	rect1.left = sprite1->x+1+15;
	rect1.top = sprite1->y+1;
	rect1.right = sprite1->x + sprite1->width - 1 - 15;
	rect1.bottom = sprite1->y + sprite1->height - 1;

	RECT rect2;
	rect2.left = sprite2->GetX() + 1;
	rect2.top = sprite2->GetY() + 1;
	rect2.right = sprite2->GetX() + sprite2->GetWidth() - 1;
	rect2.bottom = sprite2->GetY() + sprite2->GetHeight() - 1;

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}

void Game::CheckPlayerCollisions()
{
	for(int i=0; i < level->GetNextElementPos(); i++)
	{
		if(level->GetElement(i)->GetHasCollision())
		{
			if(Collision(&playerActor->GetSprite(), level->GetElement(i)))
			{
				playerActor->SetHasCollided();
				break;
			}
		}
	}

	if(!robotActor->GetIsDamaged())
	{
		if(Collision(&playerActor->GetSprite(), &robotActor->GetSprite()))
		{
			playerActor->SetHasCollided();
			robotActor->CaughtPlayer(playerActor);
		}
	}
}

void Game::CheckRobotCollisions()
{
	for(int i=0; i < level->GetNextElementPos(); i++)
	{
		if(level->GetElement(i)->GetHasCollision())
		{
			if(Collision(&robotActor->GetSprite(), level->GetElement(i)))
			{
				robotActor->SetHasCollided();
				break;
			}
		}
	}

	if(!robotActor->GetIsDamaged())
	{
		if(Collision(&playerActor->GetSprite(), &robotActor->GetSprite()))
		{
			robotActor->SetHasCollided();
			robotActor->CaughtPlayer(playerActor);
		}
	}
}

int Game::Game_Init(HWND hwnd)	//initialises the game
{
	currentOffsetX = 0;
	currentOffsetY = 0;

	GAME_STATE = GAMESCREEN;

	//init timer object
	timerObject = new Timer();

	//init level object
	level = new Level();

	//init elements
	Init_Elements();

	//init navigation graph
	robotNavGraph = new NavGraph();

	//populate navgraph
	filler = new FloodFillNG();
	//filler->Generate(robotNavGraph,level,GraphNode(0,0,Vector2(769,788)));
	filler->Generate_Basic(robotNavGraph);


	//load background images into surfaces
	gameBackground = LoadSurface("gameBackground.png", 0);
	if(gameBackground == NULL)
	{
		MessageBoxA(hwnd, "error initialising background texture", "error", MB_OK);
	}
	fow = LoadTexture("FOW.png", 0);
	if(fow == NULL)
	{
		MessageBoxA(hwnd, "error initialising fow texture", "error", MB_OK);
	}

	//init actors
	playerActor = new PLAYER(393,214);//starts at node 5
	robotActor = new ROBOT(782,422,*playerActor, hwnd, level, robotNavGraph);//starts at node 10


	//create sprite handler object
	result = D3DXCreateSprite(d3ddev, &sprite_handler);
	if(result != D3D_OK)
	{
		MessageBoxA(hwnd, "error creating sprite image", "error", MB_OK);
		return 0;
	}

	//return ok
	return 1;
}

void Game::Init_Elements()
{
	//wall collision elements
	level->CreateElement(0,0,28,1500,0,0,true);
	level->CreateElement(29,260,211,24,0,0,true);
	level->CreateElement(338,259,22,272,0,0,true);
	level->CreateElement(0,525,706,20,0,0,true);
	level->CreateElement(704,0,30,245,0,0,true);
	level->CreateElement(705,456,29,130,0,0,true);
	level->CreateElement(931,280,19,880,0,0,true);
	level->CreateElement(931,551,569,5,0,0,true);
	level->CreateElement(0,548,130,304,0,0,true);
	level->CreateElement(0,851,707,10,0,0,true);
	level->CreateElement(705,804,19,230,0,0,true);
	level->CreateElement(708,1246,19,199,0,0,true);
	level->CreateElement(933,1368,19,78,0,0,true);
	level->CreateElement(952,958,100,13,0,0,true);
	level->CreateElement(1201,955,231,25,0,0,true);
	level->CreateElement(0,0,1500,17,0,0,true);
	level->CreateElement(0,1443,1500,57,0,0,true);
	level->CreateElement(1469,0,31,552,0,0,true);
	level->CreateElement(1434,568,66,932,0,0,true);
	level->CreateElement(931,26,19,45,0,0,true);

	//pickups
	level->CreateElement(85,370,16,17,0,10,true,false,"waterglass.png",true,true, 1);
	level->CreateElement(1400,470,30,30,1,10,true,false,"hairdryer.png",true,true, 2);
}

void Game::Game_Run(HWND hwnd)	//main game loop, runs every frame
{
	if(d3ddev==NULL)
		return;
	//update keyboard
	Poll_Keyboard();

	//ONLY RUN IN GAMESCREEN
	if(GAME_STATE == GAMESCREEN)
	{
		//limit fps
		if(timerObject->StopWatch(17))	//runs update logic every 17 ms, this is about 60fps
		{
			Game_Update();
		}
	}

	//start rendering
	if(d3ddev->BeginScene())
	{
		Game_Render();

		d3ddev->EndScene();
	}

	//display the backbuffer on the screen
	d3ddev->Present(NULL,NULL,NULL,NULL);

	//check for escape key (to exit program)
	if(Key_Down(DIK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);

	//F1 to restart game
	if(Key_Down(DIK_F1))
		PostMessage(hwnd, WM_HR_RESTART, 0, 0);
}

void Game::Game_Update()
{
	//update actors----------------------------------------------------------

	//update player
	if(playerActor->GetRendered())
	{
		playerActor->Update(currentOffsetX, currentOffsetY, robotActor, *level);
		CheckPlayerCollisions();
	}

	//update robot
	if(robotActor->GetRendered())
	{
		robotActor->Update(currentOffsetX, currentOffsetY, *playerActor);
		//CheckRobotCollisions();
	}

	//update level object (player element activation)
	level->Update(playerActor);

	//END: update actors------------------------------------------------------
}

void Game::Game_Render()
{
	Render_Backgrounds();

	//begin sprite drawing
	sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

	Render_Actors();

	//end sprite drawing
	sprite_handler->End();
}

void Game::Render_Backgrounds()
{
	//set source rect
	RECT sr;
	sr.top = 0+(playerActor->GetY()-(SCREEN_HEIGHT/2));
	sr.left = 0+(playerActor->GetX()-(SCREEN_WIDTH/2));

	//if beyond level boundaries, move back
	if(sr.left > (GAME_WIDTH - SCREEN_WIDTH))
		sr.left = (GAME_WIDTH - SCREEN_WIDTH);
	else if(sr.left < 0)
		sr.left = 0;

	if(sr.top > (GAME_HEIGHT - SCREEN_HEIGHT))
		sr.top = (GAME_HEIGHT - SCREEN_HEIGHT);
	else if(sr.top < 0)
		sr.top = 0;

	sr.right = sr.left + SCREEN_WIDTH;
	sr.bottom = sr.top + SCREEN_HEIGHT;

	//offset vars for sprites
	currentOffsetX = sr.left;
	currentOffsetY = sr.top;

	//set dest rect
	RECT dr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	d3ddev->StretchRect(gameBackground, &sr, backbuffer, &dr, D3DTEXF_NONE);
}

void Game::Render_Actors()
{
	//render robot before the player, so player is displayed over robot
	//(this is important when robot is damaged, and player steps over him)
	if(robotActor->GetRendered())
	{
		robotActor->Draw(sprite_handler, currentOffsetX, currentOffsetY);
	}

	//render player
	if(playerActor->GetRendered())
	{
		playerActor->Draw(sprite_handler, currentOffsetX, currentOffsetY);
	}

	//Render level elements
	level->RenderElements(sprite_handler, currentOffsetX, currentOffsetY);

	//render fow
	//set dest rect
	D3DXVECTOR3 position(0,0,0);

	RECT fowSR;
	if(playerActor->GetY()<=(SCREEN_HEIGHT/2))
		fowSR.top = 1170+((SCREEN_HEIGHT/2)-(playerActor->GetY()-currentOffsetY));
	else
		fowSR.top = 1170-((playerActor->GetY()-currentOffsetY)-(SCREEN_HEIGHT/2));
	if(playerActor->GetX()<=(SCREEN_WIDTH/2))
		fowSR.left = 990+((SCREEN_WIDTH/2)-(playerActor->GetX()-currentOffsetX));
	else
		fowSR.left = 990-((playerActor->GetX()-currentOffsetX)-(SCREEN_WIDTH/2));
	fowSR.bottom = fowSR.top+SCREEN_HEIGHT;
	fowSR.right = fowSR.left+SCREEN_WIDTH;

	sprite_handler->Draw(
			fow,
			&fowSR,
			NULL,
			&position,
			D3DCOLOR_XRGB(255,255,255)
			);
}

void Game::Game_End(HWND hwnd)
{
	if(sprite_handler!=NULL)
		sprite_handler->Release();
}