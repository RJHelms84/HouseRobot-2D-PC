#include "projectile.h"

Projectile::Projectile()
{
	typeID = 0;
	Init(0,0);
}

Projectile::Projectile(int x, int y, int id)
{
	typeID = id;
	Init(x,y);
}

void Projectile::Init(int x, int y)
{
	if(typeID==0)//water glass
	{
		currentTexture = LoadTexture("waterglass.png", 0);
		sprite.width=16;
		sprite.height=17;
	}
	else if(typeID==1)//hairdryer
	{
		currentTexture = LoadTexture("hairdryer.png", 0);
		sprite.width=30;
		sprite.height=30;
	}

	sprite.x=x;
	sprite.y=y;
	sprite.movex=0;
	sprite.movey=0;
	sprite.curframe=0;
	sprite.lastframe=0;
	sprite.animcount=0;
	sprite.animdelay=10;

	isRendered = true;
}

Projectile::~Projectile(){}

void Projectile::MakeMove(int move)
{
		if(move == 0)
		{
			sprite.x -= 10;
		}
		//check for right arrow
		else if(move == 1)
		{
			sprite.x += 10;
		}
		//check for up arrow
		else if(move == 2)
		{
			sprite.y -= 10;
		}
		//check for down arrow
		else if(move == 3)
		{
			sprite.y += 10;
		}
}

void Projectile::Draw(LPD3DXSPRITE& sprite_handler, int currentOffsetX, int currentOffsetY)
{
	D3DXVECTOR3 position(0,0,0);

	if(sprite.x > currentOffsetX)
		position.x = (float)(sprite.x - currentOffsetX);
	else
		position.x = (float)-(currentOffsetX - sprite.x);

	if(sprite.y > currentOffsetY)
		position.y = (float)(sprite.y - currentOffsetY);
	else
		position.y = (float)-(currentOffsetY - sprite.y);

	RECT srcRect = this->GetSrcRect();

	sprite_handler->Draw(
			currentTexture,
			&srcRect,
			NULL,
			&position,
			D3DCOLOR_XRGB(255,255,255)
			);
}

RECT Projectile::GetSrcRect()
{
	srcRect.left = sprite.curframe * sprite.width;
	srcRect.right = srcRect.left + sprite.width;
	srcRect.top = 0;
	srcRect.bottom = srcRect.top + sprite.height;

	return srcRect;
}