#pragma once
class Edge
{
private:
	int to, capacity, flow;	
public:
	int getTo() { return to; }
	int getCapacity() { return capacity; }
	int getFlow() {	return flow; }
	void changeTo(int new_to) { to = new_to; }
	void changeCapacity(int new_capacity) { capacity = new_capacity; }
	void changeFlow(int new_flow) { flow = new_flow; }
};

