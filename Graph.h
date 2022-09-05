#pragma once
#include "Edge.h"
#include <queue>
#include <list>
#include <array>
#include <iostream>
#include <vector>
#include <stdlib.h> 

using namespace std;


class Graph
{
private:
    int t = 0, s = 0, n;
public: 
    enum status { UNDEFINED , S, T };
    vector<list<Edge>> adjList;
    Graph() { t = -1, s = -1, n = 0; }
    ~Graph() {};
    void MakeEmptyGraph(int new_n) { adjList.resize(new_n); n = new_n; };
    void updateSourceVer(int new_s) { s = new_s; }
    void updateDstVer(int new_t) { t = new_t; }
    void updateNumVer(int new_n) { n = new_n; }
    int getSourceVer() { return s; }
    int getDstVer() { return t; }
    int getNumVers() { return n; }
    int getFlow(int u, int v);
    int getCapacity(int u, int v);
    void updateFlow(int u, int v, int new_flow);
    void updateCapacity(int u, int v, int new_capacity);
    int findFlow(int s);
    int NumofNeibors(int u);
    list<int>GetAdjList(int u);
    void AddEdge(int u, int v, int c, int flow);
    void RemoveEdge(int u, int v);  
    void printGraph();
    void mergeSort(int* array, int const begin, int const end);
    void merge(int* array, int const left, int const mid, int const right);
    bool isEgde(int u, int v);
    void update_S_Cut(int* statusArray, queue<int>& Q);
    void update_T_Cut(int* statusArray, queue<int>& Q);
    void printMinCut(int s, int t);
};





