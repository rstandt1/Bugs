#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include "Actor.h"
#include "Field.h"
#include"Compiler.h"
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
		elapsed = antCount[0] = antCount[1] = antCount[2] = antCount[3] = 0;
		antTime[0] = antTime[1] = antTime[2] = antTime[3] = 0;
		for (int i = 0; i < 4; i++)
			m_comps[i] = nullptr;
	}
	~StudentWorld()	
	{
		cleanUp(); 
		for (int i = 0; i < 4; i++)
			if (m_comps[i])
			delete m_comps[i];
	}
	int getTicks() const { return elapsed; } //returns number of elapsed ticks
	void incrementAnt(int colony); //increments the ant per colony counter used in the scoreboard
	void insertAnt(int x, int y, Compiler *compiler, int colonyNum); //inserts a new ant of the given colony at the provided x, y location
	void insertPheromone(int x, int y, int colonyNum); //inserts a new pheromone of the given colony at the provided x, y location
	bool loadField(); //attempts to load the field file, program will only run as intended if true is returned
	int winning(); //returns the current leader among the different Bug files
	void moult(int x, int y); //inserts a new Adult Grasshopper at the provided x, y location
	bool enemyPresent(Actor* me, int x, int y); //returns true if an enemy of the Actor* parameter is found at the specified x, y location. Otherwise false
	bool onMyHill(int colony, int x, int y); //returns true if the Actor* parameter is found at the x, y location of the Anthill of the specified colony. Otherwise false
	bool biteSuccess(Actor* me, int x, int y); //Attempts to bite an enemy of the Actor* parameter at the given x, y location if one is found. Returns true if successfull otherwise false
	bool blocked(int x, int y); //returns true if a Pebble object found at the given x, y location. Otherwise false
	bool containsFood(int x, int y); //returns true if a Food object found at the given x, y location. Otherwise false
	int eatFood(Actor *me, int x, int y); //reduces the health of the Food object being eaten and returns the amount the Actor* parameter's health should increase by
	int pickupFood(Actor *me, int x, int y); //reduces the health of the Food object being eaten and returns the amount the Actor* parameter's burden should increase by
	bool smellPheromone(int colony, int x, int y); //returns true if a Pheromone of the specified colony is found at x, y. Otherwise false
	int dropFood(int x, int y, int amount); //creates a food object with amount points or increments an existing food object at x, y by amount. Returns amount dropped
	bool smellDanger(Actor *me, int x, int y); //Returns true if an enemy of Actor* parameter or a poison/water object found at x, y. Otherwise false
	void stunTargets(int x, int y); //Returns true if an enemy of Actor* parameter or a poison object found at x, y. Otherwise false
	void poisonTargets(int x, int y); //Poisons all Baby Grasshoppers and Ants at x, y who were not poisoned on the last turn by the same object without having moved
	virtual int init(); //initializes the Field and adds all starting objects to data structure
	virtual int move(); //simulates the movement 
	virtual void cleanUp();
private:
	int elapsed, antCount[4], antTime[4];
	Compiler* m_comps[4];
	std::list<Actor*> actors[64][64];
	Field m_field;
};

#endif // STUDENTWORLD_H_
