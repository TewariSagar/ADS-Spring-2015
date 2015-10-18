#include "FibonacciHeap.h"
#include <vector>


Edge::Edge(graph_nd* t, graph_nd* hd, double ln)//Declaring edge for adjacency graph
{
	this->tail = t;
	this->head = hd;
	this->length = ln;
}

graph_nd::graph_nd(int d, double k)//overloaded constructor
{
	pred = NULL;//various data  members
	parent = NULL;
	rank = 0;
	children = NULL;
	this->key = k;
	state = UNLABLD;
	L_Sib = NULL;
	this->data = d;
	R_Sib = NULL;
}

graph_nd::graph_nd()//graph constructor
{
	pred = NULL;//various data  members
	parent = NULL;
	rank = 0;
	children = NULL;
	state = UNLABLD;
	L_Sib = NULL;
	R_Sib = NULL;
}

bool graph_nd::child_add(graph_nd *n)//program to add child to a node
{
	if(children)
	{
		children->Sib_add(n);
		return true;
	}
	else
	{
		children = n;
		rank = 1;
		n->parent = this;
		return true;
	}
}

bool graph_nd::Sib_add(graph_nd *nd)// this program adds sibling to a node
{
	graph_nd* p = Sibl_R();
	if(p == NULL)
		return false;

	p->R_Sib = nd;
	nd->L_Sib = p;
	nd->parent = this->parent;
	nd->R_Sib = NULL;

	if(parent!=NULL)
	{
		int i=parent->rank+1;
		parent->rank=i;
		return true;
	}

	return true;
}

bool graph_nd::remove()//removes a node
{
	if(parent!=NULL)
	{
		int j=parent->rank-1;
		parent->rank=j;
		if(L_Sib)
			parent->children = L_Sib;
		else if(R_Sib)
			parent->children = R_Sib;
		else
			parent->children = NULL;
	}	
	
	if(R_Sib)//if right sibling exists
		R_Sib->L_Sib = L_Sib;
	if(L_Sib)//if left sibling exists
		L_Sib->R_Sib = R_Sib;
	
	L_Sib = NULL;//earthing the left sibling 
	parent = NULL;
	R_Sib = NULL;

	return true;
}

void graph_nd::add_in_edge(Edge * edge)//adds an incoming edge to a node
{
	In_Edge.push_back(edge);
}

void graph_nd::add_out_edge(Edge * edge)//adds an outcoming edge to a node
{
	Out_Edge.push_back(edge);
}


graph_nd* graph_nd::Sibl_L()//finds left most sibling for a node
{
	if(!this)
		return NULL;

	graph_nd* m = this;
	while(m->L_Sib)
		m = m->L_Sib;
	return m;
}

graph_nd* graph_nd::Sibl_R()//finds right most sibling for a node
{
	if(!this)
		return NULL;

	graph_nd* m = this;
	while(m->R_Sib)
		m = m->R_Sib;
	return m;
}


FibonacciHeap::FibonacciHeap()
{
	RootMin = NULL;
}

FibonacciHeap::FibonacciHeap(graph_nd *r)//initializes the fibo heap
{
	this->RootMin = r;
	RootMin->R_Sib = NULL;
	RootMin->rank = 0;
	RootMin->children = NULL;
	RootMin->L_Sib = NULL;
	RootMin->parent = NULL;
}

FibonacciHeap::~FibonacciHeap()//deletes the heap once it goes out of scope
{
	delete[] Ranked_root_list;
}

bool FibonacciHeap::isEmpty()//returns true when the heap is empty
{
	return (RootMin == NULL);
}

bool FibonacciHeap::vrtx_ins(graph_nd * n)//to insert a vertex in the heap
{
	if(n == NULL)//if the node in empty
		return false;

	if(!RootMin)
		RootMin = n;
	else
	{
		RootMin->Sib_add(n);
		if(RootMin->key > n->key)
			RootMin = n;
	}
	return true;
}

graph_nd* FibonacciHeap::findMin()//returns the minimum value in fibo heap
{
	return RootMin;
}

graph_nd* FibonacciHeap::minDelete()//deletes the min node from the heap
{
	graph_nd *p = RootMin->children->Sibl_L();
	graph_nd *nextp = NULL;
	
	while(p != NULL)
	{
		nextp = p->R_Sib;//store the nxt sib
		p->remove();
		RootMin->Sib_add(p);
		p = nextp;
	}

	p = RootMin->Sibl_L();

	if(p == RootMin)
	{
		if(RootMin->R_Sib)
			p = RootMin->R_Sib;
		else
		{//since heap is empty
			graph_nd* out = RootMin;
			RootMin->remove();
			RootMin = NULL;
			return out;
		}
	}
	graph_nd* out = RootMin;
	RootMin->remove();
	RootMin = p;
	{
	int i=0;
	while(i<100)
		Ranked_root_list[i++] = NULL;
	}
	while(p)//to see whether key of present vertx is less than the minimum value
	{
		
		if(p->key < RootMin->key)
		{
			RootMin = p;
		}

		nextp = p->R_Sib;		
		link(p);
		p = nextp;
	}

	return out;	
}

bool FibonacciHeap::link(graph_nd* r)//links the different nodes in heap
{
	
	if(Ranked_root_list[r->rank] == NULL)
	{
		Ranked_root_list[r->rank] = r;
		return false;
	}
	else
	{
		
		graph_nd* Lnkvrtx = Ranked_root_list[r->rank];
		Ranked_root_list[r->rank] = NULL;
		
		if(r->key < Lnkvrtx->key || r == RootMin)
		{
			Lnkvrtx->remove();
			r->child_add(Lnkvrtx);
			if(Ranked_root_list[r->rank] != NULL)
				link(r);
			else
				Ranked_root_list[r->rank] = r;
		}
		else
		{
			r->remove();
			Lnkvrtx->child_add(r);
			if(Ranked_root_list[Lnkvrtx->rank] != NULL)
				link(Lnkvrtx);
			else
				Ranked_root_list[Lnkvrtx->rank] = Lnkvrtx;
		}
		return true;
	}
}


void FibonacciHeap::keyDec(double delta, graph_nd* v)
{
	v->key = delta;

	if(v->parent != NULL) // Parent of vertex exists
	{
		v->remove();//adding parent t oroot list
		RootMin->Sib_add(v);		
	}
	if(v->key < RootMin->key)//if present key is the smallest in the heap
		RootMin = v;
}
