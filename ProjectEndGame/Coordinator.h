#pragma once

#include <iostream>
#include <stdio.h>

#include <stack>
#include <queue>
#include <list>

#include "iObject.h"
#include "Behaviour.h"
#include "Graph.h"

class Coordinator {
public:
	enum class states
	{
		idle,
		search,
		returnToBase,
		gathering,
		depositing,
		findResource,
		findBase
	};
	Coordinator(void);
	~Coordinator(void);

	void SetTarget(glm::vec3 target);

	void AddVehicle(iObject* vehicle);
	void RemoveVehicle(iObject* vehicle);

	void SetCoordinatorObject(iObject* coordinator);
	iObject* GetCoordinatorObject();

	void SetCommand(std::string command);
	void SetFormation(std::string formation);
	void CreateResourcePath(std::vector<Node*> thePath);
	void CreateBasePath(std::vector<Node*> thePath);

	Node* GetCurrentPathPoint();
	std::vector<Node*> GetCurrentPath();
	int GetCurrentPathSection();
	std::string GetCurrentCommand();

	void SetGraph(Graph* graph);

	void update(float dt);

	states currentState;

	iObject* theCoordinator;

	Graph* theGraph;

	Node* rootNode;
	Node* currentNode;
	Node* currentResourceNode;
	Node* baseNode;
	Node* goalNode;
	bool hasResource;

private:
	void Seek();
	void MakeFormation();
	void FindResource();
	void FindBase();
	void Idle();
	void Deposit(float dt);
	void Gather(float dt);
	void FindNewResource();
	void GoToBase();
	void Flock();
	glm::vec3 Separation(iObject* currentObject);
	glm::vec3 Alignment(iObject* currentObject);
	glm::vec3 Cohesion(iObject* currentObject);
	glm::vec3 CoSeek(iObject* currentObject, glm::vec3 target);

	std::vector<iObject*> vehicles;
	std::vector<glm::vec3> currentFormation;
	std::vector<glm::vec3> squareOffsets;
	std::vector<glm::vec3> circleOffsets;
	std::vector<glm::vec3> vOffsets;
	std::vector<glm::vec3> lineOffsets;
	std::vector<glm::vec3> rowsOffsets;
	std::vector<Node*> resourcePath;
	std::vector<Node*> basePath;
	std::string currentCommand;
	glm::vec3 currentTarget;

	int pathSection;

	float offsetX;
	float offsetZ;

	float depositTime;
	float gatherTime;

};