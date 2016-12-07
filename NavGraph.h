#ifndef _NAVGRAPH
#define _NAVGRAPH

#include <list>
#include "Vector.h"
#include "GraphNode.h"
#include "GraphEdge.h"

class NavGraph
{
private:
	//the nodes in the navgraph
	std::vector<GraphNode> nodes;

	//the vector of adjacency edge lists
	//each node index keys into the list of edges associated with that node
	std::vector<std::vector<GraphEdge>> edges;

	//the index value for the next node to be added to the navgraph
	int nextNodeIndex;

public:
	NavGraph():nextNodeIndex(0){}
	~NavGraph(){}

	std::vector<std::vector<GraphEdge>> GetEdges(){return edges;}

	int GetNextFreeNodeIndex(){return nextNodeIndex;}

	int GetNumNodes()const{return nodes.size();}

	GraphNode& GetNode(int idx){return nodes[idx];}
	GraphEdge& GetEdge(int from, int to);

	//adds node to graph and returns index
	int AddNode(GraphNode node);

	//remove mode by setting it's index to -1 (invalid)
	void RemoveNode(int node);

	void AddEdge(GraphEdge edge);
	void RemoveEdge(int from, int to);

	bool FindNode(GraphNode node);

	class EdgeIterator
	{
	private:
		std::vector<GraphEdge>::iterator curEdge;
		NavGraph* G;
		const int NodeIndex;

	public:
		EdgeIterator(NavGraph* graph, int node):G(graph),NodeIndex(node)
		{
			curEdge = G->edges[NodeIndex].begin();
		}

		GraphEdge* begin()
		{
			curEdge = G->edges[NodeIndex].begin();
			return &(*curEdge);
		}

		GraphEdge* next()
		{
			++curEdge;
			if(end()) return NULL;
			return &(*curEdge);
		}

		bool end()
		{
			return (curEdge == G->edges[NodeIndex].end());
		}
	};

};

#endif