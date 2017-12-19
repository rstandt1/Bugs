#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

bool StudentWorld::loadField()
{
		std::string fieldFile = getFieldFilename(); //retrieves field file name
		std::string error; //used for an error message
		if (m_field.loadField(fieldFile, error) != Field::LoadResult::load_success)
		{
			setError(fieldFile + " " + error); //lets user know what went wrong and returns if load unsuccessful
			return false; // something bad happened!
		}
		return true; //everything went as planned
}

int StudentWorld::init()
{
	if (!loadField())
		return GWSTATUS_LEVEL_ERROR; //returns error if field unable to load file
	vector<string> fileNames = getFilenamesOfAntPrograms(); //retrieves all bug files
	for (int i = 0; i < 64; i++) //for every index of the 2D array of linked lists
	{
		for (int j = 0; j < 64; j++)
		{
			if (m_field.getContentsOf(i, j) == Field::FieldItem::rock)
				actors[j][i].push_back(new Pebble(this, i, j)); //inserts a Pebble at the proper place according to the field file loaded
			if (m_field.getContentsOf(i, j) == Field::FieldItem::grasshopper)
				actors[j][i].push_back(new BabyGrasshopper(this, i, j)); //inserts a Baby Grasshopper at the proper place according to the field file loaded
			if (m_field.getContentsOf(i, j) == Field::FieldItem::water) //inserts a Pool of Water at the proper place according to the field file loaded
				actors[j][i].push_front(new WaterPool(this, i, j)); //pushes to front in order to make stunning occupants more efficienct
			if (m_field.getContentsOf(i, j) == Field::FieldItem::poison)//inserts a Poison object at the proper place according to the field file loaded
				actors[j][i].push_front(new Poison(this, i, j)); //pushes to front in order to make poisoning occupants more efficienct
			if (m_field.getContentsOf(i, j) == Field::FieldItem::food)
				actors[j][i].push_front(new Food(this, 6000, i, j)); //inserts a Food object at the proper place according to the field file loaded
			if (m_field.getContentsOf(i, j) == Field::FieldItem::anthill0 && fileNames.size() > 0) //if there is an anthill object and corresponding bug file to be loaded
			{
				m_comps[0] = new Compiler();
				string error;
				// compile the source file… If the compile function returns  
				// false, there was a syntax error during compilation!  
				if (!m_comps[0]->compile(fileNames[0], error))
				{     // entrant 0’s source code had a syntax error!    
					  // send this error to our framework to warn the user.     
					  // do it JUST like this!   
					setError(fileNames[0] + " " + error); 
					  // return an error to tell our simulation framework     
					  // that something went wrong, and it’ll inform the user   
					return GWSTATUS_LEVEL_ERROR;  
				} 
				actors[j][i].push_front(new AntHill(this, i, j, m_comps[0], 0)); //pushes successfully allocated AntHill onto the data structure
			}
			if (m_field.getContentsOf(i, j) == Field::FieldItem::anthill1 && fileNames.size() > 1) //if there is an anthill object and corresponding bug file to be loaded
			{
				m_comps[1] = new Compiler();
				string error;
				// compile the source file… If the compile function returns  
				// false, there was a syntax error during compilation!  
				if (!m_comps[1]->compile(fileNames[1], error))
				{     // entrant 0’s source code had a syntax error!    
					  // send this error to our framework to warn the user.     
					  // do it JUST like this!   
					setError(fileNames[1] + " " + error);
					// return an error to tell our simulation framework     
					// that something went wrong, and it’ll inform the user   
					return GWSTATUS_LEVEL_ERROR;
				}
				actors[j][i].push_front(new AntHill(this, i, j, m_comps[1], 1)); //pushes successfully allocated AntHill onto the data structure
			}
			if (m_field.getContentsOf(i, j) == Field::FieldItem::anthill2 && fileNames.size() > 2) //if there is an anthill object and corresponding bug file to be loaded
			{
				m_comps[2] = new Compiler();
				string error;
				// compile the source file… If the compile function returns  
				// false, there was a syntax error during compilation!  
				if (!m_comps[2]->compile(fileNames[2], error))
				{     // entrant 0’s source code had a syntax error!    
					  // send this error to our framework to warn the user.     
					  // do it JUST like this!   
					setError(fileNames[2] + " " + error);
					// return an error to tell our simulation framework     
					// that something went wrong, and it’ll inform the user   
					return GWSTATUS_LEVEL_ERROR;
				}
				actors[j][i].push_front(new AntHill(this, i, j, m_comps[2], 2)); //pushes successfully allocated AntHill onto the data structure
			}
			if (m_field.getContentsOf(i, j) == Field::FieldItem::anthill3 && fileNames.size() > 3) //if there is an anthill object and corresponding bug file to be loaded
			{
				m_comps[3] = new Compiler();
				string error;
				// compile the source file… If the compile function returns  
				// false, there was a syntax error during compilation!  
				if (!m_comps[3]->compile(fileNames[3], error))
				{     // entrant 0’s source code had a syntax error!    
					  // send this error to our framework to warn the user.     
					  // do it JUST like this!   
					setError(fileNames[3] + " " + error);
					// return an error to tell our simulation framework     
					// that something went wrong, and it’ll inform the user   
					return GWSTATUS_LEVEL_ERROR;
				}
				actors[j][i].push_front(new AntHill(this, i, j, m_comps[3], 3)); //pushes successfully allocated AntHill onto the data structure
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME; //ready for the move function to be called
}

int StudentWorld::move()
{
	list<Actor*>::iterator master;
	for (int i = 0; i < 64; i++) //reaches every index of data structure
	{
		for (int j = 0; j < 64; j++)
		{
			if (!actors[i][j].empty()) //if the linked list is not empty
			{
				master = actors[i][j].begin();
				while (master != actors[i][j].end())
				{
					if ((*master)->moveCount() <= elapsed) //if the actor has not moved on this tick
					{
						int priorX = (*master)->getX();
						int priorY = (*master)->getY();
						(*master)->doSomething(); //uses polymorphism to call most derived version of doSomething
						if ((*master)->isDead() && !(dynamic_cast<Insect*>(*master))) //if the actor died and does not release food upon death
						{
							delete *master;
							master = actors[i][j].erase(master); //deallocates and removes from linked list
						}
						else if ((*master)->isDead()) //if the actor died and must release food
						{
							list<Actor*>::iterator temp;
							temp = actors[i][j].begin();
							while (temp != actors[i][j].end() && dynamic_cast<Food*>(*temp)) //iterates until either the end of the list or a food object is reached
								temp++;
							if (temp == actors[i][j].end()) //if there was no food object at this location
							{
								delete *master; 
								*master = new Food(this, 100, j, i); //deallocate the dead actor and replace it with a food object
							}
							else
							{
								(*temp)->appendHealth(100); //if a food object was found then append its health
								delete *master;
								master = actors[i][j].erase(master); //deallocate and remove the dead actor from the linked list
							}
						}
						else if (priorX != (*master)->getX() || priorY != (*master)->getY()) //if the actor moved during its call to doSomething()
						{
							actors[(*master)->getY()][(*master)->getX()].push_back(*master); //add the actor to its new location in the data structure and remove
							master = actors[i][j].erase(master);							//but do not deallocate from the old position
						}
						else
							master++; //move to next element
					}
					else
						master++; //move to next element
				}
			}
		}
	}
	ostringstream oss;
	vector<string> fileNames = getFilenamesOfAntPrograms();
	oss << "Ticks:" << setw(5) << (2000 - elapsed) << " -  "; //formats the tick counter
	oss.fill('0'); //set the fill for antCount[i]
	for (int i = 0; i < fileNames.size(); i++) //inserts the bug file name and ant counter into the object oss. If that bug is winning add the '*'
	{
		if (m_comps[i])
		{
			if (winning() == i)
				oss << m_comps[i]->getColonyName() << "*: " << setw(2) << antCount[i] << "  ";
			else
				oss << m_comps[i]->getColonyName() << ": " << setw(2) << antCount[i] << "  ";
		}
	}
	setGameStatText(oss.str()); //sets the status bar equal to the string equivalent of oss
	elapsed++; //increase the tick count
	if (elapsed < 2000)
		return GWSTATUS_CONTINUE_GAME; //continue simulating if less than 2000 ticks have occurred
	else if (winning() == -1)
		return GWSTATUS_NO_WINNER; //if the game is over and there was no winner return the no winner status
	else
	{
		if (m_comps[winning()])
			setWinner(m_comps[winning()]->getColonyName()); //if the game ended and there was a winner then set the winner equal to the winning bug file name
		return GWSTATUS_PLAYER_WON; //return that a player won
	}
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
}

void StudentWorld::cleanUp()
{
	list<Actor*>::iterator master;
	for (int i = 0; i < 64; i++) //iterate through the entire data structure and deallocate each object then remove it from the 
	{							//corresponding linked list
		for (int j = 0; j < 64; j++)
		{
			while (!actors[i][j].empty())
			{
				master = actors[i][j].begin();
				delete *master;
				actors[i][j].erase(master);
			}
		}
	}
	//compiler objects deallocated in the destructor
}

void StudentWorld::stunTargets(int x, int y)
{
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end()) //iterates through the entire linked list
	{
		if (dynamic_cast<BabyGrasshopper*>(*temp) || dynamic_cast<Ant*>(*temp)) //if the actor is stunnable 
			if (!dynamic_cast<Insect*>(*temp)->isStunned())						//and has not yet been stunned
				dynamic_cast<Insect*>(*temp)->stun();							//stun the actor
		temp++;
	}
}

bool StudentWorld::smellDanger(Actor *me, int x, int y)
{
	if (enemyPresent(me, x, y)) //if an enemy is present return true
		return true;
	if (!actors[y][x].empty()) //if there are objects to iterate through
	{
		std::list<Actor*>::iterator temp;
		temp = actors[y][x].begin();
		if (dynamic_cast<Poison*>(*temp) || dynamic_cast<WaterPool*>(*temp)) //if the linked list in question contains Poison or Water return true
			return true;
	}
	return false;
}

bool StudentWorld::smellPheromone(int colony, int x, int y)
{
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end()) //iterates through entire linked list
	{
		if (dynamic_cast<Pheromone*>(*temp)) //if the actor pointer points to a pheromone object
			if (dynamic_cast<Pheromone*>(*temp)->getColony() == colony) //if said pheromone object has the same colony as the any looking for a pheromone
				return true;
		temp++;
	}
	return false;
}

int StudentWorld::dropFood(int x, int y, int amount)
{
	if (!containsFood(x, y)) //if no food object is at x, y
		actors[y][x].push_back(new Food(this, amount, x, y)); //add the new food object to the data structure
	else //if there is a food object at x, y
	{
		std::list<Actor*>::iterator temp;
		temp = actors[y][x].begin();
		while (temp != actors[y][x].end() && !dynamic_cast<Food*>(*temp)) //iterate until *temp points at the food object
			temp++;
		(*temp)->appendHealth(amount); //increase the quantity of food by amount
	}
	return amount;
}

void StudentWorld::poisonTargets(int x, int y)
{
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end()) //iterates through entire linked list
	{
		if (dynamic_cast<BabyGrasshopper*>(*temp) || dynamic_cast<Ant*>(*temp)) //if the actor being pointed at is poisonable
		{
			if (!dynamic_cast<Insect*>(*temp)->isPoisoned()) //if *temp hasn't been poisoned
			{
				dynamic_cast<Insect*>(*temp)->poison(); //poison the actor
			}
			else
				temp++;
		}
		else
			temp++;
	}
}

int StudentWorld::pickupFood(Actor *me, int x, int y)
{
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end() && !dynamic_cast<Food*>(*temp)) //iterates until end of list has been reached or *temp points at a food object
		temp++;
	if (temp == actors[y][x].end())
		return 0; //return 0 if the end of the list was reached
	if (dynamic_cast<Ant*>(me)->getBurden() <= 1400) //if *temp points at food and the ant pointed to by "me" is more than one pickup from max capacity
	{
		if ((*temp)->getHealth() > 400) //if the food object has more than 400 energy points remaining
		{
			(*temp)->appendHealth(-400); //pickup 400 food and appropriately decrement the food object's health
			return 400;
		}
		else
		{
			int essen = (*temp)->getHealth(); //pickup the rest of the food and return this value 
			delete *temp;						//after deallocating and removing the empty food object
			actors[y][x].erase(temp);
			return essen;
		}
	}
	else if (dynamic_cast<Ant*>(me)->getBurden() > 1400) //if the ant is within one pickup of max capacity
	{
		if ((*temp)->getHealth() > (1800 - dynamic_cast<Ant*>(me)->getBurden())) //if there will be remaining energy in the food object once the ant's burden's capacity is reached
		{
			(*temp)->appendHealth(-1 * (1800 - dynamic_cast<Ant*>(me)->getBurden()));
			return (1800 - dynamic_cast<Ant*>(me)->getBurden()); //decrement the food's health by and return the quantity to be picked up
		}
		else
		{
			int essen = (*temp)->getHealth(); //pickup the rest of the food and return this value 
			delete *temp;						//after deallocating and removing the empty food object
			actors[y][x].erase(temp);
			return essen;
		}
	}
	return 0;
}

int StudentWorld::eatFood(Actor *me, int x, int y)
{
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end() && !dynamic_cast<Food*>(*temp))
		temp++;
	if (dynamic_cast<Grasshopper*>(me)) //if the actor eating is a Grasshopper
	{
		if ((*temp)->getHealth() > 200) //if the food object will still have energy after being fed on
		{
			(*temp)->appendHealth(-200); //append the energy of the food item and return amount eaten
			return 200;
		}
		else //if the food item is going to be emptied of energy
		{
			int essen = (*temp)->getHealth(); //store the amount of energy in the food object, this quantity will be returned
			delete *temp;
			actors[y][x].erase(temp); //deallocates and removes the empty food object
			return essen;
		}
	}
	if (dynamic_cast<AntHill*>(me)) //if the actor eating is an Anthill
	{
		if ((*temp)->getHealth() > 10000) //if the food object will still have energy after being fed on
		{
			(*temp)->appendHealth(-10000); //append the energy of the food item and return amount eaten
			return 10000;
		}
		else //if the food item is going to be emptied of energy
		{
			int essen = (*temp)->getHealth(); //store the amount of energy in the food object, this quantity will be returned
			delete *temp;
			actors[y][x].erase(temp); //deallocates and removes the empty food object
			return essen;
		}
	}
	return 0;
}

bool StudentWorld::containsFood(int x, int y)
{
	if (actors[y][x].size() == 1) //if the only actor at x, y is the one responsible for the function call return false
		return false;
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end() && !dynamic_cast<Food*>(*temp)) //iterates until a food object is found or the end of the list is reached
		temp++;
	if (temp == actors[y][x].end()) //if end of list reached return false
		return false;
	return true;
}

bool StudentWorld::blocked(int x, int y)
{
	if (actors[y][x].size() == 0)
		return false;
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	if (dynamic_cast<Pebble*>(*temp)) //if a Pebble object is located at x, y then return true
		return true;
	return false;
}

bool StudentWorld::biteSuccess(Actor* me, int x, int y)
{
	std::vector<Actor*> targets;
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end())
	{
		Insect *hunted = dynamic_cast<Insect*>(*temp);
		if (hunted != nullptr && !hunted->isDead() && hunted != me) //if the insect pointer is a viable target
		{
			if (dynamic_cast<Ant*>(me) && dynamic_cast<Ant*>(*temp)) //if both the biter and actor being bitten are ants
			{
				if (dynamic_cast<Ant*>(me)->getColony() != dynamic_cast<Ant*>(*temp)->getColony()) //if both ants do not come from the same colony
					targets.push_back(*temp); //add insect to the target vector
			}
			else
				targets.push_back(*temp); //insect is a viable target and gets added to the vector
		}
		temp++;
	}
	if (targets.empty()) //if no viable targets return false
		return false;
	Insect *aquired = dynamic_cast<Insect*>(targets[randInt(0, targets.size() - 1)]); //pick a random target out of the target vector
	AdultGrasshopper *it = dynamic_cast<AdultGrasshopper*>(aquired);
	if (it == nullptr) //if the aquired target is not an Adult Grasshopper
	{
		 if (dynamic_cast<Ant*>(aquired)) //if target is an Ant
			 dynamic_cast<Ant*>(aquired)->setBitten(); //set to bitten at current square
		if (dynamic_cast<AdultGrasshopper*>(me)) //if biter is an Adult Grasshopper then do respective damage
			aquired->appendHealth(-50);
		else
			aquired->appendHealth(-15); //if biter is an Ant do respective damage
		if (aquired->getHealth() <= 0)
		{
			aquired->setDead(); //set to dead if killed by bite
		}
		return true;
	}
	else //if aquired target an Adult Grasshopper
	{
		if (dynamic_cast<AdultGrasshopper*>(me)) //if biter is an Adult Grasshopper do respective damage
			aquired->appendHealth(-50);
		else
			aquired->appendHealth(-15); //if biter is an Ant do respective damage
		if (aquired->getHealth() <= 0) //set to dead if killed by bite
		{
			aquired->setDead();
			return true;
		}
		if (randInt(0, 1) == 0) //if Adult Grasshopper survived the bite then set fifty percent chance it will bite back
			biteSuccess(aquired, x, y);
		return true;
	}
}

bool StudentWorld::onMyHill(int colony, int x, int y)
{
	if (actors[y][x].size() == 1)
		return false;
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end()) //iterates through list until end
	{
		if (dynamic_cast<AntHill*>(*temp)) //if the iterator points at an Anthill
			if (dynamic_cast<AntHill*>(*temp)->getColony() == colony) //if said Anthill has the same colony number as the parameter colony return true
				return true;
		temp++;
	}
	return false;
}

bool StudentWorld::enemyPresent(Actor* me, int x, int y)
{
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end()) //iterates through list until end
	{
		Insect *hunted = dynamic_cast<Insect*>(*temp);
		if (hunted != nullptr && hunted != me && !hunted->isDead()) //if there is a living insect at this location
		{
			if (dynamic_cast<Ant*>(me) && dynamic_cast<Ant*>(*temp)) //if both the parameter "me" and the potential enemy are ants
			{
				if (dynamic_cast<Ant*>(me)->getColony() != dynamic_cast<Ant*>(*temp)->getColony()) //if the ants do not share a colony mark as an enemy
					return true;
			}
			else
				return true; //the insect is a type of Grasshopper and therefore automatically an enemy
		}
		temp++;
	}
	return false;
}

void StudentWorld::moult(int x, int y)
{
	actors[y][x].push_back(new AdultGrasshopper(this, x, y)); //push an Adult Grasshopper to the back of the linked list at location x, y
	std::list<Actor*>::iterator temp = actors[y][x].end();
	--temp; //sets the iterator equal to our newly pushed Adult Grasshopper
	(*temp)->setMoveCount(elapsed - 1); //ensures that the new Grasshopper will not move out of turn
}

int StudentWorld::winning()
{
	if (antCount[0] <= 5 && antCount[1] <= 5 && antCount[2] <= 5 && antCount[3] <= 5) //No winner during birth of initial ants
		return -1;
	//following for loop pushes to the back of vector max
	//if the colony being examined has more ants than the previous max
	//if the amount of ants equals the previous max then sets the first object
	//in the vector equal to that index
	std::vector<int> max;
	max.push_back(0);
	for (int i = 1; i < getFilenamesOfAntPrograms().size(); i++)
	{
		if (antCount[i] > antCount[max[max.size() - 1]])
			max.push_back(i);
		else if (antCount[i] == antCount[max[max.size() - 1]])
			max[0] = i;
	}
	if (max.size() == 1) //if the first object was the max return its index
		return max[0];
	if (antCount[max[0]] < antCount[max[max.size() - 1]]) //if there are multiple items but the most recently pushed item is greater than the 0th item
		return max[max.size() - 1];
	//following for loop pushes to the back of vector min
	//if the colony being examined reached its number of ants quicker than the previous max's number of ticks
	//if the amount of ticks equals the previous num ticks then sets the first object
	//in the vector equal to that index
	std::vector<int> min;
	min.push_back(max[0]);
	for (int i = 1; i < max.size(); i++)
	{
		if (antTime[i] < antTime[min[min.size() - 1]])
			min.push_back(max[i]);
		else if (antTime[max[i]] == antTime[min[min.size() - 1]])
			min[0] = i;
	}
	if (min.size() == 1) //if the first object was the min return its index
		return min[min.size() - 1];
	if (antTime[min[0]] > antTime[min[min.size() - 1]]) //if there are multiple items but the most recently pushed item is greater than the 0th item
		return min[min.size() - 1];
	return -1;
}

void StudentWorld::insertPheromone(int x, int y, int colonyNum)
{
	std::list<Actor*>::iterator temp;
	temp = actors[y][x].begin();
	while (temp != actors[y][x].end()) //iterates through list to end
	{		
		if (dynamic_cast<Pheromone*>(*temp)) //if iterator points to Pheromone Object
			if (dynamic_cast<Pheromone*>(*temp)->getColony() == colonyNum) //if Pheromone object has the same Colony Number as parameter colonyNum
			{
				if ((*temp)->getHealth() > 768 - 256) //if Pheromone is within one filling of full strength
				{
					(*temp)->appendHealth(768 - (*temp)->getHealth()); //fill Pheromone to full strength 
					return;
				}
				else
				{
					(*temp)->appendHealth(256); //append health by maximum amount
					return;
				}
			}
		temp++;
	}
	if (temp == actors[y][x].end()) //if end of list reached and no Pheromone found
		actors[y][x].push_back(new Pheromone(this, x, y, colonyNum)); //create new Pheromone
	return;
}

void StudentWorld::insertAnt(int x, int y, Compiler *compiler, int colonyNum)
{
	actors[y][x].push_back(new Ant(this, x, y, compiler, colonyNum)); //push an new Ant of given colony and compiler to the back of the linked list at location x, y
	std::list<Actor*>::iterator temp = actors[y][x].end();
	--temp; //sets the iterator equal to our newly pushed Ant
	(*temp)->setMoveCount(elapsed - 1); //ensures that the new Ant will not move out of turn
}

void StudentWorld::incrementAnt(int colony)
{
	++antCount[colony]; //increment number of ants and time reached for the respected colony
	antTime[colony] = getTicks();
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
