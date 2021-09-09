#include "Coordinator.h"

bool IsNodeInOpenList(vector<Node*> openList, Node* child)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->id == child->id)
			return true;

	}
	return false;
}


Node* Dijkstra(Node* rootNode, Graph* graph, std::vector<int> &groceryList)
{
	if (groceryList.size() == 0)
	{
		return NULL;
	}

	graph->ResetGraph();

	rootNode->visited = true;
	rootNode->gCostSoFar = 0;
	vector<Node*> closedList;
	vector<Node*> openList;
	openList.push_back(rootNode);

	while (!openList.empty())
	{
		float minCost = FLT_MAX;
		int minIndex = 0;
		Node* currNode;
		//find node with the lowest cost from root node
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i]->gCostSoFar < minCost)
			{
				minCost = openList[i]->gCostSoFar;
				minIndex = i;
			}
		}

		//remove current node from open list and add to closed list
		currNode = openList[minIndex];
		for (auto iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (*iter == currNode)
			{
				openList.erase(iter);
				break;
			}
		}
		closedList.push_back(currNode);

		std::cout << currNode->id << endl;
		currNode->visited = true;
		if (currNode->hasResource && currNode->id == groceryList[groceryList.size() - 1])
		{
			groceryList.pop_back();
			return currNode;
		}

		//Go through every child node node 
		for (pair <Node*, float> child : currNode->children)
		{
			if (child.first->visited == false)
			{
				float weightSoFar = currNode->gCostSoFar + child.second;
				if (weightSoFar < child.first->gCostSoFar)
				{
					//update node when new better path is found
					child.first->gCostSoFar = weightSoFar;
					child.first->parent = currNode;
					if (!IsNodeInOpenList(openList, child.first))
					{
						openList.push_back(child.first); //add newly discovered node to open list
					}
				}
			}
		}
	}

	return NULL;
}

float CalculateHeuristics(Node* node, Node* goal)
{
	float D = 1;
	float dx = abs(node->position.x - goal->position.x);
	float dy = abs(node->position.y - goal->position.y);
	return D * (dx + dy);
}

Node* AStar(Node* rootNode, Graph* graph, Node* goal)
{
	graph->ResetGraph();

	rootNode->gCostSoFar = 0;
	rootNode->hDistance = CalculateHeuristics(rootNode, goal);

	vector<Node*> closedList;
	vector<Node*> openList;
	openList.push_back(rootNode);

	while (!openList.empty())
	{
		float minCost = FLT_MAX;
		int minIndex = 0;
		Node* currNode;
		//find node with the lowest cost from root node and heuristic distance from the goal node
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i]->gCostSoFar + openList[i]->hDistance < minCost)
			{
				minCost = openList[i]->gCostSoFar + openList[i]->hDistance;
				minIndex = i;
			}
		}

		//remove current node from open list and add to closed list
		currNode = openList[minIndex];
		for (auto iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (*iter == currNode)
			{
				openList.erase(iter);
				break;
			}
		}
		closedList.push_back(currNode);

		std::cout << currNode->id << endl;
		currNode->visited = true;
		if (currNode->id == goal->id)
		{
			return currNode;
		}

		//Go through every child node node 
		for (pair <Node*, float> child : currNode->children)
		{
			if (child.first->visited == false)
			{
				float weightSoFar = currNode->gCostSoFar + child.second;
				if (weightSoFar < child.first->gCostSoFar)
				{
					child.first->gCostSoFar = weightSoFar;
					child.first->parent = currNode;
					if (!IsNodeInOpenList(openList, child.first))
					{
						child.first->hDistance = CalculateHeuristics(child.first, goal);
						openList.push_back(child.first);
					}
				}
			}
		}
		graph->PrintParents(true);
	}

	return NULL;
}

Coordinator::Coordinator(void) : currentCommand("none"), currentTarget(glm::vec3(0.0f,0.0f,0.0f)), offsetX(-60.0f), offsetZ(-60.0f), pathSection(0)
{
	this->theCoordinator = nullptr;
	//square
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, -20.0f));
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, 20.0f));
	squareOffsets.push_back(glm::vec3(-60.0f, 0.0f, 60.0f));
	squareOffsets.push_back(glm::vec3(-20.0f, 0.0f, 60.0f));
	squareOffsets.push_back(glm::vec3(-20.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(20.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(20.0f, 0.0f, 60.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, -60.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, -20.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, 20.0f));
	squareOffsets.push_back(glm::vec3(60.0f, 0.0f, 60.0f));

	vOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	vOffsets.push_back(glm::vec3(-30.0f, 0.0f, -30.0f));
	vOffsets.push_back(glm::vec3(-60.0f, 0.0f, -60.0f));
	vOffsets.push_back(glm::vec3(-90.0f, 0.0f, -90.0f));
	vOffsets.push_back(glm::vec3(-120.0f, 0.0f, -120.0f));
	vOffsets.push_back(glm::vec3(-150.0f, 0.0f, -150.0f));
	vOffsets.push_back(glm::vec3(30.0f, 0.0f, -30.0f));
	vOffsets.push_back(glm::vec3(60.0f, 0.0f, -60.0f));
	vOffsets.push_back(glm::vec3(90.0f, 0.0f, -90.0f));
	vOffsets.push_back(glm::vec3(120.0f, 0.0f, -120.0f));
	vOffsets.push_back(glm::vec3(150.0f, 0.0f, -150.0f));
	vOffsets.push_back(glm::vec3(180.0f, 0.0f, -180.0f));

	circleOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	circleOffsets.push_back(glm::vec3(-30.0f, 0.0f, -30.0f));
	circleOffsets.push_back(glm::vec3(-60.0f, 0.0f, -60.0f));
	circleOffsets.push_back(glm::vec3(-90.0f, 0.0f, -90.0f));
	circleOffsets.push_back(glm::vec3(-120.0f, 0.0f, -60.0f));
	circleOffsets.push_back(glm::vec3(-150.0f, 0.0f, -30.0f));
	circleOffsets.push_back(glm::vec3(-180.0f, 0.0f, 0.0f));
	circleOffsets.push_back(glm::vec3(-30.0f, 0.0f, 30.0f));
	circleOffsets.push_back(glm::vec3(-60.0f, 0.0f, 60.0f));
	circleOffsets.push_back(glm::vec3(-90.0f, 0.0f, 90.0f));
	circleOffsets.push_back(glm::vec3(-120.0f, 0.0f, 60.0f));
	circleOffsets.push_back(glm::vec3(-150.0f, 0.0f, 30.0f));

	lineOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-30.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-60.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-90.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-120.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-150.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(-180.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(30.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(60.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(90.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(120.0f, 0.0f, 0.0f));
	lineOffsets.push_back(glm::vec3(150.0f, 0.0f, 0.0f));

	rowsOffsets.push_back(glm::vec3(30.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(0.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(-30.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(-60.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(60.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(90.0f, 0.0f, 30.0f));
	rowsOffsets.push_back(glm::vec3(0.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(30.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(-60.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(-30.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(60.0f, 0.0f, -30.0f));
	rowsOffsets.push_back(glm::vec3(90.0f, 0.0f, -30.0f));

	currentFormation = squareOffsets;
}

Coordinator::~Coordinator(void)
{
}

void Coordinator::SetTarget(glm::vec3 target)
{
	this->currentTarget = target;
}

bool Coordinator::GetInfected()
{
	return this->infected;
}

void Coordinator::SetInfected(bool status)
{
	this->infected = status;
}

void Coordinator::Seek()
{
	/*calculates the desired velocity */
		/*Seek uses target position - current position*/
		/*Flee uses current position - target position*/
	glm::vec3 desiredVelocity = this->currentTarget - theCoordinator->getPositionXYZ();

	/* get the distance from target */
	float dist = glm::distance(theCoordinator->getPositionXYZ(), this->currentTarget);

	glm::normalize(desiredVelocity);

	float maxVelocity = 10.0f;
	float arrived = 2.0f;

	/*is the game object within the radius around the target */
	if (dist < arrived)
	{
		/* game object is approaching the target and slows down*/
		//desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		this->currentCommand = "none";
	}
	else
	{
		/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - theCoordinator->getVelocity();

	/* add steering force to current velocity*/
	theCoordinator->setVelocity(steer * 0.03f);

	if (glm::length(theCoordinator->getVelocity()) > maxVelocity)
	{
		theCoordinator->setVelocity(glm::normalize(theCoordinator->getVelocity()) * maxVelocity);
	}
}

void Coordinator::MakeFormation()
{
	for (int i = 0; i < vehicles.size(); i++)
	{
		/*calculates the desired velocity */
		/*Seek uses target position - current position*/
		/*Flee uses current position - target position*/
		glm::vec3 desiredPosition = glm::vec3(theCoordinator->getPositionXYZ().x, 13.0f, theCoordinator->getPositionXYZ().z) + currentFormation.at(i);
		
		glm::vec3 desiredVelocity = desiredPosition - vehicles.at(i)->getPositionXYZ();

		/* get the distance from target */
		float dist = glm::distance(vehicles.at(i)->getPositionXYZ(), desiredPosition);

		glm::normalize(desiredVelocity);

		//float dotAngle = glm::dot(desiredVelocity, glm::vec3(0.0f, 0.0f, 1.0f));

		//vehicles.at(i)->setRotationXYZ(glm::vec3(0.0f, dotAngle, 0.0f));

		float maxVelocity = 25.0f;
		float slowingRadius = 5.0f;

		///*is the game object within the radius around the target */
		//if (dist < slowingRadius)
		//{
		//	/* game object is approaching the target and slows down*/
		//	desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		//}
		//else
		//{
			/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
		//}

		/*calculate the steering force */
		glm::vec3 steer = desiredVelocity - vehicles.at(i)->getVelocity();

		/* add steering force to current velocity*/
		vehicles.at(i)->setVelocity(steer * 0.03f);

		if (glm::length(vehicles.at(i)->getVelocity()) > maxVelocity)
		{
			vehicles.at(i)->setVelocity(glm::normalize(vehicles.at(i)->getVelocity()) * maxVelocity);
		}
	}
}

void Coordinator::Checkout()
{
	glm::vec3 desiredVelocity = glm::vec3(this->cashierPath.at(pathSection)->position.x, this->cashierPath.at(pathSection)->position.y + 10.0f, this->cashierPath.at(pathSection)->position.z) - theCoordinator->getPositionXYZ();

	/* get the distance from target */
	float dist = glm::distance(theCoordinator->getPositionXYZ(), glm::vec3(this->cashierPath.at(pathSection)->position.x, this->cashierPath.at(pathSection)->position.y + 10.0f, this->cashierPath.at(pathSection)->position.z));

	glm::normalize(desiredVelocity);

	float maxVelocity = 30.0f;
	float arrived = 2.0f;

	/*is the game object within the radius around the target */
	if (dist < arrived)
	{
		/* game object is approaching the target and slows down*/
		//desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		//this->currentCommand = "none";
		if (pathSection < cashierPath.size() - 1)
		{
			pathSection++;
		}
		else
		{
			currentCommand = "none";
			currentNode = cashierPath.at(pathSection);
			currentState = states::depositing;
			pathSection = 0;
		}
	}
	else
	{
		/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - theCoordinator->getVelocity();

	/* add steering force to current velocity*/
	theCoordinator->setVelocity(steer * 0.3f);

	if (glm::length(theCoordinator->getVelocity()) > maxVelocity)
	{
		theCoordinator->setVelocity(glm::normalize(theCoordinator->getVelocity()) * maxVelocity);
	}
}

void Coordinator::GoToBase()
{
	/*calculates the desired velocity */
	/*Seek uses target position - current position*/
	/*Flee uses current position - target position*/
	//std::cout << "Current path point: " << std::endl << "X: " << this->basePath.at(pathSection)->position.x << ", Y: " << this->basePath.at(pathSection)->position.y << ", Z: " << this->basePath.at(pathSection)->position.z << std::endl;
	//std::cout << "Path section: " << pathSection << std::endl;
	//std::cout << "Current gatherer position: " << std::endl << "X: " << this->theCoordinator->getPositionXYZ().x << ", Y: " << this->theCoordinator->getPositionXYZ().y << ", Z: " << this->theCoordinator->getPositionXYZ().z << std::endl;
	glm::vec3 desiredVelocity = glm::vec3(this->basePath.at(pathSection)->position.x, this->basePath.at(pathSection)->position.y + 10.0f, this->basePath.at(pathSection)->position.z) - theCoordinator->getPositionXYZ();

	/* get the distance from target */
	float dist = glm::distance(theCoordinator->getPositionXYZ(), glm::vec3(this->basePath.at(pathSection)->position.x, this->basePath.at(pathSection)->position.y + 10.0f, this->basePath.at(pathSection)->position.z));

	glm::normalize(desiredVelocity);

	float maxVelocity = 30.0f;
	float arrived = 2.0f;

	/*is the game object within the radius around the target */
	if (dist < arrived)
	{
		/* game object is approaching the target and slows down*/
		//desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		//this->currentCommand = "none";
		if (pathSection < basePath.size() - 1)
		{
			pathSection++;
		}
		else
		{			
			currentCommand = "none";
			currentNode = basePath.at(pathSection);
			currentState = states::depositing;
			pathSection = 0;
			finishedShopping = true;
		}
	}
	else
	{
		/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - theCoordinator->getVelocity();

	/* add steering force to current velocity*/
	theCoordinator->setVelocity(steer * 0.3f);

	if (glm::length(theCoordinator->getVelocity()) > maxVelocity)
	{
		theCoordinator->setVelocity(glm::normalize(theCoordinator->getVelocity()) * maxVelocity);
	}
}

void Coordinator::FindResource()
{
	/*calculates the desired velocity */
	/*Seek uses target position - current position*/
	/*Flee uses current position - target position*/
	/*std::cout << "Current path point: " << std::endl << "X: " << this->resourcePath.at(pathSection)->position.x << ", Y: " << this->resourcePath.at(pathSection)->position.y << ", Z: " << this->resourcePath.at(pathSection)->position.z << std::endl;
	std::cout << "Path section: " << pathSection << std::endl;
	std::cout << "Current gatherer position: " << std::endl << "X: " << this->theCoordinator->getPositionXYZ().x << ", Y: " << this->theCoordinator->getPositionXYZ().y << ", Z: " << this->theCoordinator->getPositionXYZ().z << std::endl;*/
	glm::vec3 desiredVelocity = glm::vec3(this->resourcePath.at(pathSection)->position.x, this->resourcePath.at(pathSection)->position.y + 10.0f, this->resourcePath.at(pathSection)->position.z) - theCoordinator->getPositionXYZ() + glm::vec3(1.f, 1.f, 1.f);

	/* get the distance from target */
	float dist = glm::distance(theCoordinator->getPositionXYZ(), glm::vec3(this->resourcePath.at(pathSection)->position.x, this->resourcePath.at(pathSection)->position.y + 10.0f, this->resourcePath.at(pathSection)->position.z));

	glm::normalize(desiredVelocity);

	float maxVelocity = 20.0f;
	float arrived = 2.0f;

	/*is the game object within the radius around the target */
	if (dist < arrived)
	{
		/* game object is approaching the target and slows down*/
		//desiredVelocity = desiredVelocity * maxVelocity * (dist / slowingRadius);
		//this->currentCommand = "none";
		if (pathSection < resourcePath.size() - 1)
		{
			pathSection++;
		}
		else
		{			
			currentCommand = "none";
			currentNode = resourcePath.at(pathSection);
			currentState = states::gathering;
			pathSection = 0;
		}
	}
	else
	{
		/* target is far away from game object*/
		desiredVelocity *= maxVelocity;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - theCoordinator->getVelocity();

	/* add steering force to current velocity*/
	theCoordinator->setVelocity(steer * 0.3f);

	if (glm::length(theCoordinator->getVelocity()) > maxVelocity)
	{
		theCoordinator->setVelocity(glm::normalize(theCoordinator->getVelocity()) * maxVelocity);
	}
}

float separationRadius = 50.0f;
float maxVelocity = 15.0f;
float maxForce = 15.0f;

glm::vec3 Coordinator::Separation(iObject* currentObject)
{
	glm::vec3 totalFlee = glm::vec3(0.0f);
	int neighbourCount = 0;
	for (int i = 0; i < vehicles.size(); i++)
	{
		if (currentObject->getFriendlyName() == vehicles.at(i)->getFriendlyName())
		{
			continue;
		}
		float closeEnough = glm::distance(currentObject->getPositionXYZ(), vehicles.at(i)->getPositionXYZ());
		if (closeEnough > 0.0f && closeEnough < separationRadius)
		{
			glm::vec3 fleeVector = currentObject->getPositionXYZ() - vehicles.at(i)->getPositionXYZ();
			glm::vec3 normalFlee = glm::normalize(fleeVector);
			totalFlee += normalFlee;
			neighbourCount++;
		}
	}
	glm::vec3 steerForce = glm::vec3(0.0f, 0.0f, 0.0f);
	if (neighbourCount > 0)
	{
		glm::vec3 desiredVelocity = totalFlee / (float)neighbourCount;
		glm::vec3 normalDesire = glm::normalize(desiredVelocity);
		normalDesire *= maxVelocity;
		steerForce = normalDesire - currentObject->getVelocity();
		steerForce += maxForce;
	}
	return steerForce;
}

glm::vec3 Coordinator::Alignment(iObject* currentObject)
{
	glm::vec3 totalVelocity = glm::vec3(0.0f);
	float alignmentRadius = 50.0f;
	int neighbourCount = 0;
	for (int i = 0; i < vehicles.size(); i++)
	{
		if (currentObject->getFriendlyName() == vehicles.at(i)->getFriendlyName())
		{
			continue;
		}
		float distance = glm::distance(currentObject->getPositionXYZ(), vehicles.at(i)->getPositionXYZ());
		if (distance > 0.0f && distance < alignmentRadius)
		{
			totalVelocity += vehicles.at(i)->getVelocity();
			neighbourCount++;
		}
	}
	glm::vec3 steerForce = glm::vec3(0.0f);
	if (neighbourCount > 0)
	{
		glm::vec3 desiredVelocity = totalVelocity / (float)neighbourCount;
		glm::vec3 normalDesire = glm::normalize(desiredVelocity);
		normalDesire *= maxVelocity;
		steerForce = normalDesire - currentObject->getVelocity();
		steerForce += maxForce;
	}
	return steerForce;
}

glm::vec3 Coordinator::CoSeek(iObject* currentObject, glm::vec3 target)
{
	/*calculates the desired velocity */
	/*Seek uses target position - current position*/
	/*Flee uses current position - target position*/
	glm::vec3 desiredVelocity = target - currentObject->getPositionXYZ();

	/* get the distance from target */
	float dist = glm::distance(currentObject->getPositionXYZ(), target);

	glm::normalize(desiredVelocity);

	float maxVelocity = 10.0f;

	/* target is far away from game object*/
	desiredVelocity *= maxVelocity;

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - currentObject->getVelocity();

	return steer * 0.03f;
}

glm::vec3 Coordinator::Cohesion(iObject* currentObject)
{
	glm::vec3 totalPosition = glm::vec3(0.0f);
	float cohesionRadius = 50.0f;
	int neighbourCount = 0;
	for (int i = 0; i < vehicles.size(); i++)
	{
		if (currentObject->getFriendlyName() == vehicles.at(i)->getFriendlyName())
		{
			continue;
		}
		float distance = glm::distance(currentObject->getPositionXYZ(), vehicles.at(i)->getPositionXYZ());
		if (distance > 0.0f && distance < cohesionRadius)
		{
			totalPosition += vehicles.at(i)->getPositionXYZ();
			neighbourCount++;
		}
	}
	glm::vec3 steerForce = glm::vec3(0.0f);

	if (neighbourCount > 0)
	{
		glm::vec3 target = totalPosition / (float)neighbourCount;
		steerForce = CoSeek(currentObject, target);
	}
	return steerForce;
}

void Coordinator::Flock()
{
	for (int i = 0; i < vehicles.size(); i++)
	{
		glm::vec3 separation = Separation(vehicles.at(i));
		glm::vec3 alignment = Alignment(vehicles.at(i));
		glm::vec3 cohesion = Cohesion(vehicles.at(i));

		separation = glm::vec3(separation.x, 0.0f, separation.z);
		alignment = glm::vec3(alignment.x, 0.0f, alignment.z);
		cohesion = glm::vec3(cohesion.x, 0.0f, cohesion.z);

		separation *= 0.25f;
		alignment *= 0.13f;
		cohesion *= 0.5f;

		glm::vec3 newVelocity = vehicles.at(i)->getVelocity() + (separation + alignment + cohesion);

		vehicles.at(i)->setVelocity(newVelocity);
	}
}

void Coordinator::AddVehicle(iObject* vehicle)
{
	this->vehicles.push_back(vehicle);
	
}

void Coordinator::RemoveVehicle(iObject* vehicle)
{
	for (int i = 0; i < this->vehicles.size(); i++)
	{
		if (this->vehicles.at(i)->getFriendlyName() == vehicle->getFriendlyName())
		{
			this->vehicles.erase(this->vehicles.begin() + i);
		}
	}
}

void Coordinator::SetCoordinatorObject(iObject* coordinator)
{
	this->theCoordinator = coordinator;
}

iObject* Coordinator::GetCoordinatorObject()
{
	return theCoordinator;
}

void Coordinator::SetCommand(std::string command)
{
	this->currentCommand = command;
}

void Coordinator::SetFormation(std::string formation)
{
	if (formation == "square")
	{
		currentFormation = squareOffsets;
	}
	if (formation == "v")
	{
		currentFormation = vOffsets;
	}
	if (formation == "circle")
	{
		currentFormation = circleOffsets;
	}
	if (formation == "line")
	{
		currentFormation = lineOffsets;
	}
	if (formation == "rows")
	{
		currentFormation = rowsOffsets;
	}
}

template <class T>
T randInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
};

void Coordinator::CreateResourcePath(std::vector<Node*> thePath)
{
	if (resourcePath.size() > 0)
	{
		resourcePath.clear();
	}
	int pathLength = thePath.size();
	std::reverse(thePath.begin(), thePath.end());
	for (int i = 0; i < pathLength; i++)
	{
		//glm::vec3 point = glm::vec3(randInRange(-200, 200), 0.0f, randInRange(-200.0f, 200.0f));
		resourcePath.push_back(thePath.at(i));
	}
}

void Coordinator::CreateBasePath(std::vector<Node*> thePath)
{
	if (basePath.size() > 0)
	{
		basePath.clear();
	}
	int pathLength = thePath.size();
	std::reverse(thePath.begin(), thePath.end());
	for (int i = 0; i < pathLength; i++)
	{
		//glm::vec3 point = glm::vec3(randInRange(-200, 200), 0.0f, randInRange(-200.0f, 200.0f));
		basePath.push_back(thePath.at(i));
	}
}

Node* Coordinator::GetCurrentPathPoint()
{
	if (currentState == states::search)
	{
		return resourcePath.at(pathSection);
	}
	else
	{
		return basePath.at(pathSection);
	}
}

std::vector<Node*> Coordinator::GetCurrentPath()
{
	if (currentState == states::search)
	{
		return resourcePath;
	}
	else
	{
		return basePath;
	}
}

int Coordinator::GetCurrentPathSection()
{
	return pathSection;
}

std::string Coordinator::GetCurrentCommand()
{
	return currentCommand;
}

void Coordinator::SetGraph(Graph* graph)
{
	this->theGraph = graph;
}

void Coordinator::Idle()
{
	//don't do anything
}

void Coordinator::Deposit(float dt)
{
	theCoordinator->setVelocity(glm::vec3(0.0f));
	if (depositTime < 2.0f)
	{
		depositTime += dt;
	}
	else
	{
		depositTime = 0.0f;
		currentState = states::findBase;
	}
}

void Coordinator::Gather(float dt)
{
	theCoordinator->setVelocity(glm::vec3(0.0f));
	if (gatherTime < 7.0f)
	{
		gatherTime += dt;
	}
	else
	{
		gatherTime = 0.0f;
		if (groceryList.size() > 0)
		{
			currentState = states::findResource;
		}
		else
		{
			currentState = states::findCashier;
		}
		hasResource = true;
	}
}

void Coordinator::FindNewResource()
{
	Node* resourceNode;

	if (groceryList.size() == 0)
	{
		this->currentState = states::findCashier;
		return;
	}

	resourceNode = Dijkstra(currentNode, theGraph, groceryList);
	if (resourceNode == nullptr)
	{
		this->currentState = states::findCashier;
		return;
	}
	currentResourceNode = resourceNode;
	std::vector<glm::vec3> resourcePathPoints;
	//int numChildren = resourceNode->children.size();
	int numParents = theGraph->parentList.size();

	Node* currentNode = resourceNode;
	if (resourcePath.size() > 0)
	{
		resourcePath.clear();
	}
	while (currentNode->parent != NULL)
	{
		resourcePath.push_back(currentNode);
		currentNode = currentNode->parent;
	}
	std::reverse(resourcePath.begin(), resourcePath.end());
	if (currentResourceNode == nullptr)
	{
		currentState = states::findCashier;
	}
	else
	{
		currentState = states::search;
	}	
}

void Coordinator::FindCashier()
{
	Node* aStarRootNode;
	if (this->currentNode != nullptr)
	{
		aStarRootNode = currentNode;
	}
	else
	{
		aStarRootNode = rootNode;
	}

	if (aStarRootNode == nullptr)
	{
		return;
	}
	Node* cashierNode = nullptr;
	for (int i = 0; i < theGraph->nodes.size(); i++)
	{
		if (theGraph->nodes[i]->id == laneID)
		{
			cashierNode = theGraph->nodes[i];
		}
	}
	goalNode = AStar(aStarRootNode, theGraph, cashierNode);

	std::vector<glm::vec3> cashierPathPoints;
	Node* currentAStarNode = goalNode;
	if (cashierPath.size() > 0)
	{
		cashierPath.clear();
	}
	while (currentAStarNode->parent != NULL)
	{
		cashierPath.push_back(currentAStarNode);
		currentAStarNode = currentAStarNode->parent;
	}
	std::reverse(cashierPath.begin(), cashierPath.end());
	currentState = states::checkout;
}

void Coordinator::FindBase()
{
	Node* aStarRootNode;
	if (this->currentNode != nullptr)
	{
		aStarRootNode = currentNode;
	}
	else
	{
		aStarRootNode = rootNode;
	}
	
	if (aStarRootNode == nullptr)
	{
		return;
	}
	goalNode = AStar(aStarRootNode, theGraph, baseNode);

	std::vector<glm::vec3> basePathPoints;
	Node* currentAStarNode = goalNode;
	if (basePath.size() > 0)
	{
		basePath.clear();
	}
	while (currentAStarNode->parent != NULL)
	{
		basePath.push_back(currentAStarNode);
		currentAStarNode = currentAStarNode->parent;
	}
	std::reverse(basePath.begin(), basePath.end());
	currentState = states::returnToBase;
}

void Coordinator::update(float dt)
{
	if (this->currentState == states::findResource)
	{
		FindNewResource();
	}
	else if (this->currentState == states::findBase)
	{
		FindBase();
	}
	else if (this->currentState == states::search)
	{
		FindResource();
	}
	else if (this->currentState == states::returnToBase)
	{
		GoToBase();
	}
	else if (this->currentState == states::idle)
	{
		Idle();
	}
	else if (this->currentState == states::depositing)
	{
		Deposit(dt);
	}
	else if (this->currentState == states::gathering)
	{
		Gather(dt);
	}
	else if (this->currentState == states::findCashier)
	{
		FindCashier();
	}
	else if (this->currentState == states::checkout)
	{
		Checkout();
	}
}
