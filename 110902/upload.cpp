#include <cstdlib>
#include <iostream>
#include <queue>

using namespace std;

class Graph
{
public:
	Graph();
	virtual ~Graph();
	void deleteVertex(int n);
	int bfs(int init, int target);

private:
	enum State
	{
		undiscovered, 
		discovered, 
		processed
	};
	
	struct Vertex
	{
		int length;
		State state;
		Vertex *parent;
	};

	struct Node
	{
		int y;
		Node *next;
	};

	int nVertex;
	Vertex *vertexes;
	Node **edges;
};

Graph::Graph()
{
	nVertex = 10000;
	vertexes = new Vertex[nVertex];
	edges = new Node*[nVertex];
	
	int digits[4];
	int neighbour[4];
	for (int i = 0; i < nVertex; ++i)
	{
		vertexes[i].length = 0;
		vertexes[i].state = undiscovered;
		vertexes[i].parent = NULL;
	
		edges[i] = NULL;
			
		digits[3] = i / 1000;
		digits[2] = i / 100 - digits[3] * 10;
		digits[1] = i / 10 - digits[3] * 100 - digits[2] * 10;
		digits[0] = i % 10;
		
		#pragma unroll
		for (int k = 0; k < 8; ++k)
		{
			neighbour[0] = digits[0];
			neighbour[1] = digits[1];
			neighbour[2] = digits[2];
			neighbour[3] = digits[3];
			neighbour[k / 2] = (digits[k / 2] + (k % 2 * 2 - 1) + 20) % 10;

			Node *node = new Node();
			node->y = neighbour[3] * 1000 + neighbour[2] * 100 + neighbour[1] * 10 + neighbour[0];
			node->next = edges[i];
			edges[i] = node;
		}
	}
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
		edges[i] = NULL;
	}
	delete[] edges;
	delete[] vertexes;
}

void Graph::deleteVertex(int n)
{
	Node *p = edges[n];
	while (p != NULL)
	{
		int y = p->y;
	
		Node *tmp = p->next;
		delete p;
		p = tmp;
		
		tmp = edges[y];
		Node *parent = NULL;
		while (true)
		{
			if (tmp->y == n)
			{
				if (parent == NULL)
				{
					edges[y] = tmp->next;
				}
				else
				{
					parent->next = tmp->next;
				}
				delete tmp;
				break;			
			}
			parent = tmp;
			tmp = tmp->next;
		}
	}
	edges[n] = NULL;
}

int Graph::bfs(int init, int target)
{
	queue<int> processQueue;
	processQueue.push(init);
	vertexes[init].state = discovered;
	
	if (init == target) return 0;
	
	while (!processQueue.empty())
	{
		int x = processQueue.front();
		processQueue.pop();
		
		Node *p = edges[x];
		while (p != NULL)
		{
			int y = p->y;
			if (vertexes[y].state == undiscovered)
			{
				vertexes[y].state = discovered;
				vertexes[y].parent = &vertexes[x];
				vertexes[y].length = vertexes[y].parent->length + 1;
				processQueue.push(y);
				
				if (y == target) 
				{
					return vertexes[y].length;
				}
			}
			p = p->next;
		}
	}
	
	return -1;
}

int main(int argc, char *argv[])
{	
	int nTestCase = 0;
	cin >> nTestCase;
	
	//cin.get();
	
	int value[4];
	int initValue, targetValue;
	
	for (int idx = 0; idx < nTestCase; ++idx)
	{
		Graph g;
		
		cin >> value[3] >> value[2] >> value[1] >> value[0];
		initValue = value[3] * 1000 + value[2] * 100 + value[1] * 10 + value[0];
		
		cin >> value[3] >> value[2] >> value[1] >> value[0];
		targetValue = value[3] * 1000 + value[2] * 100 + value[1] * 10 + value[0];
		
		int nForbid = 0;
		cin >> nForbid;
	
		for (int i = 0; i < nForbid; ++i)
		{
			cin >> value[3] >> value[2] >> value[1] >> value[0];
			g.deleteVertex(value[3] * 1000 + value[2] * 100 + value[1] * 10 + value[0]);
		}
		cout << g.bfs(initValue, targetValue) << endl;
		
		if (idx < nTestCase - 1) cin.get();
	}
	
	return EXIT_SUCCESS;
}

