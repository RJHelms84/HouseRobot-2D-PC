#ifndef _GRAPHNODE
#define _GRAPHNODE

#include "Vector.h"

class GraphNode
{
private:
	//every node has an index that is >= 0
	//index is set to -1 for makeshift deletion
	int index;
	int parentIndex;

	Vector2 position; //node's position in 2d space

public:
	GraphNode():index(-1){} //index set to -1 (invalid) if no index is passed to constructor
	GraphNode(int idx, Vector2 pos):index(idx){position=pos;}
	GraphNode(int idx,int parent, Vector2 pos):index(idx),parentIndex(parent){position=pos;}

	~GraphNode(){}

	int GetIndex(){return index;}
	void SetIndex(int idx){index=idx;}

	int GetParentIndex(){return parentIndex;}

	Vector2 GetPosition(){return position;}
	void SetPosition(Vector2 pos){position=pos;}
};

#endif