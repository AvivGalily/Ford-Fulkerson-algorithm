#pragma once
#include "Graph.h"
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <array>


using namespace std;

#define INT_MAX 2147483647 // max num in the computer 


struct AlgorithmManager
{
	enum { MINUS_ONE = -1 };
	bool BFS(Graph& G, int s, int t, vector<int>& parent);
	int ford_fulkerson_by_bfs(Graph& G, int s, int t, vector<int> parentArr);
	int ford_fulkerson_by_dijkstra(Graph& G, int s, int t, vector<int>& parentArr);
	void Zeroflow(Graph& G);
	void CopyGraph(Graph& G_f, Graph& G);
	void InitGraphFromuser(Graph& G, int Numofedges);
	bool dijkstra(Graph& G, int s, int t, vector<int>& parent);
	void ford_fulkerson_by_two_ways(Graph& G, int s, int t, vector<int>& parentArr);
	void run();
};





