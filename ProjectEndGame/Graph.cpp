#include "Graph.h"

#include <iostream>

Graph::Graph() {}


void Graph::CreateNode(int id, Vertex position, string type, bool bHasResource, bool bHasBase)
{
	Node* node = new Node;
	node->id = id;
	node->position = position;
	node->hasResource = bHasResource;
	node->hasBase = bHasBase;
	node->gCostSoFar = FLT_MAX;
	node->hDistance = 0;
	node->parent = NULL;
	node->visited = false;
	node->nodeType = type;

	this->nodes.push_back(node);
}

void Graph::AddEdge(Node* parent, Node* child, float weight, bool bUndirected)
{
	pair<Node*, float> edge;
	edge.first = child;
	edge.second = weight;
	parent->children.push_back(edge);

	//this if statement is because too I'm lazy to write out a second AddEdge function call in the main.
	//Recommend removing this if statement in your assignment. It will come back and bite you.
	//if (bUndirected)
	//{
	//	pair<Node*, float> reverseEdge;
	//	reverseEdge.first = parent;
	//	reverseEdge.second = weight;
	//	child->children.push_back(reverseEdge);
	//}
}

void Graph::ResetGraph() {
	for (Node*& currNode : this->nodes)
	{
		currNode->visited = false;
		currNode->parent = NULL;
		currNode->gCostSoFar = FLT_MAX;
	}
}

void Graph::PrintGraph() {
	for (Node*& currNode : this->nodes)
	{
		cout << "Node: " << currNode->id << " -> ";
		for (pair <Node*, float>& child : currNode->children)
		{
			cout << "( " << child.first->id << ", " << child.second << ") ";
		}
		cout << endl;
	}
}

void Graph::PrintParents(bool includeCost)
{
	for (Node*& currNode : this->nodes)
	{
		//cout << "Node: " << currNode->id << " -> ";
		if (currNode->parent != NULL)
		{
			//cout << currNode->parent->id;
			parentList.push_back(currNode);
		}
		else
		{
			//cout << "NULL";
		}
		if (includeCost)
		{
			//cout << " cost so far: " << currNode->gCostSoFar << " hDist= " << currNode->hDistance << " f= " << currNode->gCostSoFar + currNode->hDistance;
		}
		//cout << endl;
	}
}