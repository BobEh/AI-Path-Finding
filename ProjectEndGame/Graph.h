#pragma once

#include <list>
#include <vector>
#include "Vertex.h"
#include <string>

using namespace std;

struct Node
{
	unsigned int id;
	bool visited;
	bool hasResource;
	bool hasBase;
	float gCostSoFar;
	float hDistance;
	string nodeType;
	Vertex position;     //position in the game world. used for calculating heuristic distances
	struct Node* parent; //parent Node that we can follow back to the root node.
	vector<pair< Node*, float>> children; //Edges pointing to neighbouring graph <childNode, edgeWeight>
};


class Graph
{
public:
	Graph();
	void CreateNode(int id, Vertex position, string type, bool bHasResource, bool bHasBase);
	void AddEdge(Node* parent, Node* child, float weight, bool bUndirected = true);
	void ResetGraph();
	void PrintGraph();
	void PrintParents(bool includeCost = false);

	vector<Node*> nodes;
	vector<Node*> parentList;
};