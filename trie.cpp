#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
using namespace std;
//defining structure of trie nodes
struct node
{
	int next_hop;
	struct node *zero,*one;
};
//creates new trie nodes
struct node *crNode()
{
	struct node *temp= new node;
	temp->zero=NULL;
	temp->one=NULL;
	temp->next_hop=-1;
	return temp;
}
//adds ip addresses to the router specified by **root. nhop has the next_hop value 
void trie_add(struct node **root,char *str,int nhop)
{
	if(*root==NULL)
	{
		*root=crNode();
	}
	int level;
	struct node *crawler=*root;
	for(level=0;level<32;level++)
	{
		if(str[level]=='1')
		{
			if (!crawler->one)
				crawler->one=crNode();
			crawler=crawler->one;
		}
		else
		{
			if (!crawler->zero)
				crawler->zero=crNode();
			crawler=crawler->zero;
		}
	}
	crawler->next_hop=nhop;
}
//used in postorder trimming of trie
void del(struct node *root)
{
	if(root)
	{
		if((root)->zero)
			del((root)->zero);
		if((root)->one)
			del((root)->one);
		free((root));
	}
}
int trim_trie(struct node **root)
{
	if(*root)
	{
		int l,r;
		l=trim_trie(&((*root)->zero));
		r=trim_trie(&((*root)->one));
		if(l==-1&&r==-1)
			return (*root)->next_hop;//leaf node has the next_hop value
		else if(l==-1&&r!=-1)
			return r;
		else if(l!=-1&&r==-1)//returns next_hop value for a prefix match upto this node
			return l;
		else
		{
			if(l==r)//deletes both subtrees iff both zero and one pointers lead to the same next_hop and are not -1
			{
				(*root)->next_hop=l;//we need to change this as it is going to b a leaf node now
				del(((*root)->zero));
				(*root)->zero=NULL;
				del(((*root)->one));
				(*root)->one=NULL;
				return l;
			}
			else
				return -1;
		}
	}
	else 
		return -1;
}
void find_gateway(node **routing_table,int start,char *ip,int end)
{
	if(start==end)
	{
		//cout<<"Reached "<<end<<"!"<<endl;
		return;
	}
	struct node *ptr=routing_table[start];
	struct node *par=NULL;//points to the parent of crawling pointer, used to get next_hop
	int ind=0;
	while(ptr&&ip[ind])//termionates if ip reaches end or ptr becomes NULL
	{
		if(ip[ind]=='0')
		{
			par=ptr;
			ptr=ptr->zero;
			ind++;
		}
		else if(ip[ind]=='1')
		{
			par=ptr;
			ptr=ptr->one;
			ind++;
		}
	}
	if(!ip[ind])//if ip reaches end
	{
		ind=31;
		if(ptr)
			par=ptr;
	}
	else//if ptr becomes NULL
		ind=ind-2;
	for(int i=0;i<=ind;i++)//prints the string matched
		cout<<ip[i];
	cout<<" ";
	if(par->next_hop!=-1)
	{
		//cout<<"going to "<<par->next_hop<<endl;
		find_gateway(routing_table,par->next_hop,ip,end);//goes to next router
	}
}
void dec_to_bin(char *binaddr,char *tempbin, int n)//converts a number to its binary representation
{
	for(int i=0;i<8;i++)
	{
		tempbin[7-i]=n&1?'1':'0';
		n=n>>1;
	}
	strcat(binaddr,tempbin);
}
void convert(char *ip_addr,char *binaddr)//converts an ip address to 4 numbers and then to its binary representation
{
	int n1,n2,n3,n4;
	sscanf(ip_addr,"%d.%d.%d.%d",&n1,&n2,&n3,&n4);
	char tempbin[9]="";
	dec_to_bin(binaddr,tempbin,n1);
	dec_to_bin(binaddr,tempbin,n2);
	dec_to_bin(binaddr,tempbin,n3);
	dec_to_bin(binaddr,tempbin,n4);
}
int main(int argc, char const *argv[])
{
	char ip_addr[16]="";
	char binaddr[33]="";
	int start =atoi(argv[3]);//reads starting router
	int end =atoi(argv[4]);
	ifstream graph(argv[1]);
	string inp;
	getline(graph,inp);
	int k=1;
	while(inp[k] != ' ' && inp[k]!='\0')//reading number of vertices
		k++;
	long vertices = atoi(const_cast<char*>(inp.substr(0, k).c_str()));
	//long edges = atoi(const_cast<char*>(inp.substr(k+1,inp.length()).c_str()));
	ifstream ips(argv[2]);//opening ip addresses of routers
	char ip_addresses[vertices][16];
	char bin_addresses[vertices][33];//stores binary representations of IPs
	struct node *routing_table[vertices];//creating routing table
	int weight[vertices][vertices];
	memset(weight,-1,sizeof(vertices*vertices));
	k=0;
	int c=0;
	while(!ips.eof())
	{
		char temp[160];
		ips.getline(temp,160);//reads IP addresses
		int leng=strlen(temp);
		//printf("Line is = %s size = %d\n",temp,leng);
			//char str[161]=inp.c_str();
		if(!leng)
			if(!ips.eof())
				continue;
			else
				break;
		else
		{
			for(int j=0;j<leng;j++)
				ip_addresses[k][j]=temp[j];
			ip_addresses[k][leng]='\0';
			k++;
			c++;
		}
	}
	for(k=0;k<vertices;k++)//converting IP to binary form
	{
		convert(ip_addresses[k],binaddr);
		strcpy(bin_addresses[k],binaddr );
		//cout<<binaddr<<endl;
		binaddr[0]='\0';
	}
	for(int dest=0;dest<vertices;dest++)//creating routing tables
	{
		routing_table[dest]=NULL;//initializing routing table
		for(int src=0;src<vertices;src++)
		{
			if(src!=dest)
			{

				char command[100];
				sprintf(command,"./ssp %s %d %d",argv[1],src, dest);//calls ssp with the required arguments
				//cout<<"Command is "<<command<<endl;
				FILE *out=popen(command,"r");//points to output obtained from ssp
				if(out)
				{
					char  input[2049];
					fgets(input,2048,out);
					weight[dest][src]=atoi(input);
					fgets(input,2048,out);

					int p=strlen(input)-1;
					while(p>=0&&input[p]!=' ')
						p--;
					p--;
					while(p>=0&&input[p]!=' ')
						p--;//p points to the next_hop value in the output string from ssp
					int next_hop,tem;
					sscanf(&input[p-1],"%d %d",&next_hop,&tem);//scans and stores next_hop
					pclose(out);
					trie_add(&routing_table[dest],bin_addresses[src],next_hop);//adds IP to routing table of dest
				}
				else
				{
					cout<<"Couldn't execute "<<command<<endl;
				}
			}

		}
		trim_trie(&routing_table[dest]);

	}
	//cout<<"Start from "<<start<<endl;
	cout<<weight[start][end]<<endl;
	find_gateway(routing_table,start,bin_addresses[end],end);
	cout<<endl;
	return 0;
}
