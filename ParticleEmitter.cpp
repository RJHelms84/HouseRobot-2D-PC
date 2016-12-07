#include <d3dx9math.h>
#include "dxaudio.h"
#include "dxgraphics.h"
#include "game.h"
#include "ParticleEmitter.h"
#include <boost/foreach.hpp>
#include <boost/timer.hpp>

	//constructor
	ParticleEmitter::ParticleEmitter()
	{
		//initialise vars
		p_image = NULL;				//NULL image texture
		p_max = 100;				//max 100 particles
		p_direction = 0;			//direction (angle)
		p_alphaMin = 254;			//min alpha value
		p_alphaMax = 255;			//max alpha value
		p_minR = 0; p_maxR = 255;	//min / max red val
		p_minG = 0; p_maxG = 255;	//min / max green val
		p_minB = 0; p_maxB = 255;	//min / max blue val
		p_spread = 10;				//spread
		p_velocity = 1.0f;			//velocity
		p_scale = 1.0f;				//scale
		p_gap = 100;					//distance between particles

		startTime = p_timer.elapsed() * 1000;
	}

	//load a texture for the particle image
	bool ParticleEmitter::Load(std::string imageFile)
	{
		
		//if already a texture, release memory
		if(p_image) {
			delete p_image;
			p_image = NULL;
		}

		//load the texture from the image file
		char* temp = &imageFile[0];
		p_image = LoadTexture(temp,0);

		return true;
	}

	//destructor
	ParticleEmitter::~ParticleEmitter()
	{
		//clear the particles STL vector
		p_particles.clear();
	}

	//Add individual particles
	void ParticleEmitter::Add()
	{
		//velocities on x and y
		double vx,vy;
		
		//new sprite; set image and position
		SPRITE p;
		p.image = p_image;
		p.x = p_position_x;
		p.y = p_position_y;
		p.width = 10;
		p.height = 10;
		p.curframe = 0;
		p.lastframe = 0;
		
		//add some randomness to the spread
		double variation = (rand() % p_spread - p_spread / 2) / 100.0f;
		
		//set linear velocity
		double dir = Math::WrapAngleDegs( p_direction-90.0f );
		dir = Math::ToRadians( dir );
		vx = cos( dir ) + variation;
		vy = sin( dir ) + variation;
		p.movex = (int)(vx * p_velocity);
		p.movey = (int)(vy * p_velocity);
		
		//set random color based on ranges
		int r = rand()%(p_maxR-p_minR)+p_minR;
		int g = rand()%(p_maxG-p_minG)+p_minG;
		int b = rand()%(p_maxB-p_minB)+p_minB;
		int a = rand()%(p_alphaMax-p_alphaMin)+p_alphaMin;
		p.r = r;
		p.g = g;
		p.b = b;
		p.a = a;
		
		//set the scale
		p.scalex = (int)p_scale;
		p.scaley = (int)p_scale;
		
		//add particle to the emitter
		p_particles.push_back(p);
	}

	//Render method (with optional rotation)
	void ParticleEmitter::Render(LPD3DXSPRITE sprite_handler)
	{

		//for each sprite in the particles vector
		BOOST_FOREACH( SPRITE sprite, p_particles ) 
		{
			//if rotate set, give random rotation
			//if(rotate) {
			//	sprite->setRotation(rand()%5);
			//}

			//render the sprite
			//set origin, position and scale
			D3DXVECTOR2 center(((sprite.width*sprite.scalex)/2),((sprite.height*sprite.scaley)/2));
			D3DXVECTOR2 trans(sprite.x, sprite.y);
			D3DXVECTOR2 scale((float)sprite.scalex,(float)sprite.scaley);

			//Use the scale, center, rotation and 
			//trans values to get transform matrix
			D3DXMATRIX matTransforms;
			D3DXMatrixTransformation2D(&matTransforms,NULL,0.0,
									&scale,&center,NULL,&trans);

			//set the transform matrix to the sprite
			sprite_handler->SetTransform(&matTransforms);

			//draw animation frame
			int fx = sprite.curframe * sprite.width;
			int fy = sprite.curframe * sprite.height;
			RECT srcRect = {fx,fy, fx + sprite.width, fy + sprite.height};

			/*D3DXVECTOR3 position(0,0,0);
			position.x = sprite.x;
			position.y = sprite.y;*/

			LPDIRECT3DTEXTURE9 tempTex = sprite.image;

			//use the Draw method to render the sprite
			sprite_handler->Draw(tempTex,&srcRect,NULL,NULL,
							D3DCOLOR_XRGB(sprite.r,sprite.g,sprite.b));

			//reset sprite identity
			D3DXMATRIX identity;
			D3DXMatrixIdentity(&identity);
			sprite_handler->SetTransform(&identity);
		}
	}

	//Update method
	void ParticleEmitter::Update()
	{
		static Timer timer;
		
		//do we need to add a new particle?
		if ((int)p_particles.size() < p_max)
		{
			//trivial but necessary slowdown
			//to add new particles
			if (timer.StopWatch(p_gap))
			{
				Add();
			}
		}

		int spriteCount = 0;

		//for each sprite in the particle vector
		BOOST_FOREACH( SPRITE sprite, p_particles )
		{
			//update particle’s position
			//double t = p_timer.elapsed();
			//float deltaTime = (t - startTime) * 0.001f;

			//had to edit the sprites directly, as opposed to using "sprite" inside the boost
			//function, as "sprite" here is only a temp copy, so editing it doesn't edit
			//the sprites directly. Which is probably why they used pointers in 3D engine.
			p_particles[spriteCount].x += (sprite.movex * 0.1f);
			p_particles[spriteCount].y += (sprite.movey * 0.1f);
			
			//is particle beyond the emitter’s range?
			Vector2 p_position;
			p_position.x = p_position_x;
			p_position.y = p_position_y;

			Vector2 spritePosition;
			spritePosition.x = p_particles[spriteCount].x;
			spritePosition.y = p_particles[spriteCount].y;

			double dist = Math::Distance( spritePosition, 
											p_position );
			if ( dist > p_length)
			{			
				//reset particle to the origin
				p_particles[spriteCount].x = (float)(p_position.getX());
				p_particles[spriteCount].y = (float)(p_position.getY());
			}

			spriteCount++;
		}
	}

	//set min / max alpha values
	void ParticleEmitter::setAlphaRange(int min, int max)
	{
		p_alphaMin=min; p_alphaMax=max;
	}

	//update RGB min / max colour range
	void ParticleEmitter::setColorRange(int r1,int g1,int b1,
										int r2,int g2,int b2)
	{
		p_minR = r1; p_maxR = r2;
		p_minG = g1; p_maxG = g2;
		p_minB = b1; p_maxB = b2;
	}