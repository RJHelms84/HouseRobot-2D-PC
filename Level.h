#ifndef _LEVEL_H
#define _LEVEL_H

#include <vector>
#include "player.h"
#include "Element.h"

class Level
{
private:
	std::vector<Element*>* elementContainer;
	int nextElementPos; //index position for next element added

public:
	Level();
	~Level();

	void Update(PLAYER*);
	void RenderElements(LPD3DXSPRITE& sprite_handler, int currentOffsetX, int currentOffsetY);

	Element* GetElement(int index) {return (*elementContainer)[index];}
	int GetNextElementPos() {return nextElementPos;}

	//facingDirection variable should match player's facing direction, not elements
	void CreateElement(int x,int y,int width,int height,
				int facingDirection,int activationDistance,bool rendered,bool startAnimated,std::string tex, bool hasCollision, bool removeOnActivate, int uniqueID);
	//for no textures
	void CreateElement(int x,int y,int width,int height,
				int facingDirection,int activationDistance, bool hasCollision);

	void ClearElements();

};

#endif