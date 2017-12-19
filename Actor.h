#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"

 // Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
		: GraphObject(imageID, startX, startY, dir, size, depth)
	{
		m_world = world;
		moves = 0;
	}
	virtual ~Actor() {} //base class need virtual destructor
	virtual void doSomething() = 0; //makes Actor an Abstract Class
	//accessor and mutator functions
	StudentWorld* getStudentWorld() const { return m_world; }
	int moveCount()	const			{ return moves; }
	void setMoveCount(int num)		{ moves += num; }
	virtual bool isDead() const { return false; }
	virtual void appendHealth(int change) {}
	virtual int getHealth() const { return 0; }
private:
	StudentWorld *m_world;
	int moves;
};

class Pebble : public Actor
{
public:
	Pebble(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_ROCK, startX, startY, right, .25, 1)
	{}
	virtual void doSomething() { return; } //pebbles do nothing
};

class WaterPool : public Actor
{
public:
	WaterPool(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_WATER_POOL, startX, startY, right, .25, 2)
	{}
	virtual void doSomething(); 
};

class Poison : public Actor
{
public:
	Poison(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_POISON, startX, startY, right, .25, 2)
	{}
	virtual void doSomething();
};

class EnergySource : public Actor
{
public:
	EnergySource(StudentWorld *world, int health, int imageID, int startX, int startY, Direction dir = right, double size = 0.25, unsigned int depth = 0)
		: Actor(world, imageID, startX, startY, dir, 0.25, depth)
	{
		m_health = health;
		m_wait = 0;
		m_dead = false;
	}
	virtual ~EnergySource() {}
	virtual int getHealth() const		{ return m_health; }
	int getWait()	const				{ return m_wait;  }
	virtual void appendHealth(int change)		{ m_health += change; }
	void appendWait(int change)			{ m_wait += change; }
	void setDead()						{ m_dead = true; }
	virtual bool isDead()	const		{ return m_dead; }
	virtual void doSomething() = 0; //makes EnergySource an Abstract Class
private:
	int m_health, m_wait;
	bool m_dead;
};

class Insect : public EnergySource
{
public:
	Insect(StudentWorld *world, int health, int imageID, int startX, int startY, Direction dir = right, double size = 0.25, unsigned int depth = 0)
		: EnergySource(world, health, imageID, startX, startY, right, 0.25, depth)
	{
		m_stun = false;
		m_poison = false;
	}
	virtual bool isStunned() const { return m_stun; }
	void unstun() { m_stun = false; }
	void stun()
	{
		appendWait(2); //if stunned append the number of turns waiting by two and mark as stunned
		m_stun = true;
	}
	bool isPoisoned() const { return m_poison; }
	void unpoison() { m_poison = false; }
	void poison()
	{
		appendHealth(-150); //if poisoned do damage, mark as poisoned and potentially set to dead
		m_poison = true;
		if (getHealth() <= 0)
			setDead();
	}
	void doSomething() = 0; //makes Insect an abstract class
private:
	bool m_stun;
	bool m_poison;
};

class Ant : public Insect
{
public:
	Ant(StudentWorld *world, int startX, int startY, Compiler *compiler, int colonyNum)
		: Insect(world, 1500, colonyNum, startX, startY, right, 0.25, 1)
	{ //initialize instance variables as dictated by the spec
		m_burden = lrn = ic = 0;
		m_compiler = compiler;
		wasBit = wasBlocked = done = false;
		int num = randInt(0, 3);
		if (num == 0)
			setDirection(up);
		else if (num == 1)
			setDirection(down);
		else if (num == 2)
			setDirection(right);
		else
			setDirection(left);
		m_colony = colonyNum;
	}
	int getBurden() const { return m_burden; } //used to track food carried
	void setBitten() { wasBit = true; }
	void appendBurden(int amount) { m_burden += amount; } //used to change food carried
	int getColony() const { return m_colony; }
	virtual void doSomething();
private:
	int m_burden, lrn, ic, m_colony;
	bool wasBit, wasBlocked, done;
	Compiler *m_compiler;
	void eat()
	{
		if (getBurden() <= 100) //eats all food being carried
		{
			appendHealth(getBurden());
			appendBurden(-1 * getBurden());
		}
		else
		{
			appendHealth(100); //eats the maximum amount from the food being carried
			appendBurden(-100);
		}
	}
	void interpret(const Compiler::Command& cmd);
	bool conditionTriggered(const Compiler::Command& cmd);
	Compiler* getCompiler() const { return m_compiler; }

};

class AntHill : public EnergySource
{
public:
	AntHill(StudentWorld *world, int startX, int startY, Compiler* compile, int num)
		: EnergySource(world,  8999, IID_ANT_HILL, startX, startY, right, 0.25, 2)
	{
		//initialize instance variables according to the spec
		m_compiler = compile;
		if (num == 0)
			m_colony = IID_ANT_TYPE0;
		else if (num == 1)
			m_colony = IID_ANT_TYPE1;
		else if (num == 2)
			m_colony = IID_ANT_TYPE2;
		else
			m_colony = IID_ANT_TYPE3;
	}
	//accessor methods
	Compiler* getCompiler() const { return m_compiler; }
	int getColony() const { return m_colony; }
	virtual void doSomething();
private:
	Compiler* m_compiler;
	int m_colony;
};

class Pheromone : public EnergySource
{
public:
	Pheromone(StudentWorld *world, int startX, int startY, int colonyNum)
		: EnergySource(world, 256, colonyNum + 11, startX, startY, right, 0.25, 2)
	{
		m_colony = colonyNum;
	}
	virtual void doSomething()
	{
		setMoveCount(1); //reduce Pheromone health and increment number of moves
		appendHealth(-1);
		if (getHealth() <= 0) //set to dead if health dips to or below 0
		{
			setDead();
			return;
		}
	}
	int getColony() const { return m_colony; }
private:
	int m_colony;
};

class Food : public EnergySource
{
public:
	Food(StudentWorld* world, int health, int startX, int startY)
		: EnergySource(world, health, IID_FOOD, startX, startY, right, 0.25, 2)
	{}
	virtual void doSomething() { return; } //food does nothing
};

class Grasshopper : public Insect
{
public:
	Grasshopper(StudentWorld *world, int energy, int ID, int startX, int startY)
		: Insect(world, energy, ID, startX, startY, right, 1.0, 1)
	{
		setDir(); //randomly set the direction and distance
	}
	virtual ~Grasshopper() {}
	//accessor and mutator methods
	virtual void doSomething() = 0; //makes Grasshopper abstract
	void setDist(int dist) { m_dist += dist; }
	int getDist()	const { return m_dist; }

protected:
	void setDir();

private:
	int m_dist;	
};

class BabyGrasshopper : public Grasshopper
{
public:
	BabyGrasshopper(StudentWorld *world, int startX, int startY);
	virtual ~BabyGrasshopper() {}
	virtual void doSomething();
};

class AdultGrasshopper : public Grasshopper
{
public:
	AdultGrasshopper(StudentWorld *world, int startX, int startY);
	virtual ~AdultGrasshopper() {}
	virtual void doSomething();
	virtual bool isStunned() const { return true; }
	virtual bool isPoisoned() const { return true; }
};

#endif //ACTOR_H_
