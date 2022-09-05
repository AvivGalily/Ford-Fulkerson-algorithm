#include "AlgorithmManager.h"

// run algorithem function:
void AlgorithmManager::run()
{
	Graph G;
	vector<int> parentArr;
	
	int Numofvertex, Numofedges, s, t;
	cin >> Numofvertex;
	cin >> Numofedges;
	cin >> s;
	cin >> t;
	if (Numofvertex < 0 || s < 0 || t < 0 || Numofedges < 0)
	{
		cout << "invalid input";
		exit(1);
	}

	G.updateNumVer(Numofvertex + 1);
	G.updateSourceVer(s);
	G.updateDstVer(t);
	G.MakeEmptyGraph(Numofvertex + 1);
	InitGraphFromuser(G, Numofedges);
	parentArr.resize(G.getNumVers());

	// ford fulkerson algorithem required by 2 ways
	ford_fulkerson_by_two_ways(G, s, t, parentArr);
}

// ford fulkerson algorithem required by 2 ways:
void AlgorithmManager::ford_fulkerson_by_two_ways(Graph& G, int s, int t, vector<int>& parentArr)
{
	int n = G.getNumVers();
	Graph G_cpy;
	Zeroflow(G);
	G_cpy.MakeEmptyGraph(n);
	CopyGraph(G_cpy, G);

	ford_fulkerson_by_bfs(G, s, t, parentArr);
	ford_fulkerson_by_dijkstra(G_cpy, s, t, parentArr);
}

/* our version to ford fulkerson by bfs Algorithem - 
the BFS Algo checks if ther is a path from ver 's' to ver 't' and return a boolean answer in the G_f Graph*/ 
int AlgorithmManager::ford_fulkerson_by_bfs(Graph& G, int s, int t, vector<int> parentArr)
{
	int n = G.getNumVers();
	Graph G_f;
	G_f.updateNumVer(n);
	G_f.MakeEmptyGraph(n);
	Zeroflow(G);
	CopyGraph(G_f, G);

	while (BFS(G_f, s, t, parentArr)) // checks if ther is a path from ver 's' to ver 't' and return a boolean answer in the G_f Graph
	{
		int path_flow = INT_MAX, C_f = INT_MAX; // capacity "Shiyuri" 

		// update the C_f of the minimum path 
		for (int v = t; v != s; v = parentArr[v])
		{
			int u = parentArr[v];
			int curr_capacity_G_ver = G.getCapacity(u, v), curr_flow_G_ver = G.getFlow(u, v);
			C_f = min(C_f, (curr_capacity_G_ver - curr_flow_G_ver));
		}

		// increase the flow in G toward P path ( minimal path )
		for (int v = t; v != s; v = parentArr[v])
		{
			int u = parentArr[v];
			int curr_flow_G_ver = G.getFlow(u, v) + C_f, curr_capacity_G_ver = G.getCapacity(u, v); // f(u,v) + C_f(P)

			G.updateFlow(u, v, curr_flow_G_ver); // f(u,v) = f(u,v) + C_f(P)
			curr_flow_G_ver *= -1; // -f(u,v)

			if (!G.isEgde(v, u)) // f(v,u) = -f(u,v)
				G.AddEdge(v, u, curr_capacity_G_ver, curr_flow_G_ver);
			else
				G.updateFlow(v, u, curr_flow_G_ver);
		}

		// update the capacity in G_f toward P path ( the minimal path )
		for (int v = t; v != s; v = parentArr[v])
		{
			int u = parentArr[v];
			int curr_capacity_G_ver = G.getCapacity(u, v), curr_flow_G_ver = G.getFlow(u, v); // C(u,v) , f(u,v)

			// C_f(u,v) = C(u,v) - f(u,v)
			if (!G_f.isEgde(u, v))
				G_f.AddEdge(u, v, (curr_capacity_G_ver - curr_flow_G_ver), curr_flow_G_ver);
			else
				G_f.updateFlow(u, v, (curr_capacity_G_ver - curr_flow_G_ver));

			if ((curr_capacity_G_ver - curr_flow_G_ver) == 0)
				G_f.RemoveEdge(u, v);

			curr_capacity_G_ver = G.getCapacity(v, u); // C(v,u)
			curr_flow_G_ver = G.getFlow(v, u); // f(v,u)

			//C_f(v, u) = C(u, v) - f(u, v)
			if (!G_f.isEgde(v, u))
				G_f.AddEdge(v, u, (curr_capacity_G_ver - curr_flow_G_ver), curr_flow_G_ver);
			else
				G_f.updateFlow(v, u, (curr_capacity_G_ver - curr_flow_G_ver));

			if ((curr_capacity_G_ver - curr_flow_G_ver) == 0)
				G_f.RemoveEdge(v, u);
		}
	}

	cout << "BFS Method:" << endl;
	int maxFlow = G.findFlow(s);
	cout << "Max flow = " << maxFlow << endl;
	G_f.printMinCut(s, t);
	return maxFlow;

}

/* our implemention to BFS algorithem 'by the book' -
returns a boolean answer - if there is a way fron ver 's' to ver 't'*/
bool AlgorithmManager::BFS(Graph& G, int s, int t, vector<int>& parent)
{
	int n = G.getNumVers();
	queue<int> Q;
	vector<bool> visited;
	vector<int> distance;
	visited.resize(n);
	distance.resize(n);

	fill(visited.begin(), visited.end(), false);  // fill the visited bool array
	fill(distance.begin(), distance.end(), AlgorithmManager::MINUS_ONE); // fill the visited bool array to UNDEFINED (-1)
	distance[s] = 0;

	Q.push(s);
	visited[s] = true;
	parent[s] = AlgorithmManager::MINUS_ONE; // 's' has no parent

	while (!Q.empty())
	{
		int u = Q.front();
		Q.pop();

		for (auto it : G.adjList[u]) // run on all the Edges (u,v) 
		{
			int curr_to = it.getTo();
			if (!visited[curr_to] && curr_to != s && distance[curr_to] == AlgorithmManager::MINUS_ONE)
			{
				distance[curr_to] = distance[u] + 1;
				parent[curr_to] = u;
				Q.push(curr_to);
				visited[curr_to] = true;
			}
		}
	}

	return visited[t];
}


/* our version to ford fulkerson by dijkstra Algorithem -
the dijkstra Algo checks if there is a path from ver 's' to ver 't' and return a boolean answer in the G_f Graph*/
int AlgorithmManager::ford_fulkerson_by_dijkstra(Graph& G, int s, int t, vector<int>& parentArr)
{
	int n = G.getNumVers();
	Graph G_f;
	Zeroflow(G);
	G_f.updateNumVer(n);
	G_f.MakeEmptyGraph(n);
	CopyGraph(G_f, G);

	while (dijkstra(G_f, s, t, parentArr)) // checks if there is a path from ver 's' to ver 't' and return a boolean answer in the G_f Graph
	{
		int path_flow = INT_MAX, C_f = INT_MAX; // capacity "Shiyuri" 

		// update the C_f of the minimum path 
		for (int v = t; v != s; v = parentArr[v])
		{
			int u = parentArr[v];
			int curr_capacity_G_ver = G.getCapacity(u, v), curr_flow_G_ver = G.getFlow(u, v);
			C_f = min(C_f, (curr_capacity_G_ver - curr_flow_G_ver));
			//path_flow = min(path_flow, G_f.getFlow(u, v));
		}

		// increase the flow in G toward P path ( minimal path )
		for (int v = t; v != s; v = parentArr[v])
		{
			int u = parentArr[v];
			int curr_flow_G_ver = G.getFlow(u, v) + C_f, curr_capacity_G_ver = G.getCapacity(u, v); // f(u,v) + C_f(P)

			G.updateFlow(u, v, curr_flow_G_ver); // f(u,v) = f(u,v) + C_f(P)
			curr_flow_G_ver *= -1; // -f(u,v)

			if (!G.isEgde(v, u)) // f(v,u) = -f(u,v)
				G.AddEdge(v, u, curr_capacity_G_ver, curr_flow_G_ver);
			else
				G.updateFlow(v, u, curr_flow_G_ver);
		}

		// update the capacity in G_f toward P path ( the minimal path )
		for (int v = t; v != s; v = parentArr[v])
		{
			int u = parentArr[v];
			int curr_capacity_G_ver = G.getCapacity(u, v), curr_flow_G_ver = G.getFlow(u, v); // C(u,v) , f(u,v)

			// C_f(u,v) = C(u,v) - f(u,v)
			if (!G_f.isEgde(u, v))
				G_f.AddEdge(u, v, (curr_capacity_G_ver - curr_flow_G_ver), curr_flow_G_ver);
			else
				G_f.updateFlow(u, v, (curr_capacity_G_ver - curr_flow_G_ver));

			if ((curr_capacity_G_ver - curr_flow_G_ver) == 0) // remove the edge if the curr flow is zero
				G_f.RemoveEdge(u, v);

			curr_capacity_G_ver = G.getCapacity(v, u); // C(v,u)
			curr_flow_G_ver = G.getFlow(v, u); // f(v,u)

			//C_f(v, u) = C(u, v) - f(u, v)
			if (!G_f.isEgde(v, u))
				G_f.AddEdge(v, u, (curr_capacity_G_ver - curr_flow_G_ver), curr_flow_G_ver);
			else
				G_f.updateFlow(v, u, (curr_capacity_G_ver - curr_flow_G_ver));

			if ((curr_capacity_G_ver - curr_flow_G_ver) == 0) // remove the edge if the curr flow is zero
				G_f.RemoveEdge(v, u);
		}

	}

	//finds the max flow
	cout << "Greedy Method:" << endl;
	int maxFlow = G.findFlow(s);
	cout << "Max flow = " << maxFlow << endl;
	G_f.printMinCut(s, t);
	return maxFlow;
}


/* dijkstra Algorithem implemention 'by the book' and the returns a boolean
answer - if there is a way fron ver 's' to ver 't'.
the priority queue contains pairs of the distance of the ver + ver (which means weight)*/
bool AlgorithmManager::dijkstra(Graph& G, int s, int t, vector<int>& parent)
{
	int n = G.getNumVers(); // num of Vers
	priority_queue<pair<int, int>> Q;  // priority_queue of: (distance of u, u)
	vector<int> distance; 
	vector<bool> visited;
	distance.resize(n); 
	visited.resize(n); 
	
	fill(visited.begin(), visited.end(), false); // fill the visited bool array
	fill(distance.begin(), distance.end(), AlgorithmManager::MINUS_ONE);  // fill the visited bool array to undefined (-1)

	//init Q
	distance[s] = 0;
	Q.push(make_pair(distance[s], s));
	visited[s] = true;

	while (!Q.empty())
	{
		int u = Q.top().second;
		Q.pop();
		for (auto& it : G.adjList[u]) // run on all the Edges (u,v) 
		{
			int v = it.getTo();

			int curr_weight = G.getCapacity(u, v);
			if (distance[v] <= distance[u] + curr_weight && !visited[v]) // d[v] < d[u] + w(u,v) 
			{
				distance[v] = distance[u] + curr_weight;
				parent[v] = u;
				Q.push(make_pair(distance[v], v));
				visited[v] = true;
			}
		}
	}

	return visited[t]; 
}

// updates the flow in G to zero
void AlgorithmManager::Zeroflow(Graph& G)
{
	int n = G.getNumVers();

	for (int i = 0; i < n; i++)
		for (auto it : G.adjList[i])
			it.changeFlow(0);
}

// copy Graph 
void AlgorithmManager::CopyGraph(Graph& G_f, Graph& G)
{
	int n = G.getNumVers();
	G_f.MakeEmptyGraph(n);
	G_f.updateDstVer(G.getDstVer());
	G_f.updateSourceVer(G.getSourceVer());


	for (int i = 0; i < n; i++)
	{
		for (auto it : G.adjList[i])
		{
			int from = i, to = it.getTo(), cpacity = it.getCapacity(), flow = it.getFlow();
			G_f.AddEdge(from, to, cpacity, flow);
		}
	}
}



void AlgorithmManager::InitGraphFromuser(Graph& G, int Numofedges)
{
	int i, from, to, cpacity;
	int n = G.getNumVers();
	for (i = 0; i < Numofedges; i++)
	{
		cin >> from >> to >> cpacity;
		if (from < 0 || to < 0 || cpacity < 0)
		{
			cout << "invalid input";
			exit(1);
		}

		if (from > n || to > n)
		{
			cout << "invalid input";
			exit(1);
		}

		G.AddEdge(from, to, cpacity, 0);
	}
}