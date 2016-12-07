#include "game.h"
#include "player.h"
#include "Element.h"

Element::Element()
{
	facingDirection = 0; //0=left,1=right,2=up,3=down
	activationDistance = 0;
	activationState = false;
	vectorPos = 0;

	isRendered = false;
	hasCollision = false;
	shortAnimMode = false;
	animMode = false;
	animID = 0;
	removeOnActivate = false;
	uniqueID=0;

	currentTexture = NULL;

	sprite.x=0;
	sprite.y=0;
	sprite.width=0;
	sprite.height=0;
	sprite.movex=0;
	sprite.movey=0;
	sprite.curframe=0;
	sprite.lastframe=3;
	sprite.animcount=0;
	sprite.animdelay=10;
}

Element::~Element(){}

bool Element::IsPlayerTouching(int playerX, int playerY, int playerWidth, int playerHeight)
{
	if((playerX > (sprite.x+sprite.width + activationDistance))
		|| (playerX+playerWidth < (sprite.x-activationDistance))
		|| (playerY > (sprite.y+sprite.height + activationDistance))
		|| (playerY+playerHeight < (sprite.y-activationDistance)))
		return false;
	else return true;
}

RECT Element::GetSrcRect()
{
	srcRect.left = sprite.curframe * sprite.width;
	srcRect.right = srcRect.left + sprite.width;
	srcRect.top = 0;
	srcRect.bottom = srcRect.top + sprite.height;

	return srcRect;
}

SPRITE Element::GetSprite()
{
	return sprite;
}

D3DXVECTOR2 Element::GetPosition()
{
	D3DXVECTOR2 vector(0,0);
	vector.x = (float)GetX();
	vector.y = (float)GetY();

	return vector;
}

bool Element::GetIsAnimMode()
{
	return animMode;
}

void Element::SetSpriteX(int x)
{
	sprite.x = x;
}

void Element::SetSpriteY(int y)
{
	sprite.y = y;
}

void Element::SetTexture(std::string tex)
{
	currentTexture = LoadTexture((char*)tex.c_str(),0);
}

void Element::StartAnim() //runs looped anim
{
	animMode = true;
}

void Element::PlayAnim() //runs non-looped anim
{
	shortAnimMode = true;
}

void Element::ShortAnimUpdate()
{
	//has animation delay reached threshold?
	if(++sprite.animcount > sprite.animdelay)
	{
		//reset counter
		sprite.animcount = 0;

		//animate the sprite
		if(++sprite.curframe > sprite.lastframe)
		{
			StopAnim();
			sprite.curframe = sprite.lastframe; //set the current frame to the last
		}
	}
}

void Element::AnimUpdate()
{

	//has animation delay reached threshold?
	if(++sprite.animcount > sprite.animdelay)
	{
		//reset counter
		sprite.animcount = 0;

		//animate the sprite
		if(++sprite.curframe > sprite.lastframe)
			sprite.curframe = 0;
	}
}

void Element::StopAnim()
{
	shortAnimMode = false;
	animMode = false;
}

void Element::Update()
{
	if(animMode)
		AnimUpdate();
	else if(shortAnimMode)
		ShortAnimUpdate();
}

void Element::Draw(LPD3DXSPRITE& sprite_handler, int currentOffsetX, int currentOffsetY)
{
	if(isRendered) //only draw if rendered
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
}

void Element::SetEnabled(bool val)
{
	hasCollision = val;
	isRendered = val;
}