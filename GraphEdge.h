#ifndef _GRAPHEDGE
#define _GRAPHEDGE

class GraphEdge
{
private:
	//an edge is a path connecting two nodes
	int indexFrom;
	int indexTo;

	double cost; //cost of traversing edge, or its distance value

	int direction; //0=left,1=right,2=up,3=down

public:
	/*GraphEdge(int from, int to, double costVar):indexFrom(from),
												indexTo(to),
												cost(costVar){}*/
	GraphEdge(int from, int to):indexFrom(from),
								indexTo(to),
								cost(1.0){}

	//constructor used in GraphSearch::SearchBFS_3() method
	GraphEdge(int from, int to, int dir):indexFrom(from),
								indexTo(to),
								direction(dir),
								cost(1.0){}

	//create an invalid edge
	GraphEdge():cost(1.0),
				indexFrom(-1),
				indexTo(-1){}

	~GraphEdge(){}

	int GetFrom(){return indexFrom;}
	void SetFrom(int idx){indexFrom=idx;}
	int GetTo(){return indexTo;}
	void SetTo(int idx){indexTo=idx;}
	double GetCost(){return cost;}
	void SetCost(double newCost){cost=newCost;}

	int GetDirection(){return direction;}
};

#endif