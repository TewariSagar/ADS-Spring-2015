#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include "FibonacciHeap.h"
using namespace std;

int main(int argc, char *argv[])
{
	int src=atoi(argv[2]),dest=atoi(argv[3]);
	//printf("%d %d\n",src,dest );
	long n;
	//Creating vectors for storing edges and vertices as adjacency list
	vector<graph_nd*> vertices;
	char read[161];
	vector<Edge*> edges;
	//opening file to read
	ifstream indat(argv[1]);

	if(!indat.eof())
	{
		indat.getline(read, 160);
		int q=1;
		while(read[q] != ' ' && read[q]!='\0')//reading number of vertices
			q++;
		string in_str = read;
		long n = atoi(const_cast<char*>(in_str.substr(0, q).c_str()));
		int l=q+1;
		while(read[l] != ' ' && read[l]!='\0')//reading number of edges
			l++;
		int n_edges = atoi(const_cast<char*>(in_str.substr(q+1, l).c_str()));

		for(int j=0; j<=n-1 ; j++)
		{
			int k;
			if(j==dest)
				k=0;//initializing the destination vertex to 0
			else 
				k=-1;
			graph_nd* v= new graph_nd(j, k);
			vertices.push_back(v);
		} 
		vertices[dest]->state = LABELED;
		int count_edg=0;
		//cout<<"No of edges "<<n_edges<<endl;
		while(!indat.eof()&&count_edg<n_edges)
		{
			memset(read, '\0', sizeof(char)*100);
			indat.getline(read, 160);
			//scans out head tail and length of all edges
			//printf("Line is = %s size = %lu\n",read,strlen(read));
			//char str[161]=read.c_str();
			if(!strlen(read))
				if(!indat.eof())
					continue;
				else
					break;
			q=0;
			while(read[q] != ' ' && read[q]!='\0')
				q++;
			count_edg;
			in_str = read;
			int tail = atoi(const_cast<char*>(in_str.substr(0, q).c_str()));
			l=q+1;
			while(read[l] != ' ' && read[l]!='\0')
				l++;
			int head = atoi(const_cast<char*>(in_str.substr(q+1, l).c_str()));
			q=l+1;
			
			while(read[q] != ' ' && read[q]!='\0')
				q++;

			double length = atof(const_cast<char*>(in_str.substr(l+1, q).c_str()));
			//printf("tail = %d head = %d len = %f\n",tail,head,length);
			//printf("tail = %d head = %d len = %f\n",head,tail,length);
			Edge* edge = new Edge(vertices[tail], vertices[head], length);
			edge->head->add_in_edge(edge);
			edge->tail->add_out_edge(edge);
			edges.push_back(edge);
			Edge* edge2 = new Edge(vertices[head], vertices[tail], length);//since graph is undirected 
			edge2->head->add_in_edge(edge2);
			edge2->tail->add_out_edge(edge2);
			edges.push_back(edge2);
			//printf("check=%d\n",indat.eof());
		}	
	}
	else
	{
		printf("Couldn't open the file\n");
		return 0;
	}
	//creating heap
	FibonacciHeap* heap = new FibonacciHeap();

	heap->vrtx_ins(vertices[dest]);//seeding heap with the destination vertex
	long j = 0;
	do//the standard dijkstra's algo
	{
		graph_nd* v = heap->minDelete();
		
		v->state = SCANED;
		
		for(int i = 0; i < v->In_Edge.size(); i++)
		{
			Edge* currEdg = v->In_Edge[i];
			graph_nd* headOfcurrEdg = currEdg->tail;
			//printf("%d ", headOfcurrEdg->data);

			if(headOfcurrEdg->state != SCANED)
				{
				if(headOfcurrEdg->state == UNLABLD)
				{
					headOfcurrEdg->state = LABELED;
					headOfcurrEdg->pred = v;
					headOfcurrEdg->key = v->key + currEdg->length;
					heap->vrtx_ins(headOfcurrEdg);
				}
				else if(headOfcurrEdg->key > v->key + currEdg->length )
				{
					headOfcurrEdg->pred = v;//storing predecessor of each vertex 
					heap->keyDec(v->key + currEdg->length, headOfcurrEdg);
				}
			}
		}
		//printf("\n");
	}
	while(!heap->isEmpty());

	graph_nd* temp = vertices[src];//we need to start from the mentioned source

	if(!temp->pred)
	{
		printf("There exist no s-t paths\n");
		return 0;
	}

	int vertexCount = 0;
	printf("%f\n", vertices[src]->key);
	
	while(temp)
	{
		printf("%d ", temp->data);
		vertexCount++;
		//if(temp->data==src)
		//	break;
		temp = temp->pred;
	}
	printf("\n");
	return 0;
}
