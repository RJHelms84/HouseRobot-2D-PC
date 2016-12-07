#ifndef _ROBOT_H
#define _ROBOT_H

#include <string>
#include "game.h"
#include "timer.h"
#include "Math.h"
#include "GraphSearch.h"
#include "Level.h"
#include "GeneticA.h"
#include "FloodFillNG.h"

class STATE;
class PLAYER;

class ROBOT
{
private:

	HWND hWnd;

	STATE* currentState;

	NavGraph* robotNavGraph;
	FloodFillNG* filler;

	GraphSearch* gSearch;

	Vector2 playerXY;

	//NavGraph robotNavGraph;
	int startGraphIndex;
	std::vector<int> pathToTarget;
	int pathListSize;
	int pathListCounter;
	bool graphSearchSwitch;
	bool navSwitch; //switch on to begin moving to new nav node
	int currentSandboxLocation;
	int currentSandboxTarget;
	bool atSandboxPos;
	bool canSeePlayer;

	//sandbox locations
	GraphNode sbLocation0;
	GraphNode sbLocation1;
	GraphNode sbLocation2;
	GraphNode sbLocation3;
	GraphNode sbLocation4;
	GraphNode sbLocation5;


	SPRITE sprite;
	SPRITE lastPosition;
	SPRITE testActor;
	RECT srcRect;
	LPDIRECT3DTEXTURE9 currentPlayerTexture;
	LPDIRECT3DTEXTURE9 playerTextureIdleLeft;
	LPDIRECT3DTEXTURE9 playerTextureIdleRight;
	LPDIRECT3DTEXTURE9 playerTextureIdleDown;
	LPDIRECT3DTEXTURE9 playerTextureIdleUp;

	LPDIRECT3DTEXTURE9 playerTextureWalkingLeft;
	LPDIRECT3DTEXTURE9 playerTextureWalkingRight;
	LPDIRECT3DTEXTURE9 playerTextureWalkingDown;
	LPDIRECT3DTEXTURE9 playerTextureWalkingUp;

	LPDIRECT3DTEXTURE9 wetTexture;
	LPDIRECT3DTEXTURE9 damagedTexture;

	bool isIdle;
	int facingDirection;	//0=left,1=right,2=up,3=down
	bool isActivating; //is player activating an element?

	bool isRendered;
	bool animMode;
	bool staticAnimMode;
	int animID;
	bool isLocked; //is robot's controls locked?
	bool isDead;
	bool isWet;
	bool isDamaged;

	int robotSpeed;

	bool seekStart;
	int seekWaitCounter;

	//for genetic algorithm
	GeneticA* GA;
	int targetX;
	int targetY;
	//Level* levelObj;
	std::vector<int> geneticRoute;
	bool hasRecievedRoute;
	bool navSwitchGA;
	int currentSandboxLocationGA;
	Vector2 currentSandboxTargetGA;

public:
	Level* levelObj;

	ROBOT(HWND h);
	ROBOT(int,int, PLAYER p1, HWND h, Level* level, NavGraph* graph);
	~ROBOT();

	void Init(int x, int y);

	void ChangeState(STATE* newState);

	int GetFacingDirection() {return facingDirection;}//0=left,1=right,2=up,3=down
	void SetFacingDirection(int direction);//0=left,1=right,2=up,3=down
	bool GetActivating() {return isActivating;}
	void SetActivating(bool state) {isActivating = state;}

	bool GetRendered();
	void SetRendered(bool);

	void LockPlayerControls();
	void UnlockPlayerControls();

	int GetRobotSpeed(){return robotSpeed;}
	void SetRobotSpeed(int newSpeed, int animDelay){robotSpeed=newSpeed;sprite.animdelay=animDelay;}

	//navgraph functions
	int GetClosestNode(int x, int y);
	int GetClosestNode(GraphNode sbLocation);
	void NavTo(int targetIndex); //navigate to node on navgraph
	void GraphSearchUpdate();
	void StopNav();
	void NavToUpdate();
	void MoveTo(int x, int y);
	bool GetNavSwitch(){return navSwitch;}
	int GetCurrentSandboxTarget(){return currentSandboxTarget;}
	bool GetAtSandboxPos(){return atSandboxPos;}
	void SetAtSandboxPos(bool val){atSandboxPos=val;}

	//ai behaviour functions
	void SeekPlayer();
	void SeekPlayerStart(){seekStart=true;}//has a delayed start
	bool GetSeekPlayerStart(){return seekStart;}
	void FindNextSandboxLocation();
	int GetCurrentSandboxLocation(){return currentSandboxLocation;}
	bool CheckDetectionBox(PLAYER player, Level* levelObject);
	bool GetCanSeePlayer(){return canSeePlayer;}

	//genetic algorithm functions
	void StartGeneticSearch(int x, int y);
	double TestRoute(std::vector<int> &path);
	void SetGeneticRoute(std::vector<int> route);
	void NavToGA(int x, int y);
	void NavToGAUpdate();
	Vector2 GetCurrentSandboxTargetGA(){return currentSandboxTargetGA;}
	void FindNextSandboxLocationGA();

	//hit by projectiles
	void HitByWater();
	void HitByDryer();

	void MakeMove(int);
	void SetHasCollided();
	void SetToIdle();
	void CaughtPlayer(PLAYER* p1);

	void SetToWetTexture(){currentPlayerTexture=wetTexture;}
	void SetToDamagedTexture(){currentPlayerTexture=damagedTexture;}
	bool GetIsWet(){return isWet;}
	void SetIsWet(bool val){isWet=val;}
	bool GetIsDamaged(){return isDamaged;}
	void SetIsDamaged(bool val){isDamaged=val;}

	void PlayAnim(int);
	void PlayAnimUpdate();
	void PlayStaticAnimUpdate(); //anim that runs until stopped via stopanim function
	void StopAnim();

	void Update(int& currentOffsetX, int& currentOffsetY, PLAYER& p1);
	void Draw(LPD3DXSPRITE&,int,int);

	RECT GetSrcRect();
	SPRITE GetSprite();
	D3DXVECTOR2 GetPosition();
	int GetX() {return sprite.x;}
	int GetY() {return sprite.y;}
	int GetWidth() {return sprite.width;}
	int GetHeight() {return sprite.height;}
	bool GetIsAnimMode();

	void SetSpriteX(int);
	void SetSpriteY(int);

	int Collision(RECT* box, Element* sprite2);
	int Collision(RECT* box, SPRITE* sprite2);
	int Collision(SPRITE sprite1, Element* sprite2);
	bool FullLevelCollisionCheck(SPRITE sprite1);
};

#endif