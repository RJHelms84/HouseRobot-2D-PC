//#include "game.h"
#include "robotstates.h"
#include "robot.h"
#include "player.h"

ROBOT::ROBOT(HWND h)
{
	hWnd = h;

	Init(0,0);
}

ROBOT::ROBOT(int x, int y, PLAYER p1, HWND h, Level* level, NavGraph* graph)
{
	hWnd = h;

	playerXY.x = p1.GetX();
	playerXY.y = p1.GetY();

	Init(x,y);

	levelObj = level;
	robotNavGraph = graph;
}

void ROBOT::Init(int x, int y)
{
	playerTextureIdleLeft = LoadTexture("R_idle_left.png", 0);
	playerTextureIdleRight = LoadTexture("R_idle_right.png", 0);
	playerTextureIdleDown = LoadTexture("R_idle_down.png", 0);
	playerTextureIdleUp = LoadTexture("R_idle_up.png", 0);

	playerTextureWalkingLeft = LoadTexture("R_walking_left.png", 0);
	playerTextureWalkingRight = LoadTexture("R_walking_right.png", 0);
	playerTextureWalkingDown = LoadTexture("R_walking_down.png", 0);
	playerTextureWalkingUp = LoadTexture("R_walking_up.png", 0);

	wetTexture = LoadTexture("R_idle_wet.png",0);
	damagedTexture = LoadTexture("R_idle_damaged.png",0);

	currentPlayerTexture = playerTextureIdleDown; //playerTextureIdleDown


	sprite.x=x;
	sprite.y=y-130;
	sprite.width=69;
	sprite.height=130;
	sprite.movex=0;
	sprite.movey=0;
	sprite.curframe=0;
	sprite.lastframe=0;
	sprite.animcount=0;
	sprite.animdelay=15;

	//for testing genetic algorithm routes
	GA = new GeneticA(0.7,0.001,140,70,2);
	testActor.x=x;
	testActor.y=y-130;
	testActor.width=69;
	testActor.height=130;
	targetX=0;
	targetY=0;
	hasRecievedRoute = false;

	facingDirection = 3;	//0=left,1=right,2=up,3=down
	isIdle = true;

	isRendered = true;

	animMode = false;
	staticAnimMode = false;
	animID = 0;
	isLocked = false;
	isDead = false;
	isWet = false;
	isDamaged = false;

	robotSpeed = 2;

	//init sandbox location nodes
	sbLocation0 = GraphNode(0,Vector2(1318,1392));//29 on graph key (bin/nodes_edges_graph_v2.png)
	sbLocation1 = GraphNode(0,Vector2(1306,790));//33
	sbLocation2 = GraphNode(0,Vector2(176,788));//17
	sbLocation3 = GraphNode(0,Vector2(1338,492));//14
	sbLocation4 = GraphNode(0,Vector2(782,426));//10
	sbLocation5 = GraphNode(0,Vector2(62,1400));//22


	//starting state is IdleState, set to singleton
	//currentState = IdleState::Instance();
	currentState = new SandboxState();
	currentState->Enter(this);

	//create navgraph and populate with nodes and edges

	//add nodes
	/*robotNavGraph->AddNode(GraphNode(0,Vector2(64,158))); //0
	robotNavGraph->AddNode(GraphNode(1,Vector2(66,215)));//1
	robotNavGraph->AddNode(GraphNode(2,Vector2(258,215)));//2
	robotNavGraph->AddNode(GraphNode(3,Vector2(258,478)));//3
	robotNavGraph->AddNode(GraphNode(4,Vector2(74,478)));//4
	robotNavGraph->AddNode(GraphNode(5,Vector2(393,214)));//5
	robotNavGraph->AddNode(GraphNode(6,Vector2(603,213)));//6
	robotNavGraph->AddNode(GraphNode(7,Vector2(393,486)));//7
	robotNavGraph->AddNode(GraphNode(8,Vector2(607,491)));//8
	robotNavGraph->AddNode(GraphNode(9,Vector2(608,423)));//9
	robotNavGraph->AddNode(GraphNode(10,Vector2(782,422)));//10
	robotNavGraph->AddNode(GraphNode(11,Vector2(784,253)));//11
	robotNavGraph->AddNode(GraphNode(12,Vector2(987,251)));//12
	robotNavGraph->AddNode(GraphNode(13,Vector2(988,498)));//13
	robotNavGraph->AddNode(GraphNode(14,Vector2(1338,498)));//14
	robotNavGraph->AddNode(GraphNode(15,Vector2(1338,251)));//15
	robotNavGraph->AddNode(GraphNode(16,Vector2(769,788)));//16
	robotNavGraph->AddNode(GraphNode(17,Vector2(175,788)));//17
	robotNavGraph->AddNode(GraphNode(18,Vector2(775,1221)));//18
	robotNavGraph->AddNode(GraphNode(19,Vector2(577,1221)));//19
	robotNavGraph->AddNode(GraphNode(20,Vector2(577,1109)));//20
	robotNavGraph->AddNode(GraphNode(21,Vector2(63,1109)));//21
	robotNavGraph->AddNode(GraphNode(22,Vector2(63,1408)));//22
	robotNavGraph->AddNode(GraphNode(23,Vector2(580,1408)));//23
	robotNavGraph->AddNode(GraphNode(24,Vector2(775,1345)));//24
	robotNavGraph->AddNode(GraphNode(25,Vector2(993,1345)));//25
	robotNavGraph->AddNode(GraphNode(26,Vector2(993,1230)));//26
	robotNavGraph->AddNode(GraphNode(27,Vector2(1069,1230)));//27
	robotNavGraph->AddNode(GraphNode(28,Vector2(1312,1230)));//28
	robotNavGraph->AddNode(GraphNode(29,Vector2(1318,1394)));//29
	robotNavGraph->AddNode(GraphNode(30,Vector2(993,1394)));//30
	robotNavGraph->AddNode(GraphNode(31,Vector2(1068,916)));//31
	robotNavGraph->AddNode(GraphNode(32,Vector2(1311,916)));//32
	robotNavGraph->AddNode(GraphNode(33,Vector2(1311,790)));//33
	robotNavGraph->AddNode(GraphNode(34,Vector2(990,790)));//34
	robotNavGraph->AddNode(GraphNode(35,Vector2(990,916)));//35
	//extra nodes added to break up long edges
	robotNavGraph->AddNode(GraphNode(36,Vector2(254,331)));//36, between 2 and 3
	robotNavGraph->AddNode(GraphNode(37,Vector2(397,345)));//37, between 5 and 7
	robotNavGraph->AddNode(GraphNode(38,Vector2(602,315)));//38, between 6 and 9
	robotNavGraph->AddNode(GraphNode(39,Vector2(1150,252)));//39, between 12 and 15
	robotNavGraph->AddNode(GraphNode(40,Vector2(1337,362)));//40, between 15 and 14
	robotNavGraph->AddNode(GraphNode(41,Vector2(1160,500)));//41, between 13 and 14
	robotNavGraph->AddNode(GraphNode(42,Vector2(983,375)));//42, between 12 and 13
	robotNavGraph->AddNode(GraphNode(43,Vector2(769,597)));//43, between 10 and 16
	robotNavGraph->AddNode(GraphNode(44,Vector2(573,790)));//44, between 16 and 17
	robotNavGraph->AddNode(GraphNode(45,Vector2(381,790)));//45, between 16 and 17
	robotNavGraph->AddNode(GraphNode(46,Vector2(772,909)));//46, between 16 and 18
	robotNavGraph->AddNode(GraphNode(47,Vector2(772,1073)));//47, between 16 and 18
	robotNavGraph->AddNode(GraphNode(48,Vector2(314,1107)));//48, between 20 and 21
	robotNavGraph->AddNode(GraphNode(49,Vector2(67,1240)));//49, between 21 and 22
	robotNavGraph->AddNode(GraphNode(50,Vector2(324,1405)));//50, between 22 and 23
	robotNavGraph->AddNode(GraphNode(51,Vector2(1153,1395)));//51, between 29 and 30
	robotNavGraph->AddNode(GraphNode(52,Vector2(1145,790)));//52, between 33 and 34
	robotNavGraph->AddNode(GraphNode(53,Vector2(503,347)));//53
	robotNavGraph->AddNode(GraphNode(54,Vector2(1164,372)));//54
	robotNavGraph->AddNode(GraphNode(55,Vector2(330,1250)));//55
	//add edges
	robotNavGraph->AddEdge(GraphEdge(0,1));
	robotNavGraph->AddEdge(GraphEdge(1,2));
	robotNavGraph->AddEdge(GraphEdge(2,36));
	robotNavGraph->AddEdge(GraphEdge(36,3));
	robotNavGraph->AddEdge(GraphEdge(3,4));
	robotNavGraph->AddEdge(GraphEdge(2,5));
	robotNavGraph->AddEdge(GraphEdge(5,6));
	robotNavGraph->AddEdge(GraphEdge(5,37));
	robotNavGraph->AddEdge(GraphEdge(37,7));
	robotNavGraph->AddEdge(GraphEdge(6,38));
	robotNavGraph->AddEdge(GraphEdge(38,9));
	robotNavGraph->AddEdge(GraphEdge(7,8));
	robotNavGraph->AddEdge(GraphEdge(8,9));
	robotNavGraph->AddEdge(GraphEdge(9,10));
	robotNavGraph->AddEdge(GraphEdge(10,11));
	robotNavGraph->AddEdge(GraphEdge(11,12));
	robotNavGraph->AddEdge(GraphEdge(12,42));
	robotNavGraph->AddEdge(GraphEdge(42,13));
	robotNavGraph->AddEdge(GraphEdge(12,39));
	robotNavGraph->AddEdge(GraphEdge(39,15));
	robotNavGraph->AddEdge(GraphEdge(13,41));
	robotNavGraph->AddEdge(GraphEdge(41,14));
	robotNavGraph->AddEdge(GraphEdge(14,40));
	robotNavGraph->AddEdge(GraphEdge(40,15));
	robotNavGraph->AddEdge(GraphEdge(10,43));
	robotNavGraph->AddEdge(GraphEdge(43,16));
	robotNavGraph->AddEdge(GraphEdge(16,44));
	robotNavGraph->AddEdge(GraphEdge(44,45));
	robotNavGraph->AddEdge(GraphEdge(45,17));
	robotNavGraph->AddEdge(GraphEdge(16,46));
	robotNavGraph->AddEdge(GraphEdge(46,47));
	robotNavGraph->AddEdge(GraphEdge(47,18));
	robotNavGraph->AddEdge(GraphEdge(18,19));
	robotNavGraph->AddEdge(GraphEdge(19,20));
	robotNavGraph->AddEdge(GraphEdge(19,23));
	robotNavGraph->AddEdge(GraphEdge(20,48));
	robotNavGraph->AddEdge(GraphEdge(48,21));
	robotNavGraph->AddEdge(GraphEdge(21,49));
	robotNavGraph->AddEdge(GraphEdge(49,22));
	robotNavGraph->AddEdge(GraphEdge(22,50));
	robotNavGraph->AddEdge(GraphEdge(50,23));
	robotNavGraph->AddEdge(GraphEdge(18,24));
	robotNavGraph->AddEdge(GraphEdge(24,25));
	robotNavGraph->AddEdge(GraphEdge(25,26));
	robotNavGraph->AddEdge(GraphEdge(25,30));
	robotNavGraph->AddEdge(GraphEdge(26,27));
	robotNavGraph->AddEdge(GraphEdge(27,28));
	robotNavGraph->AddEdge(GraphEdge(28,29));
	robotNavGraph->AddEdge(GraphEdge(29,51));
	robotNavGraph->AddEdge(GraphEdge(51,30));
	robotNavGraph->AddEdge(GraphEdge(27,31));
	robotNavGraph->AddEdge(GraphEdge(31,32));
	robotNavGraph->AddEdge(GraphEdge(32,33));
	robotNavGraph->AddEdge(GraphEdge(33,52));
	robotNavGraph->AddEdge(GraphEdge(52,34));
	robotNavGraph->AddEdge(GraphEdge(34,35));
	robotNavGraph->AddEdge(GraphEdge(35,31));
	robotNavGraph->AddEdge(GraphEdge(53,5));
	robotNavGraph->AddEdge(GraphEdge(53,6));
	robotNavGraph->AddEdge(GraphEdge(53,38));
	robotNavGraph->AddEdge(GraphEdge(53,9));
	robotNavGraph->AddEdge(GraphEdge(53,8));
	robotNavGraph->AddEdge(GraphEdge(53,7));
	robotNavGraph->AddEdge(GraphEdge(53,37));
	robotNavGraph->AddEdge(GraphEdge(54,12));
	robotNavGraph->AddEdge(GraphEdge(54,39));
	robotNavGraph->AddEdge(GraphEdge(54,15));
	robotNavGraph->AddEdge(GraphEdge(54,40));
	robotNavGraph->AddEdge(GraphEdge(54,14));
	robotNavGraph->AddEdge(GraphEdge(54,41));
	robotNavGraph->AddEdge(GraphEdge(54,13));
	robotNavGraph->AddEdge(GraphEdge(54,42));
	robotNavGraph->AddEdge(GraphEdge(55,21));
	robotNavGraph->AddEdge(GraphEdge(55,48));
	robotNavGraph->AddEdge(GraphEdge(55,20));
	robotNavGraph->AddEdge(GraphEdge(55,19));
	robotNavGraph->AddEdge(GraphEdge(55,23));
	robotNavGraph->AddEdge(GraphEdge(55,50));
	robotNavGraph->AddEdge(GraphEdge(55,22));
	robotNavGraph->AddEdge(GraphEdge(55,49));*/

	//starting location on navgraph
	startGraphIndex = 6;

	pathListSize = 0;
	pathListCounter = 0;
	gSearch = new GraphSearch();
	graphSearchSwitch = false;
	navSwitch = false;
	navSwitchGA = false;
	currentSandboxLocation = 0;
	currentSandboxLocationGA = 0;
	currentSandboxTarget = 0;
	currentSandboxTargetGA = Vector2();
	atSandboxPos = false;
	canSeePlayer = false;

	seekStart=false;
	seekWaitCounter=0;
}

void ROBOT::ChangeState(STATE* newState)
{
	currentState->Exit(this);
	delete currentState;

	currentState = newState;

	currentState->Enter(this);
}

ROBOT::~ROBOT()
{
	delete currentState;
	currentState = NULL;
}

void ROBOT::SetFacingDirection(int direction)//0=left,1=right,2=up,3=down
{
	facingDirection=direction;

	if(direction==0)
		currentPlayerTexture=playerTextureIdleLeft;
	else if(direction==1)
		currentPlayerTexture=playerTextureIdleRight;
	else if(direction==2)
		currentPlayerTexture=playerTextureIdleUp;
	else
		currentPlayerTexture=playerTextureIdleDown;
}

bool ROBOT::GetRendered()
{
	return isRendered;
}

void ROBOT::SetRendered(bool choice)
{
	isRendered = choice;
}

void ROBOT::LockPlayerControls()
{
	isLocked = true;
}

void ROBOT::UnlockPlayerControls()
{
	isLocked = false;
}

int ROBOT::GetClosestNode(int x, int y)
{
	int lastWinner = 0;
	int currentDistance = 0;
	int closestNodeIndex = 0;

	//find closest graph node to robot
	for(int i=0; i<robotNavGraph->GetNumNodes();i++)
	{
		currentDistance = Math::Distance(robotNavGraph->GetNode(i).GetPosition().getX(),
										robotNavGraph->GetNode(i).GetPosition().getY(),
										x,
										y);

		if(lastWinner == 0)
		{
			closestNodeIndex = i;
			lastWinner=currentDistance;
		}
		else if(currentDistance<lastWinner)
		{
			closestNodeIndex = i;
			lastWinner=currentDistance;
		}
	}

	return closestNodeIndex;
}

int ROBOT::GetClosestNode(GraphNode sbLocation)
{
	int lastWinner = 0;
	int currentDistance = 0;
	int closestNodeIndex = 0;

	//find closest graph node to robot
	for(int i=0; i<robotNavGraph->GetNumNodes();i++)
	{
		currentDistance = Math::Distance(robotNavGraph->GetNode(i).GetPosition().getX(),
										robotNavGraph->GetNode(i).GetPosition().getY(),
										sbLocation.GetPosition().getX(),
										sbLocation.GetPosition().getY());

		if(lastWinner == 0)
		{
			closestNodeIndex = i;
			lastWinner=currentDistance;
		}
		else if(currentDistance<lastWinner)
		{
			closestNodeIndex = i;
			lastWinner=currentDistance;
		}
	}

	return closestNodeIndex;
}

void ROBOT::NavTo(int targetIndex)
{
	if(!graphSearchSwitch)//check thread not already running
	{
		int closestNode = GetClosestNode(sprite.x, sprite.y);
		delete gSearch;
		gSearch = new GraphSearch(robotNavGraph,closestNode,targetIndex);
		graphSearchSwitch = true;
		navSwitch=true;
	}
}

void ROBOT::GraphSearchUpdate()
{
	if(graphSearchSwitch)
	{
		//check search is done
		if(gSearch->GetFound_Thread())
		{
			pathToTarget = gSearch->GetPathToTarget();
			pathListSize = pathToTarget.size();
			pathListCounter = pathListSize-1;
			graphSearchSwitch=false;
		}
	}
}

void ROBOT::NavToGA(int x, int y)
{
	StartGeneticSearch(x,y);

	navSwitchGA=true;
}

void ROBOT::StopNav()
{
	pathListCounter = 0;
	pathListSize = 0;
	navSwitch = false;
	navSwitchGA = false;
	hasRecievedRoute=false;
	SetToIdle();
}

void ROBOT::NavToUpdate()
{
	if(navSwitch && !graphSearchSwitch)
	{
		if(pathListCounter >= 0)
		{
			int node = pathToTarget[pathListCounter];
			Vector2 pos = robotNavGraph->GetNode(node).GetPosition();

			MoveTo(pos.x,(pos.y-sprite.height));
		}
		else
		{
			StopNav();
		}
	}
}

void ROBOT::NavToGAUpdate()
{
	if(navSwitchGA)
	{
		if(hasRecievedRoute==true)
		{
			if(pathListCounter >= 0)
			{
				//store last position for collision
				lastPosition.x = sprite.x;
				lastPosition.y = sprite.y;

				switch(pathToTarget[pathListCounter])
				{
				case 0: //North
					sprite.y -= (robotSpeed);
					currentPlayerTexture = playerTextureWalkingUp;
					facingDirection = 2;
					break;

				case 1: //South
					sprite.y += (robotSpeed);
					currentPlayerTexture = playerTextureWalkingDown;
					facingDirection = 3;
					break;

				case 2: //East
					sprite.x += (robotSpeed);
					currentPlayerTexture = playerTextureWalkingRight;
					facingDirection = 1;
					break;

				case 3: //West
					sprite.x -= (robotSpeed);
					currentPlayerTexture = playerTextureWalkingLeft;
					facingDirection = 0;
					break;

				}//end switch

				isIdle=false;
				pathListCounter--;

			}
			else
			{
				StopNav();
			}
		}//end if
	}//end if
}

void ROBOT::MoveTo(int x, int y)
{
	//if within range of 5, no need to move to point
	if(std::abs(sprite.x-x) <= 5)
	{
		if(std::abs(sprite.y-y) <= 5)
		{
			pathListCounter--;
			return;
		}
	}

	//store last position for collision
	lastPosition.x = sprite.x;
	lastPosition.y = sprite.y;

	//move in direction of target coordinates
	if(x > sprite.x + (robotSpeed-1)) //need to + 1 to account for speed of 2
		sprite.x = sprite.x + robotSpeed;
	else if(x < sprite.x - (robotSpeed-1)) //need to - 1 to account for speed of 2
		sprite.x = sprite.x - robotSpeed;
	if(y > sprite.y + (robotSpeed-1))
		sprite.y = sprite.y + robotSpeed;
	else if(y < sprite.y - (robotSpeed-1))
		sprite.y = sprite.y - robotSpeed;

	//set current texture
	if(x > lastPosition.x) //is moving right
	{
		if(y > lastPosition.y) //is moving down
		{
			if((x-lastPosition.x) >= (y-lastPosition.y)) //is moving more right than down
			{
				currentPlayerTexture = playerTextureWalkingRight;
				facingDirection = 1;
			}
			else
			{
				currentPlayerTexture = playerTextureWalkingDown;
				facingDirection = 3;
			}
		}
		else //is moving up
		{
			if((x-lastPosition.x) > (lastPosition.y-y)) //is moving more right than up
			{
				currentPlayerTexture = playerTextureWalkingRight;
				facingDirection = 1;
			}
			else
			{
				currentPlayerTexture = playerTextureWalkingUp;
				facingDirection = 2;
			}
		}
	}
	else //is moving left
	{
		if(y > lastPosition.y) //is moving down
		{
			if((lastPosition.x-x) >= (y-lastPosition.y)) //is moving more left than down
			{
				currentPlayerTexture = playerTextureWalkingLeft;
				facingDirection = 0;
			}
			else
			{
				currentPlayerTexture = playerTextureWalkingDown;
				facingDirection = 3;
			}
		}
		else //is moving up
		{
			if((lastPosition.x-x) > (lastPosition.y-y)) //is moving more left than up
			{
				currentPlayerTexture = playerTextureWalkingLeft;
				facingDirection = 0;
			}
			else
			{
				currentPlayerTexture = playerTextureWalkingUp;
				facingDirection = 2;
			}
		}
	}
	
	isIdle=false;
}

void ROBOT::SeekPlayer()
{
	//only run when the SeekPlayerStart function has finished
	if(!seekStart)
	{
		int closestNode = GetClosestNode(playerXY.getX(),(playerXY.getY()+130));//add player height(130) to get bottom of sprite
		NavTo(closestNode);
	}
}

void ROBOT::FindNextSandboxLocation()
{
	if(currentSandboxLocation==0)
	{
		//check if the player is near the sandbox location
		//(in this case 1318,1392 is the x,y of the sandbox location)
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==GetClosestNode(sbLocation0))
		{
			atSandboxPos = true;
			currentSandboxLocation++;
		}
		else currentSandboxTarget=GetClosestNode(sbLocation0);
	}
	else if(currentSandboxLocation==1)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==GetClosestNode(sbLocation1))
		{
			atSandboxPos = true;
			currentSandboxLocation++;
		}
		else currentSandboxTarget=GetClosestNode(sbLocation1);
	}
	else if(currentSandboxLocation==2)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==GetClosestNode(sbLocation2))
		{
			atSandboxPos = true;
			currentSandboxLocation++;
		}
		else currentSandboxTarget=GetClosestNode(sbLocation2);
	}
	else if(currentSandboxLocation==3)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==GetClosestNode(sbLocation3))
		{
			atSandboxPos = true;
			currentSandboxLocation++;
		}
		else currentSandboxTarget=GetClosestNode(sbLocation3);
	}
	else if(currentSandboxLocation==4)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==GetClosestNode(sbLocation4))
		{
			atSandboxPos = true;
			currentSandboxLocation++;
		}
		else currentSandboxTarget=GetClosestNode(sbLocation4);
	}
	else if(currentSandboxLocation==5)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==GetClosestNode(sbLocation4))
		{
			atSandboxPos = true;
			currentSandboxLocation++;
		}
		else currentSandboxTarget=GetClosestNode(sbLocation5);
	}

	if(currentSandboxLocation>5)
		currentSandboxLocation=0;
}

void ROBOT::FindNextSandboxLocationGA()
{
	if(currentSandboxLocationGA==0)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==29)
		{
			atSandboxPos = true;
			currentSandboxLocationGA++;
		}
		else
		{
			currentSandboxTargetGA.setX(1318);//	node 29
			currentSandboxTargetGA.setY(1394);//
		}
	}
	else if(currentSandboxLocationGA==1)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==33)
		{
			atSandboxPos = true;
			currentSandboxLocationGA++;
		}
		else
		{
			currentSandboxTargetGA.setX(1311);//	node 33
			currentSandboxTargetGA.setY(790);//
		}
	}
	else if(currentSandboxLocationGA==2)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==17)
		{
			atSandboxPos = true;
			currentSandboxLocationGA++;
		}
		else
		{
			currentSandboxTargetGA.setX(175);//	node 17
			currentSandboxTargetGA.setY(788);//
		}
	}
	else if(currentSandboxLocationGA==3)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==14)
		{
			atSandboxPos = true;
			currentSandboxLocationGA++;
		}
		else
		{
			currentSandboxTargetGA.setX(1338);//	node 14
			currentSandboxTargetGA.setY(498);//
		}
	}
	else if(currentSandboxLocationGA==4)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==10)
		{
			atSandboxPos = true;
			currentSandboxLocationGA++;
		}
		else
		{
			currentSandboxTargetGA.setX(782);//	node 10
			currentSandboxTargetGA.setY(422);//
		}
	}
	else if(currentSandboxLocationGA==5)
	{
		if(GetClosestNode(sprite.x,sprite.y+sprite.height)==10)
		{
			atSandboxPos = true;
			currentSandboxLocationGA++;
		}
		else
		{
			currentSandboxTargetGA.setX(63);//	node 22
			currentSandboxTargetGA.setY(1408);//
		}
	}

	if(currentSandboxLocationGA>5)
		currentSandboxLocationGA=0;
}

bool ROBOT::CheckDetectionBox(PLAYER player, Level* levelObject)
{
	if(facingDirection==0)//facing left
	{
		RECT box;
		box.right = sprite.x;
		box.top = sprite.y;
		box.bottom = sprite.y+sprite.height;
		box.left = box.right-400;//box extends 400 to the left

		int closest = 0;

		for(int i=0; i < levelObject->GetNextElementPos(); i++)
		{
			if(levelObject->GetElement(i)->GetHasCollision())
			{
				if(Collision(&box, levelObject->GetElement(i)))
				{
					if(levelObject->GetElement(i)->GetX()>closest)
					{
						closest=levelObject->GetElement(i)->GetX();
					}
				}
			}
		}

		if(Collision(&box,&player.GetSprite()))
		{
			if(player.GetX() > closest)
			{
				//player has been detected
				return true;
			}
			else return false;
		}
	}
	else if(facingDirection==1)//facing right
	{
		RECT box;
		box.left = sprite.x+sprite.width;
		box.right = box.left+400;
		box.top = sprite.y;
		box.bottom = sprite.y+sprite.height;

		int closest = 9999;

		for(int i=0; i < levelObj->GetNextElementPos(); i++)
		{
			if(levelObj->GetElement(i)->GetHasCollision())
			{
				if(Collision(&box, levelObj->GetElement(i)))
				{
					if(levelObj->GetElement(i)->GetX()<closest)
					{
						closest=levelObj->GetElement(i)->GetX();
					}
				}
			}
		}

		if(Collision(&box,&player.GetSprite()))
		{
			if(player.GetX() < closest)
			{
				//player has been detected
				return true;
			}
			else return false;
		}
	}
	else if(facingDirection==2)//facing up
	{
		RECT box;
		box.right = sprite.x+sprite.width+30;
		box.top = sprite.y-400;
		box.bottom = sprite.y;
		box.left = sprite.x-30;

		int closest = 0;

		for(int i=0; i < levelObj->GetNextElementPos(); i++)
		{
			if(levelObj->GetElement(i)->GetHasCollision())
			{
				if(Collision(&box, levelObj->GetElement(i)))
				{
					if(levelObj->GetElement(i)->GetY()>closest)
					{
						closest=levelObj->GetElement(i)->GetY();
					}
				}
			}
		}

		if(Collision(&box,&player.GetSprite()))
		{
			if(player.GetY() > closest)
			{
				//player has been detected
				return true;
			}
			else return false;
		}
	}
	else if(facingDirection==3)//facing down
	{
		RECT box;
		box.right = sprite.x+sprite.width+30;
		box.top = sprite.y+sprite.height;
		box.bottom = box.top+400;
		box.left = sprite.x-30;

		int closest = 9999;

		for(int i=0; i < levelObj->GetNextElementPos(); i++)
		{
			if(levelObj->GetElement(i)->GetHasCollision())
			{
				if(Collision(&box, levelObj->GetElement(i)))
				{
					if(levelObj->GetElement(i)->GetY()<closest)
					{
						closest=levelObj->GetElement(i)->GetY();
					}
				}
			}
		}

		if(Collision(&box,&player.GetSprite()))
		{
			if(player.GetY() < closest)
			{
				//player has been detected
				return true;
			}
			else return false;
		}
	}

	return false;
}

void ROBOT::StartGeneticSearch(int x, int y)
{
	hasRecievedRoute = false;

	targetX = x;
	targetY = y;

	GA->Run();
}

double ROBOT::TestRoute(std::vector<int> &path)
{
	//move testActor to robot's location
	testActor.x = sprite.x;
	testActor.y = sprite.y;

	for (int dir=0; dir<path.size(); ++dir)
	{
		switch(path[dir])
		{
		case 0: //North
			testActor.y -= (robotSpeed*25);
			//check within bounds and that we can move
			if (FullLevelCollisionCheck(testActor))
			{
				testActor.y += (robotSpeed*25);
				break;
			}
			break;

		case 1: //South
			testActor.y += (robotSpeed*25);
			//check within bounds and that we can move
			if (FullLevelCollisionCheck(testActor))
			{
				testActor.y -= (robotSpeed*25);
				break;
			}
			break;

		case 2: //East
			testActor.x += (robotSpeed*25);
			//check within bounds and that we can move
			if (FullLevelCollisionCheck(testActor))
			{
				testActor.x -= (robotSpeed*25);
				break;
			}
			break;

		case 3: //West
			testActor.x -= (robotSpeed*25);
			//check within bounds and that we can move
			if (FullLevelCollisionCheck(testActor))
			{
				testActor.x += (robotSpeed*25);
				break;
			}
			break;

		}//end switch
	}//next direction

	//assign fitness score
	int diffX = abs(testActor.x - targetX);
	int diffY = abs(testActor.y - targetY);

	//arrived at destinated if within distance of robotSpeed
	if ((diffX<=(robotSpeed*25)) && (diffY<=(robotSpeed*25)))
	{
		diffX = 0;
		diffY = 0;
	}

	//we add the one to ensure we never divide by zero. Therefore
	//a solution has been found when this return value = 1
	return 1/(double)(diffX+diffY+1);
}

void ROBOT::SetGeneticRoute(std::vector<int> route)
{
	geneticRoute=route;
	hasRecievedRoute=true;

	pathToTarget = geneticRoute;
	pathListSize = pathToTarget.size();
	pathListCounter = pathListSize-1;
}

void ROBOT::MakeMove(int move)
{

	lastPosition.x = sprite.x;
	lastPosition.y = sprite.y;

		if(move == MOVE_LEFT)
		{
			sprite.x -= 2;
			isIdle = false;
			facingDirection = 0;	//0=left,1=right,2=up,3=down
		}
		//check for right arrow
		else if(move == MOVE_RIGHT)
		{
			sprite.x += 2;
			isIdle = false;
			facingDirection = 1;
		}
		//check for up arrow
		else if(move == MOVE_UP)
		{
			sprite.y -= 2;
			isIdle = false;
			facingDirection = 2;
		}
		//check for down arrow
		else if(move == MOVE_DOWN)
		{
			sprite.y += 2;
			isIdle = false;
			facingDirection = 3;
		}
}

void ROBOT::SetHasCollided()
{
	sprite.x = lastPosition.x;
	sprite.y = lastPosition.y;
}

void ROBOT::SetToIdle()
{
	//set to idle
	isIdle = true;
	sprite.curframe = 0;
	if(facingDirection == 0)
		currentPlayerTexture = playerTextureIdleLeft;
	else if (facingDirection == 1)
		currentPlayerTexture = playerTextureIdleRight;
	else if(facingDirection == 2)
		currentPlayerTexture = playerTextureIdleUp;
	else if(facingDirection == 3)
		currentPlayerTexture = playerTextureIdleDown;
}

void ROBOT::CaughtPlayer(PLAYER* p1)
{
	ChangeState(new IdleState());
	p1->LockPlayerControls();

	PostMessage(hWnd, WM_HR_CAUGHTPLAYER, 0, 0);
}

void ROBOT::HitByWater()
{
	ChangeState(new WetState());
}

void ROBOT::HitByDryer()
{
	if(isWet)
	{
		//enter damaged state
		ChangeState(new DamagedState());
	}
}

void ROBOT::PlayAnim(int id)
{
	animID = id;

	//EXAMPLE:

	/*if(animID == 0) //shitman sat typing anim
		{
		//setup data for anim
		currentChromeTexture = LoadTexture("shitdog_sleeping.png", 0);
		chromeSprite.width=69;
		chromeSprite.height=130;
		chromeSprite.curframe=0;
		chromeSprite.lastframe=3;
		chromeSprite.animcount=0;
		chromeSprite.animdelay=20;

		chromeSprite.x = 505;
		chromeSprite.y = 91;

		//start animation mode
		staticAnimMode = true;
	}
	else	//if id is not listed, then animation mode does not start
	{
		//do nothing
	}*/
}

void ROBOT::PlayAnimUpdate()
{
	//has animation delay reached threshold?
	if(++sprite.animcount > sprite.animdelay)
	{
		//reset counter
		sprite.animcount = 0;

		//animate the sprite
		if(++sprite.curframe > sprite.lastframe)
			StopAnim();
	}
}

void ROBOT::PlayStaticAnimUpdate()
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

void ROBOT::StopAnim()
{
	//stop rendering if dead
	if(isDead)
		SetRendered(false);

	animMode = false;
	staticAnimMode = false;

	//set sprite data back to normal
	sprite.movex=0;
	sprite.movey=0;
	sprite.curframe=0;
	sprite.lastframe=0;
	sprite.animcount=0;
	sprite.animdelay=20;

	sprite.width=69;
	sprite.height=130;
	currentPlayerTexture = playerTextureIdleDown;
	facingDirection = 3;
}

void ROBOT::Update(int& currentOffsetX, int& currentOffsetY, PLAYER& p1)
{
	GraphSearchUpdate();

	//genetic algorithm update method
	if(GA->Started())
	{
		GA->Epoch(this);
	}

	//SeekPlayerStart() delay
	if(seekStart)
	{
		if(seekWaitCounter>=60)
		{
			int closestNode = GetClosestNode(playerXY.getX(),playerXY.getY());
			NavTo(closestNode);
			seekWaitCounter=0;
			seekStart=false;
		}
		else seekWaitCounter++;
	}

	//sound detection
	if(p1.GetIsRunning())
	{
		//only run if in sandbox or search states
		if(currentState->GetStateID()==2 || currentState->GetStateID()==5)
		{
			RECT r;
			r.left=p1.GetX()-200;
			r.right=p1.GetX()+p1.GetWidth()+200;
			r.top=p1.GetY()-200;
			r.bottom=p1.GetY()+p1.GetHeight()+200;

			if(Collision(&r,&sprite))
			{
				//if robot hears player, enter seek state
				ChangeState(new SeekState());
			}
		}
	}

	//update if player can be seen by robot
	if(CheckDetectionBox(p1,levelObj))
	{
		canSeePlayer = true;
	}
	else canSeePlayer = false;


	//update player coords
	playerXY.x = p1.GetX();
	playerXY.y = p1.GetY();

	NavToUpdate();
	NavToGAUpdate();


	if((!animMode) && (!staticAnimMode))
	{
		if(!isIdle)
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

		//change number of frames, whether idle or not
		if(isIdle)
			sprite.lastframe=0;
		else
			sprite.lastframe=3;

		//move the robot
		currentState->Execute(this);

	}
	else if (staticAnimMode)
	{
		PlayStaticAnimUpdate();
	}
	else //if animMode is activated, run this update function instead
	{
		PlayAnimUpdate();
	}
}

void ROBOT::Draw(LPD3DXSPRITE& sprite_handler, int currentOffsetX, int currentOffsetY)
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
			currentPlayerTexture,
			&srcRect,
			NULL,
			&position,
			D3DCOLOR_XRGB(255,255,255)
			);
}

RECT ROBOT::GetSrcRect()
{
	srcRect.left = sprite.curframe * sprite.width;
	srcRect.right = srcRect.left + sprite.width;
	srcRect.top = 0;
	srcRect.bottom = srcRect.top + sprite.height;

	return srcRect;
}

SPRITE ROBOT::GetSprite()
{
	return sprite;
}

D3DXVECTOR2 ROBOT::GetPosition()
{
	D3DXVECTOR2 vector(0,0);
	vector.x = (float)GetX();
	vector.y = (float)GetY();

	return vector;
}

bool ROBOT::GetIsAnimMode()
{
	return animMode;
}

void ROBOT::SetSpriteX(int x)
{
	sprite.x = x;
}

void ROBOT::SetSpriteY(int y)
{
	sprite.y = y;
}

int ROBOT::Collision(RECT* box, Element* sprite2)	//collision detection
{
	RECT rect2;
	rect2.left = sprite2->GetX() + 1;
	rect2.top = sprite2->GetY() + 1;
	rect2.right = sprite2->GetX() + sprite2->GetWidth() - 1;
	rect2.bottom = sprite2->GetY() + sprite2->GetHeight() - 1;

	RECT dest;
	return IntersectRect(&dest, box, &rect2);
}

int ROBOT::Collision(RECT* box, SPRITE* sprite2)	//collision detection
{
	RECT rect2;
	rect2.left = sprite2->x + 1+ 15;
	rect2.top = sprite2->y + 1 + 5;
	rect2.right = sprite2->x + sprite2->width - 1 - 15;
	rect2.bottom = sprite2->y + sprite2->height - 1 - 5;

	RECT dest;
	return IntersectRect(&dest, box, &rect2);
}

int ROBOT::Collision(SPRITE sprite1, Element* sprite2)
{
	RECT rect1;
	rect1.left = sprite1.x+1+15;
	rect1.top = sprite1.y+1;
	rect1.right = sprite1.x + sprite1.width - 1 - 15;
	rect1.bottom = sprite1.y + sprite1.height - 1;

	RECT rect2;
	rect2.left = sprite2->GetX() + 1;
	rect2.top = sprite2->GetY() + 1;
	rect2.right = sprite2->GetX() + sprite2->GetWidth() - 1;
	rect2.bottom = sprite2->GetY() + sprite2->GetHeight() - 1;

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}

bool ROBOT::FullLevelCollisionCheck(SPRITE sprite1)
{
	for(int i=0; i < levelObj->GetNextElementPos(); i++)
	{
		if(levelObj->GetElement(i)->GetHasCollision())
		{
			if(Collision(sprite1, levelObj->GetElement(i)))
			{
				return true;
			}
		}
	}

	return false;
}