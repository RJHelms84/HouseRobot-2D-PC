#include "NavGraph.h"
#include <boost/foreach.hpp>

GraphEdge& NavGraph::GetEdge(int from, int to)
{
	for(std::vector<GraphEdge>::iterator curEdge = edges[from].begin();
		curEdge != edges[from].end();
		++curEdge)
	{
		if(curEdge->GetTo() == to) return *curEdge;
	}

};

int NavGraph::AddNode(GraphNode node)
{
	//MessageBoxA(0,"","",NULL);

	if(node.GetIndex() < (int)nodes.size())
	{
		//if node index is within the current size of the vector
		//this means you are replacing a current node
		nodes[node.GetIndex()] = node;
	}
	else
	{
		//if not replacing current node, add new one
		nodes.push_back(node);

		//also, add new edge list at the same index position
		edges.push_back(std::vector<GraphEdge>());

		//return index for newly created node and increment nextNodeIndex
		return nextNodeIndex++;
	}
}

void NavGraph::RemoveNode(int node)
{
	nodes[node].SetIndex(-1);//set index to invalid, so treated as removed

	//remove any links in other nodes
	for(std::vector<GraphEdge>::iterator curEdge = edges[node].begin();
		curEdge != edges[node].end();
		++curEdge)
	{
		for(std::vector<GraphEdge>::iterator curE = edges[curEdge->GetTo()].begin();
			curE != edges[curEdge->GetTo()].end();
			++curE)
		{
			if(curE->GetTo() == node)
			{
				curE = edges[curEdge->GetTo()].erase(curE);
				break;
			}
		}
	}

	//remove the node's edges
	edges[node].clear();
}

bool NavGraph::FindNode(GraphNode node)
{
	BOOST_FOREACH(GraphNode n, nodes)
	{
		if(n.GetPosition().getX() == node.GetPosition().getX()
			&& n.GetPosition().getY() == node.GetPosition().getY())
			return true;
	}

	return false;
}

void NavGraph::AddEdge(GraphEdge edge)
{
	//check the connecting nodes' index are not invalid (-1)
	if((nodes[edge.GetTo()].GetIndex() != -1) &&
		(nodes[edge.GetFrom()].GetIndex() != -1))
	{
		//add the edge
		edges[edge.GetFrom()].push_back(edge);

		//add an edge coming from the opposite direction
		//so you can travel both ways
		GraphEdge newEdge = edge;

		newEdge.SetTo(edge.GetFrom());
		newEdge.SetFrom(edge.GetTo());

		edges[edge.GetTo()].push_back(newEdge);
	}
}

void NavGraph::RemoveEdge(int from, int to)
{
	std::vector<GraphEdge>::iterator curEdge;

	//remove edge from the lists of both connecting nodes
	for(curEdge = edges[to].begin();
		curEdge != edges[to].end();
		++curEdge)
	{
		if(curEdge->GetTo() == from)
		{
			curEdge = edges[to].erase(curEdge);
			break;
		}
	}

	for(curEdge = edges[from].begin();
		curEdge != edges[from].end();
		++curEdge)
	{
		if(curEdge->GetTo() == to)
		{
			curEdge = edges[from].erase(curEdge);
			break;
		}
	}
}