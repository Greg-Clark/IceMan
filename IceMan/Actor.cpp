#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <algorithm>
#include <future>
#include <thread>

std::mutex m;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



// Actor Functions

void Actor::hitBySquirt(){
	hitPoints -= 2;
	stun = true;
	stunTime = std::max(50, 100 - int(getWorld()->getLevel())*10);
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
}

// IceMan Functions

void IceMan::doSomething() {
	getWorld()->isMapObjectThere(getX(), getY());
	if (hitPoints == 0)
		return;
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (getX() - 1 >= 0 && (getWorld()->isBoulderThereL(this) == false) && (getDirection() == left)) {
				getWorld()->removeBlocks(getX(), getY());
				moveTo(getX() - 1, getY());
				getWorld()->isMapObjectThere(getX(), getY());
			}
			setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (getX() + 1 <= 60 && (getWorld()->isBoulderThereR(this) == false) && (getDirection() == right)) {
				getWorld()->removeBlocks(getX() + 2, getY());
				moveTo(getX() + 1, getY());
				getWorld()->isMapObjectThere(getX(), getY());
			}
			setDirection(right);
			break;
		case KEY_PRESS_DOWN:
			if (getY() - 1 >= 0 && (getWorld()->isBoulderThereD(this) == false) && (getDirection() == down)) {
				getWorld()->removeBlocks(getX() + 1, getY() - 1);
				moveTo(getX(), getY() - 1);
				getWorld()->isMapObjectThere(getX(), getY());
			}
			setDirection(down);
			break;
		case KEY_PRESS_UP:
			if (getY() + 1 <= 60 && (getWorld()->isBoulderThereU(this) == false) && (getDirection() == up)) {
				getWorld()->removeBlocks(getX() + 1, getY() + 1);
				moveTo(getX(), getY() + 1);
				getWorld()->isMapObjectThere(getX(), getY());
			}
			setDirection(up);
			break;
		case KEY_PRESS_SPACE:
			if (numSquirts > 0) {
				if (getDirection() == right)
				{
					if (getWorld()->checkInitialSquirt(getX() + 4, getY())) {
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
						--numSquirts;
					}
					else
						getWorld()->createSquirt(getX() + 4, getY(), right);
				}
				else if (getDirection() == up)
				{
					if (getWorld()->checkInitialSquirt(getX(), getY() + 4)) {
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
						--numSquirts;
					}
					else
						getWorld()->createSquirt(getX(), getY() + 4, up);
				}
				else if (getDirection() == left)
				{
					if (getWorld()->checkInitialSquirt(getX() - 4, getY())) {
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
						--numSquirts;
					}
					else
						getWorld()->createSquirt(getX() - 4, getY(), left);
				}
				else
				{
					if (getWorld()->checkInitialSquirt(getX(), getY() - 4)) {
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
						--numSquirts;
					}
					else
						getWorld()->createSquirt(getX(), getY() - 4, down);
				}
			}
			break;
		case KEY_PRESS_TAB:
			if (numGold > 0) {
				getWorld()->dropGold(getX(), getY());
				break;
			}
			break;
		case KEY_PRESS_ESCAPE:
			makeHimDead();
			break;
		case 'z':
		case 'Z':
			getWorld()->useSonar();
			break;
		}
	}
}



//Protestor Functions

void Protestor::moveProtestor() {
	if(getDirection() == right && !(getWorld()->isIceVisable(getX() + 4, getY(), right)))
	{
		moveTo(getX() + 1, getY());
	}
	else if(getDirection() == left && !(getWorld()->isIceVisable(getX()-1, getY(), left)))
	{
		moveTo(getX()-1, getY());
	}
	else if(getDirection() == up && !(getWorld()->isIceVisable(getX(), getY()+3, up)))
	{
		moveTo(getX(), getY()+1);
	}
	else if(getDirection() == down && !(getWorld()->isIceVisable(getX(), getY()-1, down)))
	{
		moveTo(getX(), getY()-1);
	}
	else{
		numSquaresToMoveInCurrentDirection = 0;
		return;
	}
	--numSquaresToMoveInCurrentDirection;
}

bool Protestor::iceManInView(int& direction) {
	//right
	for (int i = 0; i + getX() < 60; ++i) {
		if (getWorld()->isIceManThere(getX() + i, getY())){
			direction = 0;
			return true;
		}
		if (getWorld()->iceProtestorH(getX() + i, getY()))
			return false;
	}
	//left
	
		for (int i = 0; getX() - i > 0; ++i) {
			if (getWorld()->isIceManThere(getX() - i, getY())){
				direction = 1;
				return true;
			}
			if (getWorld()->iceProtestorH(getX() - i, getY()))
				return false;
		}
	//up
	
		
		for (int i = 0; getY() + i < 60; ++i) {
			if (getWorld()->isIceManThere(getX(), getY() + i)){
				direction = 2;
				return true;
			}
			if (getWorld()->iceProtestorV(getX(), getY() + i))
				return false;
		}
	//down
		for (int i = 0; i - getY() < 0; ++i) {
			if (getWorld()->isIceManThere(getX(), getY() - i)){
				direction = 3;
				return true;
			}
			if (getWorld()->iceProtestorV(getX(), getY() - i))
				return false;
		}
	return false;
}


bool Protestor::isAtIntersection(int &direction)
{
	//right or left
	if(getY() != 60 && (getDirection() == up || getDirection() == down))
	{
		//right
		if (!(getWorld()->isIceVisable(getX()+3, getY(), right))){
			direction = 0;
			return true;
		}
		//left
		if(!(getWorld()->isIceVisable(getX()-1, getY(), left)))
		{
			direction = 1;
			return true;
		}
	}
	else if(getDirection() == left || getDirection() == right)
	{
		//up
		if (!(getWorld()->isIceVisable(getX(), getY()+3, up))){
			direction = 2;
			return true;
		}
		//down
		if(!(getWorld()->isIceVisable(getX(), getY()-1, down)))
		{
			direction = 3;
			return true;
		}
	}
	return false;
}

// Regular Protestor Functions

void RegularProtestor::doSomething() {
	//just temporary
	//deletes the iceman instead of him leaving
	if(isLeaving)
	{
		moveToExit();
		if((getX() == 60 && getY() == 60) || (getX() >= 60 && getY() <= 0))
		{
			setDead();
		}
		return;
	}
	//checks to see if boulder is falling on protestor
	getWorld()->isMapObjectThereRegProtestor(getX(), getY(), this);
	// If has no health, set dead
	if (hitPoints <= 0) {
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		isLeaving = true;
		exitMap->calculateMap();
		return;
	}
	if(stun)
	{
		if(stunTime > 0)
		{
			--stunTime;
		}
		else{
			stun = false;
		}
		return;
	}
	if ((ticksToWait % int(getWorld()->setTicksToWait())) != 0) {  // Rest state
		++ticksToWait;
		return;
	}
	else { // Not rest state lol leeleeleeleeleeleeleeleeleelee
		if (shouting)
		{
			--shoutingTimer;
			if (shoutingTimer <= 0)
			{
				shouting = false;
				shoutingTimer = 15;
			}
		}

		if (isLeaving) {
			if (getX() == 60 && getY() == 60) {
				setDead();
				return;
			}
			else {
				moveToExit();
				//return;
			}
		}
		else if (getWorld()->isTouchingIceman(this)){
			if(!shouting)
			{
				shouting = true;
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				getWorld()->decreaseIcemanHealth();
			}
		}
		else if (iceManInView(direction)) {
			if (direction == 0) { // if ice man is right
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
			else if (direction == 1) { // if ice man is left
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			else if (direction == 2) { // if ice man is up
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
			else if(direction == 3){ // If ice man is down
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
		}
		else if(!madeTurn && isAtIntersection(direction))
		{
			madeTurn = true;
			perpendicularTurnTime = 0;
			if(direction == 0)
			{
				setDirection(right);
				moveTo(getX()+1, getY());
			}
			else if(direction == 1)
			{
				setDirection(left);
				moveTo(getX()-1, getY());
			}
			else if(direction == 2)
				
			{
				setDirection(up);
				moveTo(getX(), getY()+1);
			}
			else if(direction == 3)
			{
				setDirection(down);
				moveTo(getX(), getY()-1);
			}
			numSquaresToMoveInCurrentDirection = (rand()%60) + 8;
		}
		//picks a new direction
		else if(numSquaresToMoveInCurrentDirection <= 0){
			direction = rand()%4;
			switch (direction) {
				case 0: //right
					setDirection(right);
					break;
					
				case 1: //left
					setDirection(left);
					break;
				case 2: //up
					setDirection(up);
					break;
				case 3: //down
					setDirection(down);
					break;
			}
			numSquaresToMoveInCurrentDirection = (rand()%60) + 8;
			moveProtestor();
		}
		else if(numSquaresToMoveInCurrentDirection > 0){
			moveProtestor();
		}
		++ticksToWait;
	}
}

//Hardcore Protestor Functions

void HardcoreProtestor::doSomething(){
	if(isLeaving)
	{
		moveToExit();
		if((getX() == 60 && getY() == 60) || (getX() >= 60 && getY() <= 0))
		{
			setDead();
		}
		return;
	}
	//checks to see if boulder is falling on protestor or if there is gold
	getWorld()->isMapObjectThereHardProtestor(getX(), getY(), this);
	// If has no health, set dead
	if (hitPoints <= 0) {
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		isLeaving = true;
		exitMap->calculateMap();
		return;
	}
	if(stun)
	{
		if(stunTime > 0)
		{
			--stunTime;
		}
		else{
			stun = false;
		}
		return;
	}
	if ((ticksToWait % int(getWorld()->setTicksToWait())) != 0) {  // Rest state
		++ticksToWait;
		return;
	}
	else { // Not rest state lol leeleeleeleeleeleeleeleeleelee
		if (shouting)
		{
			--shoutingTimer;
			if (shoutingTimer <= 0)
			{
				shouting = false;
				shoutingTimer = 15;
			}
		}
		
		if (isLeaving) {
			if (getX() == 60 && getY() == 60) {
				setDead();
				return;
			}
			else {
				moveToExit();
				//return;
			}
		}
		else if(isStaring)
		{
			//if for some reason is staring is true but ticks to stare is less than 0 make is staring false
			if(ticksToStare < 0)
				isStaring = false;
			if(currentStareTick >= ticksToStare)
			{
				isStaring = false;
				currentStareTick = 0;
				return;
			}
			++currentStareTick;
		}
		else if (getWorld()->isTouchingIceman(this)){
			if(!shouting)
			{
				shouting = true;
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				getWorld()->decreaseIcemanHealth();
			}
		}
		else if (iceManInView(direction)) {
			if (direction == 0) { // if ice man is right
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
			else if (direction == 1) { // if ice man is left
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			else if (direction == 2) { // if ice man is up
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
			else if(direction == 3){ // If ice man is down
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
		}
		else if(!madeTurn && isAtIntersection(direction))
		{
			madeTurn = true;
			perpendicularTurnTime = 0;
			if(direction == 0)
			{
				setDirection(right);
				moveTo(getX()+1, getY());
			}
			else if(direction == 1)
			{
				setDirection(left);
				moveTo(getX()-1, getY());
			}
			else if(direction == 2)
				
			{
				setDirection(up);
				moveTo(getX(), getY()+1);
			}
			else if(direction == 3)
			{
				setDirection(down);
				moveTo(getX(), getY()-1);
			}
			numSquaresToMoveInCurrentDirection = (rand()%60) + 8;
		}
		//picks a new direction
		else if(numSquaresToMoveInCurrentDirection <= 0){
			direction = rand()%4;
			switch (direction) {
				case 0: //right
					setDirection(right);
					break;
					
				case 1: //left
					setDirection(left);
					break;
				case 2: //up
					setDirection(up);
					break;
				case 3: //down
					setDirection(down);
					break;
			}
			numSquaresToMoveInCurrentDirection = (rand()%60) + 8;
			moveProtestor();
		}
		else if(numSquaresToMoveInCurrentDirection > 0){
			moveProtestor();
		}
		++ticksToWait;
		if(madeTurn)
		{
			++perpendicularTurnTime;
		}
		if(perpendicularTurnTime >= 200)
		{
			madeTurn = false;
		}
	}

	return;
}

void Protestor::moveToExit() {
	if (exitMap->map[getX()][getY()] > exitMap->map[getX() + 1][getY()])
		moveTo(getX() + 1, getY()); // Move right
	else if (exitMap->map[getX()][getY()] > exitMap->map[getX() - 1][getY()])
		moveTo(getX() - 1, getY()); // Move left
	else if (exitMap->map[getX()][getY()] > exitMap->map[getX()][getY() + 1])
		moveTo(getX(), getY() + 1); // Move up
	else 
		moveTo(getX(), getY() - 1); // Move down
}



//MapObject Functions



// Boulder Functions

void Boulder::doSomething() {
	//immediately returns if boulder is "dead"
	if (!(isVisible()))
		return;
	//immediately return if boulder is stable
	else if (isStable())
		return;
	//if rock is waiting return
	else if (!(doneWaiting()))
		return;
	else if (isFalling())
	{
		//doSomething and play sound
		moveTo(getX(), getY() - 1);
		if (hasFallen == false) {
			getWorld()->playSound(SOUND_FALLING_ROCK);
			hasFallen = true;
		}
		isFalling();
	}
}

bool Boulder::isFalling() {
	if (isStable())
	{
		setDead();
		return false;
	}
	return true;
}

bool Boulder::isStable() {
	//if boulder is at bottom
	if (getY() <= 0)
		return true;
	if (getWorld()->isBoulderThereD(this))
		return true;
	if (!(getWorld()->isIceVisable(getX(), getY() - 1, down)))
		return false;
	return true;
}

bool Boulder::doneWaiting() {
	if (m_waitingTime >= 30)
		return true;
	m_waitingTime++;
	return false;
}


// Gold Functions
void Gold::doSomething() {
	if (isBribe == true)
	{
		if (bribeTime >= 100)
		{
			setDead();
			isBribe = false;
		}
		bribeTime++;
	}
}

// Oil Barrel Functions

//Squirt Functions
void Squirt::doSomething() {
	if (isSquirt && squirtTime <= 6)
	{
		//checks to see if the squirt is about to hit an actor object
		getWorld()->checkSquirtRadius(getX(), getY(), vecPosition);
		if (getDirection() == right)
		{
			if (squirtTime == 0)
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			moveTo(getX() + 1, getY());
			squirtTime++;
			if (getWorld()->isIceVisable(getX() + 3, getY(), right))
				setDead();
		}
		if (getDirection() == left)
		{
			if (squirtTime == 0)
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			moveTo(getX() - 1, getY());
			squirtTime++;
			if (getWorld()->isIceVisable(getX(), getY(), left))
				setDead();
		}
		if (getDirection() == up)
		{
			if (squirtTime == 0)
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			moveTo(getX(), getY() + 1);
			squirtTime++;
			if (getWorld()->isIceVisable(getX(), getY() + 3, up))
				setDead();
		}
		if (getDirection() == down)
		{
			if (squirtTime == 0)
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			moveTo(getX(), getY() - 1);
			squirtTime++;
			if (getWorld()->isIceVisable(getX(), getY(), down))
				setDead();
		}
	}
	if (squirtTime > 6)
		setDead();
}

// Sonar Functions

void Sonar::doSomething() {
	getWorld()->pickUpSonar(vecPosition);
	if (sonarTime > int(getWorld()->sonarTimeMax()))
	{
		setDead();
	}
	if(isActive)
		++sonarTime;
}

//Water Pool Functions

void WaterPool::doSomething(){
	getWorld()->pickUpWaterPool(vecPosition);
	if(waterTime > int(getWorld()->sonarTimeMax()))
	{
		setDead();
	}
	if(isActive)
		++waterTime;
}

// Leaving Map Functions
void LeavingMap::calculateMap() noexcept{
	int startingX = 60;
	int startingY = 60;
	int startingValue = 0;
	calculateMapAux(startingX, startingY, startingValue);
	
	//std::future<void> ft = std::async(&LeavingMap::calculateMapAux, *this, startingX, startingY, startingValue);
	/*std::thread th(&LeavingMap::calculateMapAux, *this, startingX, startingY, startingValue);
	th.join();*/
}
void LeavingMap::calculateMapAux(int x, int y, int value) noexcept{
	
	if (x < 0 || x > 61)
		return;
	if (y < 0 || y > 61)
		return;
	map[x][y] = value;
	if (x + 1 < 60 && (!(getWorld()->isThereIce(x + 1, y)) && map[x + 1][y] > value + 1)) {
		calculateMapAux(x + 1, y, value + 1);
	}
	if (x - 1 > 0 && (!getWorld()->isThereIce(x - 1, y) && map[x - 1][y] > value + 1)) {
		calculateMapAux(x - 1, y, value + 1);
	}
	if (y + 1 < 60 && (!getWorld()->isThereIce(x, y + 1) && map[x][y + 1] > value + 1))
		calculateMapAux(x, y + 1, value + 1);
	if (y - 1 > 0 && (!getWorld()->isThereIce(x, y - 1) && map[x][y - 1] > value + 1))
		calculateMapAux(x, y - 1, value + 1);
	return;
}


