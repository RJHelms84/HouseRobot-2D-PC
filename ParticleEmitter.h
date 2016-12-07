#ifndef _PARTICLEEMITTER_H
#define _PARTICLEEMITTER_H

#include <string>
#include <vector>
#include "dxaudio.h"
#include "dxgraphics.h"
#include "game.h"
#include "Math.h"
#include "timer.h"

class ParticleEmitter
{
		private:
			//STL vector for particles
			std::vector<SPRITE> p_particles;

			//D3DXMATRIX matTransforms; //placed in sprite stuct instead, so there's multiple

			boost::timer p_timer;
			double startTime;

			//texture pointer
			LPDIRECT3DTEXTURE9 p_image;

			//position vars
			int p_position_x;
			int p_position_y;
			double p_direction;
			double p_length;

			//colour / amount / alpha variation
			int p_max;
			int p_alphaMin, p_alphaMax;
			int p_minR, p_minG, p_minB, p_maxR, p_maxG, p_maxB;

			//spread vars
			int p_spread;
			double p_velocity;
			double p_scale;

			//distance between particles
			int p_gap;

		public:
			//constructor / destructor
			ParticleEmitter();
			virtual ~ParticleEmitter();

			//load particle image
			bool Load(std::string imageFile);

			//render / update
			void Render(LPD3DXSPRITE sprite_handler); 
			void Update();
			
			//dynamically add particles
			void Add();

			//getters and setters
			
			//position
			void setPosition(double x, double y) { p_position_x = x; p_position_y = y; }
			int getPositionX() { return p_position_x; }
			int getPositionY() { return p_position_y; }
			
			//direction
			void setDirection(double angle) { p_direction = angle; }
			double getDirection() { return p_direction; }
			
			//particle numbers
			int getCount() { return (int)p_particles.size(); }
			void setMax(int num) { p_max = num; }
			
			//alpha / colour / spread / gap
			void setAlphaRange(int min,int max);
			void setColorRange(int r1,int g1,int b1,int r2,int g2,int b2);
			void setSpread(int value) { p_spread = value; }
			void setLength(double value) { p_length = value; }
			void setVelocity(double value) { p_velocity = value; }
			void setScale(double value) { p_scale = value; }
			void setGap(int val) { p_gap = val; }
	};

#endif