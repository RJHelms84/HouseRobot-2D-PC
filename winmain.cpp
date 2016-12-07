//Game Framework

#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include <stdio.h>
#include <tchar.h>
#include "dxgraphics.h"
#include "game.h"
#include "dxaudio.h"
#include "dxinput.h"
#include "Database.h"

Database* dbObject = NULL;
Game* gameObject = NULL;

//window event callback function
LRESULT WINAPI WinProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:

		//release the Direct3D device
		if(d3ddev!=NULL)
			d3ddev->Release();

		//release the Direct3D object
		if(d3d!=NULL)
			d3d->Release();

		//call the front-end shutdown function
		gameObject->Game_End(hWnd);

		delete gameObject;
		gameObject = NULL;

		delete dbObject;
		dbObject = NULL;

		//release input objects
		Kill_Keyboard();
		Kill_Mouse();
		if(dinput!=NULL)
			dinput->Release();
		
		//release sound
		//if(dsound!=NULL)
			//dsound->Release();

		//tell Windows to kill this program
		PostQuitMessage(0);
		return 0;

		//code for resetting lost device, commented out for now, as it doesn't work
	/*case WM_ACTIVATEAPP:
		if(wParam)
		{
			ResetDevice(hWnd);
			//ResetSpriteDevice();
			break;
		}*/

	case WM_HR_RESTART:
		//restart game
		delete gameObject;
		gameObject = new Game(hWnd, dbObject);

	case WM_HR_CAUGHTPLAYER:
		dbObject->SetStat(PLAYERDEATHS,dbObject->GetStat(PLAYERDEATHS)+1);
		//restart game
		delete gameObject;
		gameObject = new Game(hWnd, dbObject);
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

//helper function to set up the window properties
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//create window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//fill the struct with infor
	wc.style			=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc		=(WNDPROC)WinProc;
	wc.cbClsExtra		=0;
	wc.cbWndExtra		=0;
	wc.hInstance		=hInstance;
	wc.hIcon			=NULL;
	wc.hCursor			=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground	=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		=NULL;
	wc.lpszClassName	=_T(APPTITLE);
	wc.hIconSm			=NULL;

	//set up the window with the class info
	return RegisterClassEx(&wc);

}

//entry point for a windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;

	//register the class
	MyRegisterClass(hInstance);

	//set up the screen in windowed or fullscreen mode?
	DWORD style;
	if(FULLSCREEN)
		style=WS_EX_TOPMOST|WS_VISIBLE|WS_POPUP;
	else
		style=WS_OVERLAPPED;

	//create a new window
	hWnd= CreateWindowA(
		APPTITLE,			//window class
		APPTITLE,			//title bar
		style,				//window style
		CW_USEDEFAULT,		//xpos of window
		CW_USEDEFAULT,		//ypos of window
		SCREEN_WIDTH,		//width of window
		SCREEN_HEIGHT,		//height of window
		NULL,				//parent window
		NULL,				//menu
		hInstance,			//application instance
		NULL
		);

	//was there an error creating the window?
	if(!hWnd)
		return FALSE;

	//display the window
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	if(!Init_Direct3D(hWnd,SCREEN_WIDTH,SCREEN_HEIGHT,FULLSCREEN))
		return 0;

	//initialise DirectSound
	if(!Init_DirectSound(hWnd))
	{
		MessageBoxA(hWnd, "Error initialising DirectSound","Error",MB_OK);
		return 0;
	}

	//initialise DirectInput
	if(!Init_DirectInput(hWnd))
	{
		MessageBoxA(hWnd, "Error initialising DirectInput","Error",MB_OK);
		return 0;
	}

	//set random number seed
	srand(time(NULL));

	//initialise keyboard
	if(!Init_Keyboard(hWnd))
	{
		MessageBoxA(hWnd, "error initialising keyboard", "error", MB_OK);
	}

	//init the database
	dbObject = new Database();

	//initialise the game
	gameObject = new Game(hWnd, dbObject);

	// main message loop
	int done = 0;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
		//look for quit message
				if (msg.message == WM_QUIT)
					done = 1;
			//decode and pass messages on to WndProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			//process game loop (prevents running after window is closed)
			gameObject->Game_Run(hWnd);
	}
	return msg.wParam;
}
