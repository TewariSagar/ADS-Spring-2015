#ifndef _FIBONACCI_HEAP_H_
#define _FIBONACCI_HEAP_H_

#include <stdio.h>
#include <vector> 


enum State
{
	LABELED, UNLABLD, SCANED
};

class graph_nd;

class Edge
{
private:
public:
	graph_nd* tail;
	graph_nd* head;
	double length;
	double delta;

	Edge(graph_nd* tail, graph_nd* head, double length);
};


class graph_nd
{
private:
public:
	graph_nd * parent;
	graph_nd * L_Sib, * R_Sib;
	graph_nd * children; 
	graph_nd * pred;

	graph_nd(int data, double key);
	graph_nd();

	int data;
	double key;
	int rank;

	std::vector<Edge*> In_Edge;
	std::vector<Edge*> Out_Edge;

	State state;

	bool child_add(graph_nd * node);
	bool Sib_add(graph_nd * node);
	bool remove();
	
	graph_nd* Sibl_L();
	graph_nd* Sibl_R();

	void add_in_edge(Edge * edge);
	void add_out_edge(Edge * edge);

};

class FibonacciHeap
{
private:
	graph_nd* Ranked_root_list[100];

	bool link(graph_nd* root);	
	graph_nd * RootMin;

public:

	FibonacciHeap();
	FibonacciHeap(graph_nd *root);

	~FibonacciHeap();

	bool isEmpty();
	bool vrtx_ins(graph_nd * node);	
	void keyDec(double delta, graph_nd* vertex);

	graph_nd* findMin();
	graph_nd* minDelete();
};

#endif
