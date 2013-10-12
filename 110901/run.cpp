#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

using namespace std;

class Graph
{
public:
	Graph(int n, int m);
	~Graph();
	void insertEdge(int x, int y);
	void bfs();
	
private:
	enum State
	{
		undiscovered, 
		discovered, 
		processed
	};
	
	struct Vertex
	{
		State state;
		int color;
		Vertex *parent;
	};

	struct Node 
	{
		int y;
		Node *next;
	};
	
	int nVertex;
	int nEdge;
	
	Node **edges;
	Vertex *vertexes;
};

Graph::Graph(int n, int m)
{
	nVertex = n;
	nEdge = m;
	
	edges = new Node*[nVertex];
	vertexes = new Vertex[nVertex];
	
	for (int i = 0; i < n; ++i)
	{
		edges[i] = NULL;
		
		vertexes[i].state = undiscovered;
		vertexes[i].color = -1;
		vertexes[i].parent = NULL;
	}
}

void Graph::insertEdge(int x, int y)
{
	Node *node = new Node();
	node->y = y;
	node->next = edges[x];
	edges[x] = node;
}

Graph::~Graph()
{	
	for (int i = 0; i < nVertex; ++i)
	{
		Node *p = edges[i];
		while (p != NULL)
		{
			Node *tmp = p->next;
			delete p;
			p = tmp;
		}
	}
		
	delete[] edges;
	delete[] vertexes;
}

void Graph::bfs()
{
	queue<int> processQueue;
	processQueue.push(0);
	vertexes[0].state = discovered;
	vertexes[0].color = 0;
	
	while (!processQueue.empty())
	{
		int x = processQueue.front();
		processQueue.pop();
		Node *p = edges[x];
		
		while(p != NULL)
		{
			int y = p->y;
			
			if (vertexes[y].color >= 0 && vertexes[y].color == vertexes[x].color)
			{
				cout << "NOT BICOLORABLE." << endl;
				return;
			}
			
			if (vertexes[y].state == undiscovered)
			{
				vertexes[y].parent = &vertexes[x];
				vertexes[y].state = discovered;
				vertexes[y].color = !vertexes[y].parent->color;
				processQueue.push(y);
			}
			
			p = p->next;
		}
		vertexes[x].state = processed;
	}
	
	cout << "BICOLORABLE." << endl;
}


int main(int argc, char *argv[])
{
	ifstream fin("110901.inp");
	string s;
	stringstream ss;
	
	int n, m;
	int x, y;
	while(true)
	{
		getline(fin, s);
		ss << s;
		ss >> n;
		ss.clear();
		
		if (n <= 0) break;
		
		getline(fin, s);
		ss << s;
		ss >> m;
		ss.clear();
		
		Graph graph(n, m);
		
		for (int i = 0; i < m; ++i)
		{
			getline(fin, s);
			ss << s;
			ss >> x >> y;
			ss.clear();
			
			graph.insertEdge(x, y);
			graph.insertEdge(y, x);
		}
		
		graph.bfs();
		
		ss.str("");
	}
	
	return EXIT_SUCCESS;
}


