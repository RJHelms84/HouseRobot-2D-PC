#ifndef _ELEMENT_H
#define _ELEMENT_H

#include "game.h"
#include "player.h"

class Element
{
private:
	unsigned char facingDirection; //0=left,1=right,2=up,3=down
	unsigned short int activationDistance; //distance for this element to be activated
	bool activationState; //set to true when player has activated it
	int vectorPos;

	SPRITE sprite;
	RECT srcRect;
	LPDIRECT3DTEXTURE9 currentTexture;
	bool isRendered;
	bool hasCollision;
	bool shortAnimMode;
	bool animMode;
	int animID;
	bool removeOnActivate;
	int uniqueID; //0 means no unique id, so id's start at 1

public:
	Element();
	~Element();

	void SetX(int s) {sprite.x = s;}
	void SetY(int s) {sprite.y = s;}
	void SetWidth(int w) {sprite.width = w;}
	void SetHeight(int h) {sprite.height = h;}

	int GetVectorPos() {return vectorPos;}
	void SetVectorPos(int p) {vectorPos = p;}

	int GetFacingDirection() {return facingDirection;}
	void SetFacingDirection(int d) {facingDirection = d;}

	int GetActivationDistance() {return activationDistance;}
	void SetActivationDistance(int d) {activationDistance = d;}

	bool GetActivationState() {return activationState;}
	void SetActivationState(bool s){activationState = s;}

	//check if player is in activation distance
	bool IsPlayerTouching(int playerX, int playerY, int playerWidth, int playerHeight);

	bool GetRendered() {return isRendered;}
	void SetRendered(bool isR) {isRendered = isR;}

	bool GetHasCollision() {return hasCollision;}
	void SetHasCollision(bool value) {hasCollision=value;}

	bool GetRemoveOnActivate(){return removeOnActivate;}
	void SetRemoveOnActivate(bool newVal){removeOnActivate=newVal;}

	void StartAnim(); //runs looped anim
	void PlayAnim(); //runs non-looped anim
	void ShortAnimUpdate();
	void AnimUpdate();
	void StopAnim();

	void Update();
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

	void SetTexture(std::string);

	void SetEnabled(bool val);
	int GetUniqueID(){return uniqueID;}
	void SetUniqueID(int id){uniqueID=id;}
};

#endif