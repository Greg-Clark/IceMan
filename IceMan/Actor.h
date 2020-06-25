#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "StudentWorld.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

// Changed this class so that all of the classes have acess to the 
// Student world

struct LeavingMap {
	// Funcitons
	LeavingMap(StudentWorld* w) {
		
		for (int i = 0; i < 61; ++i) {
			for (int j = 0; j < 61; ++j)
				map[i][j] = 1000;
		}
		
		world = w;
		calculateMap();
	}
	void calculateMap() noexcept;
	void calculateMapAux(int, int, int) noexcept;
	StudentWorld* getWorld() const { return world; }

	// Data Members
	StudentWorld* world;
	int map[61][61];
};

class Actor : public GraphObject {
protected:
	// That way all of the objects have acess to world for their
	// doSomething methods
	StudentWorld* world = nullptr;
	bool isItDead;
	int vecPosition;
	bool protestor;
	int hitPoints;
	bool stun;
	int stunTime;
	int isGold;
public:
	Actor(int ID, int x, int y, Direction d, double size, int depth, StudentWorld* w)
		: GraphObject(ID, x, y, d, size, depth) {
		world = w;
		isItDead = false;
			stun = false;
			stunTime = 0;
			protestor = false;
			isGold = false;
	}
	virtual void doSomething() = 0;
	virtual StudentWorld* getWorld() { return world; }
	virtual bool isDead() const { return isItDead; }
	virtual void setDead() { isItDead = true; }
	int getVecPosition() const { return vecPosition; }
	bool returnProtestor() const { return protestor; }
	bool isItGold() const { return isGold; }
	void hitBySquirt();
	bool isProtestor() { return protestor; }
	virtual ~Actor() { }
};

class Ice : public Actor {
public:
	// Proper Constructor made for Ice to construct GraphObject
	Ice(int x, int y) : Actor(IID_ICE, x, y, right, 0.25, 3, nullptr) {
		this->setVisible(true);
	}
	void doSomething() { }
	~Ice() { }
};

class MovingObject : public Actor {
protected:
	// Protected so both iceman and protestors can access their hitpoints
public:
	// Constructor normal constructor for general Moving obj with HP parameter
	MovingObject(int hp, int ID, int x, int y, Direction d, float size, int depth, StudentWorld* w)
		: Actor(ID, x, y, d, size, depth, w) {
		hitPoints = hp;
		world = w;
	};
	// All objects other than ice do nothing
	// Pure Virtual fct
	virtual void doSomething() = 0;
	//virtual StudentWorld* getWorld();
	virtual ~MovingObject() { }
};

class Protestor : public MovingObject {
protected:
	bool isHardcore;
	bool isLeaving;
	int ticksToWait;
	int ticksToTurn;
	int restCounter;
	int numSquaresToMoveInCurrentDirection;
	int direction;
	bool shouting;
	int shoutingTimer;
	LeavingMap* exitMap;
	int perpendicularTurnTime;
	bool madeTurn;
public:
	Protestor(int hp, int ID, StudentWorld* w) : MovingObject(hp, ID, 60, 60, left, 1, 0, w) {
		setVisible(true);
		protestor = true;
		isLeaving = false;
		ticksToWait = 0;
		numSquaresToMoveInCurrentDirection = 0;
		direction = -1;
		shouting = false;
		shoutingTimer = 15;
		exitMap = new LeavingMap(getWorld());
		perpendicularTurnTime = 0;
		madeTurn = false;
	}
	bool getType() const { return isHardcore; }
	bool getStatus() const { return isLeaving; }
	virtual void doSomething() = 0;
	//void leaveTheMap();
	//void followIceMan();
	//bool checkSquirt();
	//bool checkGold();
	void moveToExit();
	bool iceManInView(int&);
	bool isAtIntersection(int&);
	//void setTicksToWait();
	void moveProtestor();
	void setToLeaving() { isLeaving = true; }
	void setHitpointsToZero() { hitPoints -= hitPoints; }
	//void move();
};


class RegularProtestor : public Protestor {
private:

public:
	RegularProtestor(StudentWorld * w) : Protestor(5, IID_PROTESTER, w) {
		isHardcore = false;
	}
	void doSomething();
};

class HardcoreProtestor : public Protestor{
private:
	int currentStareTick;
	int ticksToStare;
	bool isStaring;
public:
	HardcoreProtestor(StudentWorld* w) : Protestor(20, IID_HARD_CORE_PROTESTER, w) {
		isHardcore = true;
		currentStareTick = 0;
		ticksToStare = -1;
		isStaring = false;
	}
	void doSomething();
	void setTicksToStare(int ticks) { ticksToStare = ticks;}
	void setIsStaring(bool stare) { isStaring = stare; }
};

class IceMan : public MovingObject {
public:
	IceMan(StudentWorld* w) : MovingObject(10, IID_PLAYER, 30, 60, right, 1, 0, w) {
		setVisible(true);
		numSquirts = 5;
		numSonar = 1;
		numGold = 0;
		numOil = 0;
	};
	// Wasn't sure if I needed to re-declare this, even though it is already
	// in the MovingObject class
	void doSomething();
	int getHitPoints() const { return hitPoints; }
	int getSquirts() const { return numSquirts; }
	int getBarrels() const { return numOil; }
	int getSonar() const { return numSonar; }
	int getGold() const { return numGold; }

	void incOil() { ++numOil; }
	void incGold() { ++numGold; }
	void decGold() { --numGold; }
	void incSquirt() { numSquirts += 5; }
	void decSquirt() { --numSquirts; }
	void incSonar() { ++numSonar; }
	void decSonar() { --numSonar; }
	void decHealth() { hitPoints-= 2; }



	void makeHimDead() { isItDead = true; hitPoints = 0; }

	~IceMan() { }

private:
	// These are things that are only in IceMan
	int numSquirts;
	int numSonar;
	int numGold;
	int numOil;
	int numWaterPool;
};


class MapObject : public Actor {
protected:
	StudentWorld* world;
public:
	// All of the Map objects start out with a size of one, 
	// but everything else is specific to each MapObject
	MapObject(int ID, int x, int y, Direction d, int depth, StudentWorld* w)
		: Actor(ID, x, y, d, 1, depth, w) { };
	virtual ~MapObject() { };
	// Pure virtual class
	virtual void doSomething() = 0;

};

class Boulder : public MapObject {
private:
	// A bool to tell whether the boulder has fallen yet
	int m_waitingTime = 0;
	bool hasFallen;
public:
	Boulder(int x, int y, StudentWorld* w) : MapObject(IID_BOULDER, x, y, down, 1, w) {
		setVisible(true);
		hasFallen = false;
	};
	virtual ~Boulder() { };
	virtual void doSomething();
	virtual bool isFalling();
	bool isStable();
	bool doneWaiting();
};

class Gold : public MapObject {
private:
	// This is to decide if the gold can be picked up by either Iceman or 
	// one of the protestors as a bribe. It cannot be both, which I think
	// might make things a bit easier
	bool isBribe;
	int bribeTime;
public:
	Gold(int x, int y, int vecPos, StudentWorld* w) : MapObject(IID_GOLD, x, y, right, 2, w) {
		setVisible(false);
		isBribe = false;
		bribeTime = 0;
		isGold = true;
	}
	bool isBribeState() const { return isBribe; }
	void updateisBribeState(bool update) { isBribe = update; }
	void doSomething();
	virtual ~Gold() {
		world = nullptr;
	}
};

class OilBarrel : public MapObject {
private:

public:
	OilBarrel(int x, int y, StudentWorld* w) : MapObject(IID_BARREL, x, y, right, 2, w) {
		setVisible(false);
	};
	virtual ~OilBarrel() { };
	void doSomething() { }

};

class Squirt : public MapObject {
private:
	bool isSquirt;
	int squirtTime;
public:
	Squirt(int x, int y, int VecPos, Direction d, StudentWorld* w) : MapObject(IID_WATER_SPURT, x, y, d, 1, w) {
		setVisible(true);
		isSquirt = false;
		squirtTime = 0;
		vecPosition = VecPos;
	}
	void doSomething();
	bool getIsSquirt() const { return isSquirt; }
	void updateIsSquirt(bool update) { isSquirt = update; }
	//int getVecPosition() const;

};

class Sonar : public MapObject {
private:
	int sonarTime;
	bool isActive;
public:
	Sonar(int vecPos, StudentWorld* w) : MapObject(IID_SONAR, 0, 60, right, 2, w) {
		setVisible(true);
		vecPosition = vecPos;
		sonarTime = 0;
		isActive = false;
	}
	void doSomething();
	void setActive() { isActive = true; }
};

class WaterPool : public MapObject {
private:
	int waterTime;
	bool isActive;
public:
	WaterPool(int x, int y, int vecPos, StudentWorld* w) : MapObject(IID_WATER_POOL, x, y, right, 2, w) {
		setVisible(true);
		vecPosition = vecPos;
		waterTime = 0;
		isActive = false;
	}
	void doSomething();
	void setActive() { isActive = true; }
};

#endif //ACTOR_H_
