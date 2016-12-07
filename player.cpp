#include "game.h"
#include "player.h"
#include "robot.h"

PLAYER::PLAYER()
{
	Init(0,0);
}

PLAYER::PLAYER(int x, int y)
{
	Init(x,y);
}

void PLAYER::Init(int x, int y)
{
	playerTextureIdleLeft = LoadTexture("idle_left.png", 0);
	playerTextureIdleRight = LoadTexture("idle_right.png", 0);
	playerTextureIdleDown = LoadTexture("idle_down.png", 0);
	playerTextureIdleUp = LoadTexture("idle_up.png", 0);

	playerTextureWalkingLeft = LoadTexture("walking_left.png", 0);
	playerTextureWalkingRight = LoadTexture("walking_right.png", 0);
	playerTextureWalkingDown = LoadTexture("walking_down.png", 0);
	playerTextureWalkingUp = LoadTexture("walking_up.png", 0);

	currentPlayerTexture = playerTextureIdleRight; //playerTextureIdleDown

	runSoundTexture = LoadTexture("run_sound_display.png", 0);
	runSoundDisplaySwitch=false;
	runSoundDisplayDoOnce=false;
	runSoundDisplayCounter=0;

	waterGlass = Projectile(0,0,0);
	hairDryer = Projectile(0,0,1);

	playerSprite.x=x;
	playerSprite.y=y;
	playerSprite.width=69;
	playerSprite.height=130;
	playerSprite.movex=0;
	playerSprite.movey=0;
	playerSprite.curframe=0;
	playerSprite.lastframe=0;
	playerSprite.animcount=0;
	playerSprite.animdelay=15;

	facingDirection = 1; //3	//0=left,1=right,2=up,3=down
	isIdle = true;

	isRendered = true;

	animMode = false;
	staticAnimMode = false;
	animID = 0;
	isLocked = false;
	isDead = false;
	
	fireDirection = 0;
	fireWater = false;
	fireDryer = false;
	hasWaterGlass = false;
	hasHairDryer = false;

	playerSpeed = 2;
	isRunning=false;
}

PLAYER::~PLAYER()
{

}

bool PLAYER::GetRendered()
{
	return isRendered;
}

void PLAYER::SetRendered(bool choice)
{
	isRendered = choice;
}

void PLAYER::LockPlayerControls()
{
	isLocked = true;
}

void PLAYER::UnlockPlayerControls()
{
	isLocked = false;
}

void PLAYER::MakeMove(int move)
{
	lastPosition.x = playerSprite.x;
	lastPosition.y = playerSprite.y;

		if(move == MOVE_LEFT)
		{
			playerSprite.x -= playerSpeed;
			isIdle = false;
			facingDirection = 0;	//0=left,1=right,2=up,3=down
		}
		//check for right arrow
		else if(move == MOVE_RIGHT)
		{
			playerSprite.x += playerSpeed;
			isIdle = false;
			facingDirection = 1;
		}
		//check for up arrow
		else if(move == MOVE_UP)
		{
			playerSprite.y -= playerSpeed;
			isIdle = false;
			facingDirection = 2;
		}
		//check for down arrow
		else if(move == MOVE_DOWN)
		{
			playerSprite.y += playerSpeed;
			isIdle = false;
			facingDirection = 3;
		}
}

void PLAYER::SetHasCollided()
{
	playerSprite.x = lastPosition.x;
	playerSprite.y = lastPosition.y;
}

void PLAYER::PlayAnim(int id)
{
	animID = id;

	//EXAMPLE:

	/*
	if(animID == 0) //win battle anim
	{
		//setup data for anim
		playerTexture = LoadTexture("ActorPlayerAbsorb.png", 0);
		playerSprite.width=100;
		playerSprite.height=100;
		playerSprite.curframe=0;
		playerSprite.lastframe=7;
		playerSprite.animcount=0;
		playerSprite.animdelay=5;

		//start animation mode
		animMode = true;
	}
	else	//if id is not listed, then animation mode does not start
	{
		//do nothing
	}*/

	if(animID == 0) //shitman sat typing anim
		{
		//setup data for anim
		currentPlayerTexture = LoadTexture("shitman_sat_typing.png", 0);
		playerSprite.width=69;
		playerSprite.height=130;
		playerSprite.curframe=0;
		playerSprite.lastframe=3;
		playerSprite.animcount=0;
		playerSprite.animdelay=20;

		playerSprite.x = 428;
		playerSprite.y = 103;

		//start animation mode
		staticAnimMode = true;
	}
	else	//if id is not listed, then animation mode does not start
	{
		//do nothing
	}
}

void PLAYER::PlayAnimUpdate()
{
	//has animation delay reached threshold?
	if(++playerSprite.animcount > playerSprite.animdelay)
	{
		//reset counter
		playerSprite.animcount = 0;

		//animate the sprite
		if(++playerSprite.curframe > playerSprite.lastframe)
			StopAnim();
	}
}

void PLAYER::PlayStaticAnimUpdate()
{
	//has animation delay reached threshold?
	if(++playerSprite.animcount > playerSprite.animdelay)
	{
		//reset counter
		playerSprite.animcount = 0;

		//animate the sprite
		if(++playerSprite.curframe > playerSprite.lastframe)
			playerSprite.curframe = 0;
	}
}

void PLAYER::StopAnim()
{
	//stop rendering if dead
	if(isDead)
		SetRendered(false);

	animMode = false;
	staticAnimMode = false;

	//set sprite data back to normal
	playerSprite.movex=0;
	playerSprite.movey=0;
	playerSprite.curframe=0;
	playerSprite.lastframe=0;
	playerSprite.animcount=0;
	playerSprite.animdelay=20;

	playerSprite.width=69;
	playerSprite.height=130;
	currentPlayerTexture = playerTextureIdleDown;
}

void PLAYER::ToggleStaticAnimMode()
{
	if(staticAnimMode)
		staticAnimMode = false;
	else
		staticAnimMode = true;
}

void PLAYER::ProjectileUpdates(ROBOT* r1, Level levelObject)
{
	if(fireWater==true)
	{
		waterGlass.MakeMove(fireDirection);

		//check collision
		for(int i=0; i < levelObject.GetNextElementPos(); i++)
		{
			if(levelObject.GetElement(i)->GetHasCollision())
			{
				if(Collision(&waterGlass.GetSprite(), levelObject.GetElement(i)))
				{
					fireWater=false;
					waterGlass.SetRendered(false);
					break;
				}
			}
		}

		if(Collision(&waterGlass.GetSprite(), &r1->GetSprite()))
		{
			fireWater=false;
			waterGlass.SetRendered(false);
			r1->HitByWater();
		}
	}

	if(fireDryer==true)
	{
		hairDryer.MakeMove(fireDirection);

		//check collision
		for(int i=0; i < levelObject.GetNextElementPos(); i++)
		{
			if(levelObject.GetElement(i)->GetHasCollision())
			{
				if(Collision(&hairDryer.GetSprite(), levelObject.GetElement(i)))
				{
					fireDryer=false;
					hairDryer.SetRendered(false);
					break;
				}
			}
		}

		if(Collision(&hairDryer.GetSprite(), &r1->GetSprite()))
		{
			fireDryer=false;
			hairDryer.SetRendered(false);
			r1->HitByDryer();
		}
	}
}

void PLAYER::Update(int currentOffsetX, int currentOffsetY, ROBOT* r1, Level levelObject)
{
	//disable runsounddisplay after brief life
	if(runSoundDisplaySwitch)
	{
		if(runSoundDisplayCounter>60)
		{
			runSoundDisplayCounter=0;
			runSoundDisplaySwitch=false;
		}
		else runSoundDisplayCounter++;
	}

	//update projectiles
	ProjectileUpdates(r1, levelObject);

	if((!animMode) && (!staticAnimMode))
	{
		if(!isIdle)
		{
			//has animation delay reached threshold?
			if(++playerSprite.animcount > playerSprite.animdelay)
			{
				//reset counter
				playerSprite.animcount = 0;

				//animate the sprite
				if(++playerSprite.curframe > playerSprite.lastframe)
					playerSprite.curframe = 0;
			}
		}

		//change number of frames, whether idle or not
		if(isIdle)
			playerSprite.lastframe=0;
		else
			playerSprite.lastframe=3;

		//move the player

		if(!isLocked) //only allow access if controls are not locked
		{
			//check for activation key
			if(Key_Down(DIK_RETURN))
			{
				SetActivating(true);
			}

			//check for projectile fire activation
			if(Key_Down(DIK_1))
			{
				if(fireWater==false && hasWaterGlass==true)
				{
					waterGlass.SetSpriteX(playerSprite.x+(playerSprite.width/2));
					waterGlass.SetSpriteY(playerSprite.y+(playerSprite.height/2));
					waterGlass.SetRendered(true);

					fireWater=true;
					hasWaterGlass=false;
					fireDirection=facingDirection;
				}
			}
			if(Key_Down(DIK_2))
			{
				if(fireDryer==false && hasHairDryer==true)
				{
					hairDryer.SetSpriteX(playerSprite.x+(playerSprite.width/2));
					hairDryer.SetSpriteY(playerSprite.y+(playerSprite.height/2));
					hairDryer.SetRendered(true);

					fireDryer=true;
					hasHairDryer=false;
					fireDirection=facingDirection;
				}
			}

			//check for sprint key
			if(Key_Down(DIK_LSHIFT))
			{
				playerSpeed=4;
				playerSprite.animdelay=7;
				isRunning=true;
				if(!runSoundDisplayDoOnce)
				{
					runSoundDisplaySwitch=true;
					runSoundDisplayDoOnce=true;
					runSoundDisplayCounter=0;
				}
			}
			else
			{
				playerSpeed=2;
				playerSprite.animdelay=15;
				isRunning=false;

				//reset flag so runsounddisplay can display again
				runSoundDisplayDoOnce=false;
			}

			//check for left arrow
			if(Key_Down(DIK_LEFT))
			{
				MakeMove(MOVE_LEFT);
				currentPlayerTexture = playerTextureWalkingLeft;
			}
			//check for right arrow
			else if(Key_Down(DIK_RIGHT))
			{
				MakeMove(MOVE_RIGHT);
				currentPlayerTexture = playerTextureWalkingRight;
			}
			//check for up arrow
			else if(Key_Down(DIK_UP))
			{
				MakeMove(MOVE_UP);
				currentPlayerTexture = playerTextureWalkingUp;
			}
			//check for down arrow
			else if(Key_Down(DIK_DOWN))
			{
				MakeMove(MOVE_DOWN);
				currentPlayerTexture = playerTextureWalkingDown;
			}
			else
			{
				isIdle = true;
				playerSprite.curframe = 0;
				if(facingDirection == 0)
					currentPlayerTexture = playerTextureIdleLeft;
				else if (facingDirection == 1)
					currentPlayerTexture = playerTextureIdleRight;
				else if(facingDirection == 2)
					currentPlayerTexture = playerTextureIdleUp;
				else if(facingDirection == 3)
					currentPlayerTexture = playerTextureIdleDown;
			}
		}
	}
	else if (staticAnimMode)
	{
		PlayStaticAnimUpdate();
	}
	else //if animMode is activated, run this update function instead
	{
		PlayAnimUpdate();
	}

	//check if player has passed screen boundaries, and reposition as necessary
	if(playerSprite.x > (currentOffsetX + SCREEN_WIDTH) - playerSprite.width)
	{
		playerSprite.x = (currentOffsetX + SCREEN_WIDTH) - playerSprite.width;
	}
	else if (playerSprite.x < currentOffsetX)
	{
		playerSprite.x = currentOffsetX;
	}

	if(playerSprite.y > (currentOffsetY + SCREEN_HEIGHT) - playerSprite.height)
	{
		playerSprite.y = (currentOffsetY + SCREEN_HEIGHT) - playerSprite.height;
	}
	else if(playerSprite.y < currentOffsetY)
	{
		playerSprite.y = currentOffsetY;
	}
}

void PLAYER::Draw(LPD3DXSPRITE& sprite_handler, int currentOffsetX, int currentOffsetY)
{
	D3DXVECTOR3 position(0,0,0);

	if(playerSprite.x > currentOffsetX)
		position.x = (float)(playerSprite.x - currentOffsetX);
	else
		position.x = (float)-(currentOffsetX - playerSprite.x);

	if(playerSprite.y > currentOffsetY)
		position.y = (float)(playerSprite.y - currentOffsetY);
	else
		position.y = (float)-(currentOffsetY - playerSprite.y);

	RECT srcRect = this->GetSrcRect();

	sprite_handler->Draw(
			currentPlayerTexture,
			&srcRect,
			NULL,
			&position,
			D3DCOLOR_XRGB(255,255,255)
			);

	if(runSoundDisplaySwitch)
	{
		position.y = position.y + (playerSprite.height-28);

		sprite_handler->Draw(
			runSoundTexture,
			NULL,
			NULL,
			&position,
			D3DCOLOR_XRGB(255,255,255)
			);
	}


	if(fireWater==true)
	{
		waterGlass.Draw(sprite_handler,currentOffsetX,currentOffsetY);
	}
	if(fireDryer==true)
	{
		hairDryer.Draw(sprite_handler,currentOffsetX,currentOffsetY);
	}
}

RECT PLAYER::GetSrcRect()
{
	srcRect.left = playerSprite.curframe * playerSprite.width;
	srcRect.right = srcRect.left + playerSprite.width;
	srcRect.top = 0;
	srcRect.bottom = srcRect.top + playerSprite.height;

	return srcRect;
}

SPRITE PLAYER::GetSprite()
{
	return playerSprite;
}

D3DXVECTOR2 PLAYER::GetPosition()
{
	D3DXVECTOR2 vector(0,0);
	vector.x = (float)GetX();
	vector.y = (float)GetY();

	return vector;
}

bool PLAYER::GetIsAnimMode()
{
	return animMode;
}

void PLAYER::SetSpriteX(int x)
{
	playerSprite.x = x;
}

void PLAYER::SetSpriteY(int y)
{
	playerSprite.y = y;
}

int PLAYER::Collision(SPRITE* sprite1, SPRITE* sprite2)	//collision detection
{
	RECT rect1;
	rect1.left = sprite1->x;
	rect1.top = sprite1->y;
	rect1.right = sprite1->x + sprite1->width;
	rect1.bottom = sprite1->y + sprite1->height;

	RECT rect2;
	rect2.left = sprite2->x;
	rect2.top = sprite2->y;
	rect2.right = sprite2->x + sprite2->width;
	rect2.bottom = sprite2->y + sprite2->height;

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}

int PLAYER::Collision(SPRITE* sprite1, Element* sprite2)	//collision detection
{
	RECT rect1;
	rect1.left = sprite1->x;
	rect1.top = sprite1->y;
	rect1.right = sprite1->x + sprite1->width;
	rect1.bottom = sprite1->y + sprite1->height;

	RECT rect2;
	rect2.left = sprite2->GetX();
	rect2.top = sprite2->GetY();
	rect2.right = sprite2->GetX() + sprite2->GetWidth();
	rect2.bottom = sprite2->GetY() + sprite2->GetHeight();

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}