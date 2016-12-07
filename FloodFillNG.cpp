#include <stack>
#include <boost/foreach.hpp>
#include "FloodFillNG.h"
#include "Level.h"
#include "NavGraph.h"
#include <string>

bool FloodFillNG::Generate_Basic(NavGraph* g)
{
	NavGraph* graph = g;

	graph->AddNode(GraphNode(0,Vector2(64,158))); //0
	graph->AddNode(GraphNode(1,Vector2(66,215)));//1
	graph->AddNode(GraphNode(2,Vector2(258,215)));//2
	graph->AddNode(GraphNode(3,Vector2(258,478)));//3
	graph->AddNode(GraphNode(4,Vector2(74,478)));//4
	graph->AddNode(GraphNode(5,Vector2(393,214)));//5
	graph->AddNode(GraphNode(6,Vector2(603,213)));//6
	graph->AddNode(GraphNode(7,Vector2(393,486)));//7
	graph->AddNode(GraphNode(8,Vector2(607,491)));//8
	graph->AddNode(GraphNode(9,Vector2(608,423)));//9
	graph->AddNode(GraphNode(10,Vector2(782,422)));//10
	graph->AddNode(GraphNode(11,Vector2(784,253)));//11
	graph->AddNode(GraphNode(12,Vector2(987,251)));//12
	graph->AddNode(GraphNode(13,Vector2(988,498)));//13
	graph->AddNode(GraphNode(14,Vector2(1338,498)));//14
	graph->AddNode(GraphNode(15,Vector2(1338,251)));//15
	graph->AddNode(GraphNode(16,Vector2(769,788)));//16
	graph->AddNode(GraphNode(17,Vector2(175,788)));//17
	graph->AddNode(GraphNode(18,Vector2(775,1221)));//18
	graph->AddNode(GraphNode(19,Vector2(577,1221)));//19
	graph->AddNode(GraphNode(20,Vector2(577,1109)));//20
	graph->AddNode(GraphNode(21,Vector2(63,1109)));//21
	graph->AddNode(GraphNode(22,Vector2(63,1408)));//22
	graph->AddNode(GraphNode(23,Vector2(580,1408)));//23
	graph->AddNode(GraphNode(24,Vector2(775,1345)));//24
	graph->AddNode(GraphNode(25,Vector2(993,1345)));//25
	graph->AddNode(GraphNode(26,Vector2(993,1230)));//26
	graph->AddNode(GraphNode(27,Vector2(1069,1230)));//27
	graph->AddNode(GraphNode(28,Vector2(1312,1230)));//28
	graph->AddNode(GraphNode(29,Vector2(1318,1394)));//29
	graph->AddNode(GraphNode(30,Vector2(993,1394)));//30
	graph->AddNode(GraphNode(31,Vector2(1068,916)));//31
	graph->AddNode(GraphNode(32,Vector2(1311,916)));//32
	graph->AddNode(GraphNode(33,Vector2(1311,790)));//33
	graph->AddNode(GraphNode(34,Vector2(990,790)));//34
	graph->AddNode(GraphNode(35,Vector2(990,916)));//35
	//extra nodes added to break up long edges
	graph->AddNode(GraphNode(36,Vector2(254,331)));//36, between 2 and 3
	graph->AddNode(GraphNode(37,Vector2(397,345)));//37, between 5 and 7
	graph->AddNode(GraphNode(38,Vector2(602,315)));//38, between 6 and 9
	graph->AddNode(GraphNode(39,Vector2(1150,252)));//39, between 12 and 15
	graph->AddNode(GraphNode(40,Vector2(1337,362)));//40, between 15 and 14
	graph->AddNode(GraphNode(41,Vector2(1160,500)));//41, between 13 and 14
	graph->AddNode(GraphNode(42,Vector2(983,375)));//42, between 12 and 13
	graph->AddNode(GraphNode(43,Vector2(769,597)));//43, between 10 and 16
	graph->AddNode(GraphNode(44,Vector2(573,790)));//44, between 16 and 17
	graph->AddNode(GraphNode(45,Vector2(381,790)));//45, between 16 and 17
	graph->AddNode(GraphNode(46,Vector2(772,909)));//46, between 16 and 18
	graph->AddNode(GraphNode(47,Vector2(772,1073)));//47, between 16 and 18
	graph->AddNode(GraphNode(48,Vector2(314,1107)));//48, between 20 and 21
	graph->AddNode(GraphNode(49,Vector2(67,1240)));//49, between 21 and 22
	graph->AddNode(GraphNode(50,Vector2(324,1405)));//50, between 22 and 23
	graph->AddNode(GraphNode(51,Vector2(1153,1395)));//51, between 29 and 30
	graph->AddNode(GraphNode(52,Vector2(1145,790)));//52, between 33 and 34
	graph->AddNode(GraphNode(53,Vector2(503,347)));//53
	graph->AddNode(GraphNode(54,Vector2(1164,372)));//54
	graph->AddNode(GraphNode(55,Vector2(330,1250)));//55
	//add edges
	graph->AddEdge(GraphEdge(0,1));
	graph->AddEdge(GraphEdge(1,2));
	graph->AddEdge(GraphEdge(2,36));
	graph->AddEdge(GraphEdge(36,3));
	graph->AddEdge(GraphEdge(3,4));
	graph->AddEdge(GraphEdge(2,5));
	graph->AddEdge(GraphEdge(5,6));
	graph->AddEdge(GraphEdge(5,37));
	graph->AddEdge(GraphEdge(37,7));
	graph->AddEdge(GraphEdge(6,38));
	graph->AddEdge(GraphEdge(38,9));
	graph->AddEdge(GraphEdge(7,8));
	graph->AddEdge(GraphEdge(8,9));
	graph->AddEdge(GraphEdge(9,10));
	graph->AddEdge(GraphEdge(10,11));
	graph->AddEdge(GraphEdge(11,12));
	graph->AddEdge(GraphEdge(12,42));
	graph->AddEdge(GraphEdge(42,13));
	graph->AddEdge(GraphEdge(12,39));
	graph->AddEdge(GraphEdge(39,15));
	graph->AddEdge(GraphEdge(13,41));
	graph->AddEdge(GraphEdge(41,14));
	graph->AddEdge(GraphEdge(14,40));
	graph->AddEdge(GraphEdge(40,15));
	graph->AddEdge(GraphEdge(10,43));
	graph->AddEdge(GraphEdge(43,16));
	graph->AddEdge(GraphEdge(16,44));
	graph->AddEdge(GraphEdge(44,45));
	graph->AddEdge(GraphEdge(45,17));
	graph->AddEdge(GraphEdge(16,46));
	graph->AddEdge(GraphEdge(46,47));
	graph->AddEdge(GraphEdge(47,18));
	graph->AddEdge(GraphEdge(18,19));
	graph->AddEdge(GraphEdge(19,20));
	graph->AddEdge(GraphEdge(19,23));
	graph->AddEdge(GraphEdge(20,48));
	graph->AddEdge(GraphEdge(48,21));
	graph->AddEdge(GraphEdge(21,49));
	graph->AddEdge(GraphEdge(49,22));
	graph->AddEdge(GraphEdge(22,50));
	graph->AddEdge(GraphEdge(50,23));
	graph->AddEdge(GraphEdge(18,24));
	graph->AddEdge(GraphEdge(24,25));
	graph->AddEdge(GraphEdge(25,26));
	graph->AddEdge(GraphEdge(25,30));
	graph->AddEdge(GraphEdge(26,27));
	graph->AddEdge(GraphEdge(27,28));
	graph->AddEdge(GraphEdge(28,29));
	graph->AddEdge(GraphEdge(29,51));
	graph->AddEdge(GraphEdge(51,30));
	graph->AddEdge(GraphEdge(27,31));
	graph->AddEdge(GraphEdge(31,32));
	graph->AddEdge(GraphEdge(32,33));
	graph->AddEdge(GraphEdge(33,52));
	graph->AddEdge(GraphEdge(52,34));
	graph->AddEdge(GraphEdge(34,35));
	graph->AddEdge(GraphEdge(35,31));
	graph->AddEdge(GraphEdge(53,5));
	graph->AddEdge(GraphEdge(53,6));
	graph->AddEdge(GraphEdge(53,38));
	graph->AddEdge(GraphEdge(53,9));
	graph->AddEdge(GraphEdge(53,8));
	graph->AddEdge(GraphEdge(53,7));
	graph->AddEdge(GraphEdge(53,37));
	graph->AddEdge(GraphEdge(54,12));
	graph->AddEdge(GraphEdge(54,39));
	graph->AddEdge(GraphEdge(54,15));
	graph->AddEdge(GraphEdge(54,40));
	graph->AddEdge(GraphEdge(54,14));
	graph->AddEdge(GraphEdge(54,41));
	graph->AddEdge(GraphEdge(54,13));
	graph->AddEdge(GraphEdge(54,42));
	graph->AddEdge(GraphEdge(55,21));
	graph->AddEdge(GraphEdge(55,48));
	graph->AddEdge(GraphEdge(55,20));
	graph->AddEdge(GraphEdge(55,19));
	graph->AddEdge(GraphEdge(55,23));
	graph->AddEdge(GraphEdge(55,50));
	graph->AddEdge(GraphEdge(55,22));
	graph->AddEdge(GraphEdge(55,49));

	return true;
}

bool FloodFillNG::Generate(NavGraph* g, Level* lvl, GraphNode sN)
{
	NavGraph* graph = g;
	Level* level = lvl;
	GraphNode startNode = sN;


	std::stack<GraphNode> nodes;

	nodes.push(startNode);

	while(!nodes.empty())
	{
		//MessageBoxA(0,"startloop","startloop",NULL);

		//reset hasPopped switch
		hasPopped = false;

		GraphNode currentNode = nodes.top();
		nodes.pop();

		//check if in bounds of level
		if(currentNode.GetPosition().getX() > 0 &&
			currentNode.GetPosition().getX() < 1500 &&
			currentNode.GetPosition().getY() > 0 &&
			currentNode.GetPosition().getY() < 1500)
		{
			//MessageBoxA(0,"in bounds"," in bounds",NULL);
			//check node isn't already in graph
			if(!graph->FindNode(currentNode))
			{
				//MessageBoxA(0,"not in graph","not in graph",NULL);

				//check node isn't hitting collision objects
				for(int i=0; i < level->GetNextElementPos(); i++)
				{
					//MessageBoxA(0,"","",NULL);
					if(level->GetElement(i)->GetHasCollision())
					{
						if(NodeCollision(currentNode,*level->GetElement(i)))
						{
							//has collided?
							//MessageBoxA(0,"has collided","has collided",NULL);
							//get rid of node
							hasPopped = true;
							break;
						}

						if(index>0)//don't run on first node
						{
							//collision check the space between nodes, in increments of 5
							for(int i = 0; i < 10; i++)
							{
								if(currentNode.GetPosition().getX() != graph->GetNode((int)currentNode.GetParentIndex()).GetPosition().getX())
								{
									GraphNode tempNode = currentNode;
									Vector2 tempPos;
									if(currentNode.GetPosition().getX() > graph->GetNode((int)currentNode.GetParentIndex()).GetPosition().getX())
										tempPos.setX(currentNode.GetPosition().getX()-(5*(i+1)));
									else
										tempPos.setX(currentNode.GetPosition().getX()+(5*(i+1)));
									tempPos.setY(currentNode.GetPosition().getY());
									tempNode.SetPosition(tempPos);

									if(NodeCollision(tempNode,*level->GetElement(i)))
									{
										hasPopped = true;
										break;
									}
								}
								else
								{
									GraphNode tempNode = currentNode;
									Vector2 tempPos;
									if(currentNode.GetPosition().getY() > graph->GetNode((int)currentNode.GetParentIndex()).GetPosition().getY())
										tempPos.setY(currentNode.GetPosition().getY()-(5*(i+1)));
									else
										tempPos.setY(currentNode.GetPosition().getY()+(5*(i+1)));
									tempPos.setX(currentNode.GetPosition().getX());
									tempNode.SetPosition(tempPos);

									if(NodeCollision(tempNode,*level->GetElement(i)))
									{
										hasPopped = true;
										break;
									}
								}
							}//for loop
						}
					}
				}//for loop
			}
			else
			{
				//MessageBoxA(0,"already in graph","already in graph",NULL);
				hasPopped = true;
			}
		}
		else
		{
			//out of bounds, get rid of node
			//MessageBoxA(0,"out of level bounds","out of level bounds",NULL);
			hasPopped = true;		
		}
		

		if(!hasPopped) //if node has not been popped, keep and add next to stack
		{
			//keep node
			currentNode.SetIndex(index);
			graph->AddNode(currentNode);
			//MessageBoxA(0,"addnode","addnode",NULL);

			//add edges (one for each direction)
			if(index>0)//skip edges for first node
			{
				graph->AddEdge(GraphEdge(currentNode.GetParentIndex(),index));
			}

			//add next nodes to stack

			//up direction
			nodes.push(GraphNode(0,index,Vector2(currentNode.GetPosition().getX(),
				currentNode.GetPosition().getY()-50)));

			//down direction
			nodes.push(GraphNode(0,index,Vector2(currentNode.GetPosition().getX(),
				currentNode.GetPosition().getY()+50)));

			//left direction
			nodes.push(GraphNode(0,index,Vector2(currentNode.GetPosition().getX()-50,
				currentNode.GetPosition().getY())));

			//right direction
			nodes.push(GraphNode(0,index,Vector2(currentNode.GetPosition().getX()+50,
				currentNode.GetPosition().getY())));

			index++;
		}

	}//while loop

	return true;
}

bool FloodFillNG::NodeCollision(GraphNode node, Element element)
{
	RECT rect1 = RECT();
	rect1.left = node.GetPosition().getX();
	rect1.top = node.GetPosition().getY() - 130;
	rect1.right = rect1.left + 69;
	rect1.bottom = node.GetPosition().getY();

	RECT rect2 = RECT();
	rect2.left = element.GetX();
	rect2.top = element.GetY();
	rect2.right = element.GetX() + element.GetWidth();
	rect2.bottom = element.GetY() + element.GetHeight();

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}