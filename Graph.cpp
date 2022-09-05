#include "Graph.h"

// gets the Adj list that required
list<int> Graph::GetAdjList(int u) 
{
    list<int> sortlist;
    int size = NumofNeibors(u);
    int* arr = new int[size];
    int i = 0;

    for (auto it : adjList[u]) 
    {
        arr[i] = it.getTo();
        i++;
    }

    mergeSort(arr, 0, size-1 );

    for (int i = 0; i < size; i++) 
        sortlist.push_back(arr[i]);
        
    return sortlist;
}

// returns the number of neighbors
int Graph::NumofNeibors(int u)
{
    int count = 0;
    for (auto it : adjList[u])
        count++;

    return  count;
}

// prints the graph for tests
void Graph::printGraph() 
{
    for (int i = 0; i < n; i++) 
    {

        cout << i << "-->";
        for (auto it : adjList[i]) {
            cout << it.getTo() << " ";
            cout<< '(' << it.getCapacity() <<')' << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Add edge to the graph
void Graph::AddEdge(int u, int v, int c,int flow) 
{
   Edge edge;
   edge.changeTo(v); 
   edge.changeCapacity(c);
   edge.changeFlow(flow);
   adjList[u].push_back(edge);
};

// merge Algorithem
void Graph::merge(int* array, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;


    auto* leftArray = new int[subArrayOne], * rightArray = new int[subArrayTwo];


    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) 
    {

        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) 
        {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else 
        {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }

        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne)
    {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) 
    {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }

    delete[] leftArray;
    delete[] rightArray;
}

// merge sort Algorithem
void Graph::mergeSort(int* array, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

// remove Edge from the Graph
void Graph::RemoveEdge(int u, int v) {
    list<Edge>::iterator it1,it2;
    it1 = adjList[u].begin();
       
    while (it1 != adjList[u].end())
    {
        if ((*it1).getTo() == v)
        {      
           adjList[u].erase( it1);     
           break;
        }
        it1++;
    }
  
};

// updates the flow on edge (u,v)
void Graph::updateFlow(int u, int v, int new_flow)
{
    for (auto &it : adjList[u])
    {
        if (it.getTo() == v)
            return it.changeFlow(new_flow);
    }
}

// updates the capacity on edge (u,v)
void Graph::updateCapacity(int u, int v, int new_capacity)
{
    for (auto &it : adjList[u])
    {
        if (it.getTo() == v)
            return it.changeCapacity(new_capacity);
    }
}

// gets the flow from the edge (u,v)
int Graph::getFlow(int u, int v)
{
    for (auto &it : adjList[u])
    {
        if (it.getTo() == v)
            return it.getFlow();
    }

    return 0;
}

//gets the capacity from edge (u,v)
int Graph::getCapacity(int u, int v)
{
    for (auto &it : adjList[u])
    {
        if (it.getTo() == v)
            return it.getCapacity();
    }

    return 0;
}

//finds the flow
int Graph::findFlow(int s)
{
    int res = 0;

    for (auto &itr : adjList[s])
    {
        res += itr.getFlow();
    }

    return res;
}

// checks if there is a edge (u,v) on this graph
bool Graph::isEgde(int u, int v)
{
    for (auto &it : adjList[u])
    {
        if (it.getTo() == v)
            return true;
    }

    return false;
}

//updates the S cut required by BFS varietoin Algorithem
void Graph::update_S_Cut(int* statusArray, queue<int>& Q)
{
    int count = 1;
    cout << "Min cut: S = ";
    while (!Q.empty())
    {
        int v = Q.front();
        Q.pop();
        for (auto& it : adjList[v])
        {
            int tmp_v = it.getTo();
            if (statusArray[tmp_v] == UNDEFINED)
            {
                Q.push(tmp_v);
                statusArray[tmp_v] = S;
                count++;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (statusArray[i] == S && count > 1)
        {
            cout << i << ", ";
            count--;
        }
        else if (statusArray[i] == S)
            cout << i << ". ";
    }
}

//updates the T cut required by BFS varietoin Algorithem
void Graph::update_T_Cut(int* statusArray, queue<int>& Q)
{
    int count = 1;
    cout << "T = ";
    while (!Q.empty())
    {
        int v = Q.front();
        Q.pop();
        for (auto& it : adjList[v])
        {
            int tmp_v = it.getTo();
            if (statusArray[tmp_v] == UNDEFINED)
            {
                Q.push(tmp_v);
                statusArray[tmp_v] = T;
                count++;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (statusArray[i] == T && count > 1)
        {
            cout << i << ", ";
            count--;
        }
        else if (statusArray[i] == T)
            cout << i << "." << endl;
    }
}


// Prints the Min Cut that required
void Graph::printMinCut(int s, int t)
{

    queue<int> Q;
    int n = getNumVers();
    int* statusArray = new int[n]{UNDEFINED};

    Q.push(s); statusArray[s] = S;
    update_S_Cut(statusArray, Q);
    Q.push(t); statusArray[t] = T;
    update_T_Cut(statusArray, Q);

    delete[] statusArray;

}

