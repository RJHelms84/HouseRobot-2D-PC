#ifndef _GRAPHSEARCH
#define _GRAPHSEARCH

//#include <thread>
#include <future>
#include <thread>
#include <chrono>
//#include <boost/thread.hpp>
//#include <boost/thread/future.hpp>
#include "NavGraph.h"

class GraphSearch
{
private:
	enum{visited, unvisited, no_parent_assigned};

	//reference to the graph to be searched
	NavGraph* m_Graph;

	//record of the visited node indices
	std::vector<int> m_Visited;

	//route taken to target node
	std::vector<int> m_Route;

	//source and target node indices
	int m_Source, m_Target;

	//has path to target been found?
	bool m_Found;

	//the search methods
	bool Search();

	struct p_Struct
	{
		//reference to the graph to be searched
		NavGraph m_Graph;

		//record of the visited node indices
		std::vector<int> m_Visited;

		//route taken to target node
		std::vector<int> m_Route;

		//source and target node indices
		int m_Source;
		int m_Target;

		//has path to target been found?
		bool m_Found;
	};

	void SearchBFS();

	//threading
	std::shared_future<p_Struct> searchThread;
	//boost::shared_future<p_Struct> searchThread;

	p_Struct searchThreadResult;
	//std::future_status searchStatus;
	std::_Future_status searchStatus;
	p_Struct SearchBFS_2(NavGraph graph, int source, int target, bool found);

public:
	GraphSearch(){}

	GraphSearch(NavGraph* graph, int source, int target=-1):
		m_Graph(graph),
		m_Source(source),
		m_Target(target),
		m_Found(false),
		m_Visited(m_Graph->GetNumNodes(), unvisited),
		m_Route(m_Graph->GetNumNodes(), no_parent_assigned)
	{
		//create separate thread for algorithm
		searchThread = std::async(&GraphSearch::SearchBFS_2, this, *graph, source, target, false);

		//Depth First Search
		//m_Found = Search();

		//Breadth First Search
		//m_Found = SearchBFS();
	}

	//returns true if target node found
	bool GetFound(){return m_Found;}
	bool GetFound_Thread();

	//returns vector of node indices for the shortest path
	//from source to target node
	std::vector<int> GetPathToTarget();
};

#endif