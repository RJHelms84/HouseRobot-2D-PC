#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>
#include "game.h"
#include "timer.h"
#include "projectile.h"
#include "Level.h"

class ROBOT;

class PLAYER
{
private:
	SPRITE playerSprite;
	SPRITE lastPosition;
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

	//running sound texture
	LPDIRECT3DTEXTURE9 runSoundTexture;
	bool runSoundDisplaySwitch;
	bool runSoundDisplayDoOnce;
	int runSoundDisplayCounter;

	Projectile waterGlass;
	Projectile hairDryer;
	int fireDirection;

	bool isIdle;
	int facingDirection;	//0=left,1=right,2=up,3=down
	bool isActivating; //is player activating an element?

	bool isRendered;
	bool animMode;
	bool staticAnimMode;
	int animID;
	bool isLocked; //is player's controls locked?
	bool isDead;

	bool fireWater;
	bool hasWaterGlass;
	bool fireDryer;
	bool hasHairDryer;

	int playerSpeed;
	int isRunning;

public:
	PLAYER();
	PLAYER(int, int);
	~PLAYER();

	void Init(int x, int y);

	int GetFacingDirection() {return facingDirection;}
	bool GetActivating() {return isActivating;}
	void SetActivating(bool state) {isActivating = state;}

	bool GetHasWaterGlass(){return hasWaterGlass;}
	void SetHasWaterGlass(bool val){hasWaterGlass=val;}
	bool GetHasHairDryer(){return hasHairDryer;}
	void SetHasHairDryer(bool val){hasHairDryer=val;}

	bool GetRendered();
	void SetRendered(bool);

	void LockPlayerControls();
	void UnlockPlayerControls();

	void MakeMove(int);
	void SetHasCollided();

	bool GetIsRunning(){return isRunning;}

	void PlayAnim(int);
	void PlayAnimUpdate();
	void PlayStaticAnimUpdate(); //anim that runs until stopped via stopanim function
	void StopAnim();
	void ToggleStaticAnimMode();

	void ProjectileUpdates(ROBOT* r1, Level levelObject);

	void Update(int currentOffsetX, int currentOffsetY, ROBOT* r1, Level levelObject);
	void Draw(LPD3DXSPRITE&,int,int);

	RECT GetSrcRect();
	SPRITE GetSprite();
	D3DXVECTOR2 GetPosition();
	int GetX() {return playerSprite.x;}
	int GetY() {return playerSprite.y;}
	int GetWidth() {return playerSprite.width;}
	int GetHeight() {return playerSprite.height;}
	bool GetIsAnimMode();

	void SetSpriteX(int);
	void SetSpriteY(int);

	int Collision(SPRITE*, SPRITE*);
	int Collision(SPRITE*, Element*);
};

#endif