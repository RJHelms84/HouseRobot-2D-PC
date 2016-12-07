#include <vector>
#include <boost/foreach.hpp>
#include "player.h"
#include "Element.h"
#include "Level.h"

Level::Level()
{
	nextElementPos = 0; //index position for next element added
	elementContainer = new std::vector<Element*>;
}

Level::~Level()
{
	
}

void Level::Update(PLAYER* playerActor)
{
	BOOST_FOREACH(Element* e, *elementContainer)
	{
		e->Update();
	}

	if(playerActor->GetActivating() == true) //this will equal true when player presses activate
	{
		BOOST_FOREACH(Element* e, *elementContainer)
		{
			if(e->GetFacingDirection() == playerActor->GetFacingDirection())
			{
				if(e->IsPlayerTouching(playerActor->GetX(),playerActor->GetY(),playerActor->GetWidth(),playerActor->GetHeight()))
			    {
			        e->SetActivationState(true);
					if(e->GetRemoveOnActivate())
					{
						e->SetEnabled(false);
						if(e->GetUniqueID()==1)//water glass
						{
							playerActor->SetHasWaterGlass(true);
						}
						else if(e->GetUniqueID()==2)//hairdryer
						{
							playerActor->SetHasHairDryer(true);
						}
					}
			        playerActor->SetActivating(false); //reset player activation
			        break; //break from loop
			    }
			    else e->SetActivationState(false);
			}
			else e->SetActivationState(false);
		}
		playerActor->SetActivating(false);
	}
}

void Level::RenderElements(LPD3DXSPRITE& sprite_handler, int currentOffsetX, int currentOffsetY)
{
	BOOST_FOREACH(Element* e, *elementContainer)
	{
		if(e->GetRendered())
			e->Draw(sprite_handler,currentOffsetX,currentOffsetY);
	}

}

//create new element and add it to the vector
//facingDirection variable should match player's facing direction, not elements
void Level::CreateElement(int x,int y,int width,int height,
				int facingDirection,int activationDistance,bool rendered,bool startAnimated,std::string tex, bool hasCollision, bool removeOnActivate, int uniqueID)
{
	Element* newElement = new Element();
	newElement->SetX(x);
	newElement->SetY(y);
	newElement->SetWidth(width);
	newElement->SetHeight(height);
	newElement->SetFacingDirection(facingDirection);
	newElement->SetActivationDistance(activationDistance);
	newElement->SetVectorPos(nextElementPos);
	newElement->SetTexture(tex);
	newElement->SetHasCollision(hasCollision);
	newElement->SetRemoveOnActivate(removeOnActivate);
	newElement->SetUniqueID(uniqueID);
	if(startAnimated)
		newElement->StartAnim();
	if(rendered)
		newElement->SetRendered(true);
	nextElementPos++;
	elementContainer->push_back(newElement);

	newElement = NULL;
}

//for no textures
void Level::CreateElement(int x,int y,int width,int height,
				int facingDirection,int activationDistance, bool hasCollision)
{
	Element* newElement = new Element();
	newElement->SetX(x);
	newElement->SetY(y);
	newElement->SetWidth(width);
	newElement->SetHeight(height);
	newElement->SetFacingDirection(facingDirection);
	newElement->SetActivationDistance(activationDistance);
	newElement->SetVectorPos(nextElementPos);
	newElement->SetHasCollision(hasCollision);
	newElement->SetRendered(false);
	nextElementPos++;
	elementContainer->push_back(newElement);

	newElement = NULL;
}

void Level::ClearElements()
{
	for(std::vector<Element*>::iterator i = elementContainer->begin(); i != elementContainer->end(); ++i)
	{
		delete *i;
	}
}