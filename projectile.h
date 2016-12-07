#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include <string>
#include "game.h"

class Projectile
{
private:
	int typeID;
	SPRITE sprite;
	RECT srcRect;
	LPDIRECT3DTEXTURE9 currentTexture;
	bool isRendered;

	void Init(int x, int y);
	RECT GetSrcRect();

public:
	Projectile();
	Projectile(int x, int y, int id);
	~Projectile();

	bool GetRendered(){return isRendered;}
	void SetRendered(bool render){isRendered=render;}

	void MakeMove(int move);

	void Draw(LPD3DXSPRITE&,int,int);

	void SetSpriteX(int newX){sprite.x=newX;}
	void SetSpriteY(int newY){sprite.y=newY;}

	SPRITE GetSprite(){return sprite;}
};

#endif