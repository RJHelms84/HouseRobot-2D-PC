#include <stack>
#include <queue>
#include "GraphSearch.h"
#include <boost/foreach.hpp>

bool GraphSearch::Search()
{
	//create stack of pointers to edges
	std::stack<GraphEdge*> stack;

	//create a dummy edge and put on stack
	GraphEdge Dummy(m_Source,m_Source,0);

	stack.push(&Dummy);

	//while edges on the stack, keep searching
	while(!stack.empty())
	{
		//grab next edge
		GraphEdge* Next = stack.top();

		//remove edge from stack
		stack.pop();

		//record parent of the node this edge points to
		m_Route[Next->GetTo()] = Next->GetFrom();

		//mark if visited
		m_Visited[Next->GetTo()] = visited;

		//if target found, return
		if(Next->GetTo() == m_Target)
		{
			return true;
		}

		std::vector<std::vector<GraphEdge>> edgesTemp = m_Graph->GetEdges();

		//push edges leading from node this edge points to
		//onto the stack, if edge does not point to a visited node
		BOOST_FOREACH(GraphEdge e, edgesTemp[Next->GetTo()])
		{
			if(m_Visited[e.GetTo()] == unvisited)
			{
				GraphEdge* edge = new GraphEdge(e.GetFrom(),e.GetTo());
				stack.push(edge);
			}
		}
	}//while

	//no path to target
	return false;
}

void GraphSearch::SearchBFS()
{
	//create stack of pointers to edges
	std::queue<GraphEdge*> Q;

	//create a dummy edge and put on stack
	GraphEdge Dummy(m_Source,m_Source,0);

	Q.push(&Dummy);

	//mark source node as visited
	m_Visited[m_Source] = visited;

	//while edges on the stack, keep searching
	while(!Q.empty())
	{
		//grab next edge
		GraphEdge* Next = Q.front();

		//remove edge from stack
		Q.pop();

		//record parent of the node this edge points to
		m_Route[Next->GetTo()] = Next->GetFrom();

		//if target found, return
		if(Next->GetTo() == m_Target)
		{
			m_Found = true;
			return;
		}

		std::vector<std::vector<GraphEdge>> edgesTemp = m_Graph->GetEdges();

		//push edges leading from node this edge points to
		//onto the stack, if edge does not point to a visited node
		BOOST_FOREACH(GraphEdge e, edgesTemp[Next->GetTo()])
		{
			if(m_Visited[e.GetTo()] == unvisited)
			{
				GraphEdge* edge = new GraphEdge(e.GetFrom(),e.GetTo());
				Q.push(edge);

				//the node is marked as visited here, BEFORE it is examined, because
				//it ensures a maximum of N edges are ever placed in the queue,
				// rather than E edges.
				m_Visited[e.GetTo()] = visited;
			}
		}
	}//while

	//no path to target
	m_Found = false;
	return;
}

GraphSearch::p_Struct GraphSearch::SearchBFS_2(NavGraph p_graph, int p_source, int p_target, bool p_found)
{

	//---------setup--------

	enum{visited, unvisited, no_parent_assigned};

	std::vector<int> temp_visited;
	for(int i = 0; i < p_graph.GetNumNodes(); i++)
	{
		temp_visited.push_back(unvisited);
	}

	std::vector<int> temp_route;
	for(int i = 0; i < p_graph.GetNumNodes(); i++)
	{
		temp_route.push_back(no_parent_assigned);
	}

	p_Struct pS;

	pS.m_Graph = p_graph;
	pS.m_Visited = temp_visited;
	pS.m_Route = temp_route;
	pS.m_Source = p_source;
	pS.m_Target = p_target;
	pS.m_Found = p_found;



	//-------------method-------

	//create stack of pointers to edges
	std::queue<GraphEdge*> Q;

	//create a dummy edge and put on stack
	GraphEdge Dummy(pS.m_Source,pS.m_Source);

	Q.push(&Dummy);

	//mark source node as visited
	pS.m_Visited[pS.m_Source] = visited;

	//while edges on the stack, keep searching
	while(!Q.empty())
	{
		//grab next edge
		GraphEdge* Next = Q.front();

		//remove edge from stack
		Q.pop();

		//record parent of the node this edge points to
		pS.m_Route[Next->GetTo()] = Next->GetFrom();

		//if target found, return
		if(Next->GetTo() == pS.m_Target)
		{
			pS.m_Found = true;
			return pS;
		}

		std::vector<std::vector<GraphEdge>> edgesTemp = pS.m_Graph.GetEdges();

		//push edges leading from node this edge points to
		//onto the stack, if edge does not point to a visited node
		BOOST_FOREACH(GraphEdge e, edgesTemp[Next->GetTo()])
		{
			if(pS.m_Visited[e.GetTo()] == unvisited)
			{
				GraphEdge* edge = new GraphEdge(e.GetFrom(),e.GetTo());
				Q.push(edge);

				//the node is marked as visited here, BEFORE it is examined, because
				//it ensures a maximum of N edges are ever placed in the queue,
				// rather than E edges.
				pS.m_Visited[e.GetTo()] = visited;
			}
		}
	}//while

	//no path to target
	pS.m_Found = false;

	return pS;
}

bool GraphSearch::GetFound_Thread()
{
	searchStatus = searchThread.wait_for(std::chrono::seconds(-1));

	if(searchStatus==std::future_status::ready)
	{
		searchThreadResult = searchThread.get();

		*m_Graph = searchThreadResult.m_Graph;
		m_Visited = searchThreadResult.m_Visited;
		m_Route = searchThreadResult.m_Route;
		m_Source = searchThreadResult.m_Source;
		m_Target = searchThreadResult.m_Target;
		m_Found = searchThreadResult.m_Found;

		if(m_Found)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else return false;
}

std::vector<int> GraphSearch::GetPathToTarget()
{
	std::vector<int> path;

	//just return an empty path if no path to target found
	//or if no target has been specified
	if(!m_Found || m_Target<0) return path;

	int nd = m_Target;

	path.push_back(nd);

	while(nd != m_Source)
	{
		nd = m_Route[nd];

		path.push_back(nd);
	}

	return path;
}