#ifndef _FLOODFILL
#define _FLOODFILL

//#include "NavGraph.h"
//#include "Level.h"

class Level;
class Element;
class NavGraph;
class GraphNode;

class FloodFillNG
{
private:
	//NavGraph* graph;
	//Level* level;
	bool finished;
	//GraphNode startNode;
	int index;//startNode will be at 0
	bool hasPopped;

public:
	//startNode has to be passed with the constructor
	//that includes parentIndex initialisation
	FloodFillNG():index(0),finished(false),hasPopped(false)
	{
		//level = NULL;
	}

	~FloodFillNG(){}

	//basic
	bool Generate_Basic(NavGraph* g);

	//Fill navgraph
	bool Generate(NavGraph* g, Level* lvl, GraphNode sN);//return true when done

	bool GetFinished(){return finished;}

	//NavGraph GetNavGraph(){return graph;}

	bool NodeCollision(GraphNode node, Element element);
};

#endif