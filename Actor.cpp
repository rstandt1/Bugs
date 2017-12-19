#include "Actor.h"
#include "StudentWorld.h"
#include<iostream>
#include<cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void WaterPool::doSomething()
{
	getStudentWorld()->stunTargets(getX(), getY()); //stuns all possible targets within the StudentWorld at this location
}

void Poison::doSomething()
{
	getStudentWorld()->poisonTargets(getX(), getY()); //poisons all possible targets within the Student World at this location
}

BabyGrasshopper::BabyGrasshopper(StudentWorld *world, int startX, int startY)
	: Grasshopper(world, 500, IID_BABY_GRASSHOPPER, startX, startY)
{}

AdultGrasshopper::AdultGrasshopper(StudentWorld *world, int startX, int startY)
	: Grasshopper(world, 1600, IID_ADULT_GRASSHOPPER, startX, startY)
{}

void Grasshopper::setDir()
{
	int num = randInt(0, 3); //randomly set the direction
	if (num == 0)
		setDirection(up);
	else if (num == 1)
		setDirection(down);
	else if (num == 2)
		setDirection(right);
	else
		setDirection(left);
	m_dist = randInt(2, 10); //randomly set the distance
	return;
}

void BabyGrasshopper::doSomething()
{
	setMoveCount(1); //increment movecount and decrement count
	appendHealth(-1);
	if (getHealth() <= 0) //set to dead if health equal to or below zero
	{
		setDead();
		return;
	}
	if (getWait() > 0) //sleep if necessary
	{
		appendWait(-1); //reduce time sleeping by one
		return;
	}
	if (getHealth() >= 1600)
	{
		getStudentWorld()->moult(getX(), getY()); //if enough health, die and instead have the StudentWorld create an Adult Grasshopper
		setDead();
		return;
	}
	if (getStudentWorld()->containsFood(getX(), getY()))
	{
		appendHealth(getStudentWorld()->eatFood(this, getX(), getY())); //eat if possible
		if (randInt(0, 1) == 0)
		{
			appendWait(2); //if the Baby Grasshopper ate theres a 50% chance it sleeps
			return;
		}
	}
	if (getDist() == 0)
		setDir(); //if distance is 0 then give a new direction and distance (may be old direction)
	if (getDirection() == up)
	{
		//move if not blocked and set that it has not been stunned or poisoned at this new location
		//if blocked set distance to 0
		if (!getStudentWorld()->blocked(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1); 
			unstun();
			unpoison();
		}
		else
			setDist(-1 * getDist());
	}
	else if (getDirection() == down)
	{
		if (!getStudentWorld()->blocked(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			unstun();
			unpoison();
		}
		else
			setDist(- 1 * getDist());
	}
	else if (getDirection() == right)
	{
		if (!getStudentWorld()->blocked(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			unstun();
			unpoison();
		}
		else
			setDist(-1 * getDist());
	}
	else if (getDirection() == left)
	{
		if (!getStudentWorld()->blocked(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			unstun();
			unpoison();
		}
		else
			setDist(-1 * getDist());
	}
	if (getDist() != 0) //if distance is not equal to zero decrement by one
		setDist(-1);
	appendWait(2);	 //put to sleep for 2 following ticks
}

void AdultGrasshopper::doSomething()
{
	setMoveCount(1); //increment movecount and decrement count
	appendHealth(-1);
	if (getHealth() <= 0)  //set to dead if health equal to or below zero
	{
		setDead();
		return;
	}
	if (getWait() > 0) //if sleeping decrement time to sleep by one and return
	{
		appendWait(-1);
		return;
	}
	if (randInt(0, 2) == 2) // 1/2 chance a bite will be attempted
	{
		if (getStudentWorld()->biteSuccess(this, getX(), getY()))
		{
			appendWait(2); //if the bite was successful the Grasshopper is done for the tick
			return;
		}
	}
	if (randInt(0, 9) == 0) //10% chance it jumps within radius of 10
	{
		std::vector<int> randX, randY;
		for (int degrees = 0; degrees < 360; degrees += 5)
		{
			for (int rad = 1; rad <= 10; ++rad)
			{
				int tempX = int(getX() + rad * cos(degrees * 4.0 * atan(1) / 180.0));
				int tempY = int(getY() + rad * sin(degrees * 4.0 * atan(1) / 180.0));
				if ((tempX > 0 && tempX < 63) && (tempY > 0 && tempY < 63) && !getStudentWorld()->blocked(tempX, tempY))
				{
					randX.push_back(tempX);
					randY.push_back(tempY);
				}
			}
		}
		while (!randX.empty() && !randY.empty())
		{
			int index = randInt(0, randX.size() - 1);
			moveTo(randX[index], randY[index]);
			appendWait(2);
			return;
		}
	}
	if (getStudentWorld()->containsFood(getX(), getY())) //if food at location
	{
		appendHealth(getStudentWorld()->eatFood(this, getX(), getY()));
		if (randInt(0, 1) == 0)
		{
			appendWait(2); //50% chance Grasshopper goes to sleep after eating
			return;
		}
	}
	if (getDist() == 0)
		setDir(); //if distance is 0 then give a new direction and distance (may be old direction)
	if (getDirection() == up)
	{
		//move if not blocked and set that it has not been stunned or poisoned at this new location
		//if blocked set distance to 0
		if (!getStudentWorld()->blocked(getX(), getY() + 1))
			moveTo(getX(), getY() + 1);
		else
			setDist(-1 * getDist());
	}
	else if (getDirection() == down)
	{
		if (!getStudentWorld()->blocked(getX(), getY() - 1))
			moveTo(getX(), getY() - 1);
		else
			setDist(-1 * getDist());
	}
	else if (getDirection() == right)
	{
		if (!getStudentWorld()->blocked(getX() + 1, getY()))
			moveTo(getX() + 1, getY());
		else
			setDist(-1 * getDist());
	}
	else if (getDirection() == left)
	{
		if (!getStudentWorld()->blocked(getX() - 1, getY()))
			moveTo(getX() - 1, getY());
		else
			setDist(-1 * getDist());
	}
	if (getDist() != 0) //if distance > 0 decrement by one
		setDist(-1);
	appendWait(2); //make sleep for 2 following ticks
}

void AntHill::doSomething()
{
	setMoveCount(1);
	appendHealth(-1);
	if (getHealth() <= 0) //if 0 or negative health then set to dead
	{
		setDead();
		return;
	}
	if (getStudentWorld()->containsFood(getX(), getY()))
	{
		appendHealth(getStudentWorld()->eatFood(this, getX(), getY())); //if food then eat and return
		return;
	}
	if (getHealth() >= 2000) //if health >= 2000 then birth a new ant and decrement health accordingly
	{
		appendHealth(-1 * 1500);
		getStudentWorld()->insertAnt(getX(), getY(), getCompiler(), getColony());
		getStudentWorld()->incrementAnt(getColony());
	}
}

void Ant::doSomething()
{
	setMoveCount(1);
	appendHealth(-1);
	if (getHealth() <= 0)
	{
		setDead();
		return;
	}
	if (getWait() > 0)
	{
		appendWait(-1);
		return;
	}
	Compiler::Command cmd;
	done = false;
	for (int i = 0; i < 10; i++)
	{
		if (!getCompiler()->getCommand(ic, cmd))
		{
			setDead();
			return;
		}
		interpret(cmd);	//execture instruction gotten from Compiler Command cmd	
		if (done) //if command execution changed simulation state then end move
			return;
	}
}

bool Ant::conditionTriggered(const Compiler::Command& cmd)
{
	switch (stoi(cmd.operand1))
	{
	
	case Compiler::Condition::last_random_number_was_zero:
		if (lrn == 0)
			return true;
		return false;
	case Compiler::Condition::i_am_carrying_food:
		if (getBurden() > 0)
			return true;
		return false;
	case Compiler::Condition::i_am_hungry:
		if (getHealth() <= 25)
			return true;
		return false;
	case Compiler::Condition::i_am_standing_with_an_enemy:
		if (getStudentWorld()->enemyPresent(this, getX(), getY()))
			return true;
		return false;
	case Compiler::Condition::i_am_standing_on_food:
		if (getStudentWorld()->containsFood(getX(), getY()))
			return true;
		return false;
	case Compiler::Condition::i_am_standing_on_my_anthill:
		if (getStudentWorld()->onMyHill(getColony(), getX(), getY()))
			return true;
		return false;
	case Compiler::Condition::i_smell_pheromone_in_front_of_me:
		if (getDirection() == up)
			if (getStudentWorld()->smellPheromone(getColony(), getX(), getY() + 1))
				return true;
		if (getDirection() == right)
			if (getStudentWorld()->smellPheromone(getColony(), getX() + 1, getY()))
				return true;
		if (getDirection() == left)
			if (getStudentWorld()->smellPheromone(getColony(), getX() - 1, getY()))
				return true;
		if (getDirection() == down)
			if (getStudentWorld()->smellPheromone(getColony(), getX(), getY() - 1))
				return true;
		return false;
	case Compiler::Condition::i_smell_danger_in_front_of_me:
		if (getDirection() == up)
			if (getStudentWorld()->smellDanger(this, getX(), getY() + 1))
				return true;
		if (getDirection() == right)
			if (getStudentWorld()->smellDanger(this, getX() + 1, getY()))
				return true;
		if (getDirection() == left)
			if (getStudentWorld()->smellDanger(this, getX() - 1, getY()))
				return true;
		if (getDirection() == down)
			if (getStudentWorld()->smellDanger(this, getX(), getY() - 1))
				return true;
		return false;
	case Compiler::Condition::i_was_bit:
		if (wasBit)
			return true;
		return false;
	case Compiler::Condition::i_was_blocked_from_moving:
		if (wasBlocked)
			return true;
		return false;;
	default:
		return false;
	}
}

void Ant::interpret(const Compiler::Command& cmd)
{
	int num = 0;
	switch (cmd.opcode)
	{
	case Compiler::Opcode::goto_command:
		ic = stoi(cmd.operand1);
		return;
	case Compiler::Opcode::if_command:
		if (conditionTriggered(cmd))
		{
			ic = stoi(cmd.operand2);
		}
		else
			++ic;
		return;;
	case Compiler::Opcode::emitPheromone:
		getStudentWorld()->insertPheromone(getX(), getY(), getColony());
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::faceRandomDirection:
		num = randInt(0, 3);
		if (num == 0)
			setDirection(up);
		else if (num == 1)
			setDirection(down);
		else if (num == 2)
			setDirection(right);
		else
			setDirection(left);
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::rotateClockwise:
		if (getDirection() == up)
			setDirection(right);
		else if (getDirection() == right)
			setDirection(down);
		else if (getDirection() == down)
			setDirection(left);
		else 
			setDirection(up);
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::rotateCounterClockwise:
		if (getDirection() == up)
			setDirection(left);
		else if (getDirection() == right)
			setDirection(up);
		else if (getDirection() == down)
			setDirection(right);
		else
			setDirection(down);
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::moveForward:
		if (getDirection() == up)
		{
			if (!getStudentWorld()->blocked(getX(), getY() + 1))
			{
				moveTo(getX(), getY() + 1);
				unstun();
				unpoison();
				wasBlocked = false;
				wasBit = false;
			}
			else
				wasBlocked = true;
		}
		else if (getDirection() == down)
		{
			if (!getStudentWorld()->blocked(getX(), getY() - 1))
			{
				moveTo(getX(), getY() - 1);
				unstun();
				unpoison();
				wasBlocked = false;
				wasBit = false;
			}
			else
				wasBlocked = true;
		}
		else if (getDirection() == right)
		{
			if (!getStudentWorld()->blocked(getX() + 1, getY()))
			{
				moveTo(getX() + 1, getY());
				unstun();
				unpoison();
				wasBlocked = false;
				wasBit = false;
			}
			else
				wasBlocked = true;
		}
		else if (getDirection() == left)
		{
			if (!getStudentWorld()->blocked(getX() - 1, getY()))
			{
				moveTo(getX() - 1, getY());
				unstun();
				unpoison();
				wasBlocked = false;
				wasBit = false;
			}
			else
				wasBlocked = true;
		}
		done = true;
		++ic;
		return;;
	case Compiler::Opcode::bite:
		getStudentWorld()->biteSuccess(this, getX(), getY());
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::pickupFood:
		appendBurden(getStudentWorld()->pickupFood(this, getX(), getY()));
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::dropFood:
		appendBurden(-1 * getStudentWorld()->dropFood(getX(), getY(), getBurden()));
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::eatFood:
		eat();
		++ic;
		done = true;
		return;;
	case Compiler::Opcode::generateRandomNumber:
		lrn = randInt(0, stoi(cmd.operand1) - 1);
		++ic;
		return;;
	}
}