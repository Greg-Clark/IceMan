#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) {

	}

	virtual int init();
	virtual int move();
	virtual void cleanUp();
	
	bool checkInitialSquirt(int, int);
	void checkSquirtRadius(int, int, int);
	void createSquirt(int, int, Actor::Direction);
	void createWaterPool();
	void decreaseIcemanHealth();
	void dropGold(int, int);
	bool iceProtestorH(int, int) const;
	bool iceProtestorV(int, int) const;
	bool isBoulderThereD(Actor*);
	bool isBoulderThereL(Actor*);
	bool isBoulderThereR(Actor*);
	bool isBoulderThereU(Actor*);
	bool isIceManThere(int, int) const;
	bool isIceVisable(int, int, Actor::Direction);
	void isMapObjectThere(int, int);
	void isMapObjectThereRegProtestor(int,int, RegularProtestor*);
	void isMapObjectThereHardProtestor(int, int, HardcoreProtestor*);
	bool isThereIce(int, int);
	bool isTouchingIceman(Protestor*);
	void pickUpSonar(int);
	void pickUpWaterPool(int pos);
	void removeBlocks(int x, int y);
	void removeBoulderIce(int x, int y);
	int  setTicksToWait();
	int  sonarTimeMax();
	void updateItemCount();
	void useSonar();
	int  waterPoolTimeMax();
	
	//still needs to be implemented
	int findRadius(int);
	
	~StudentWorld();
	
private:
	IceMan* player = nullptr;
	Ice* ice[64][64] = {nullptr};
	std::vector<Actor*> gameActors;
	int bouldNum;
	int goldNum;
	int oilNum;
	void checkForObject(int&, int&);
	void createBoulder();
	void createIce();
	void createGold();
	void createOil();
	void createSonar();
	int numProtestor;
	int maxNumProtestor;
	int protestorWaitTime;
	int protestorWaitTimeCounter;
	void createProtestor();
	void createRegProtestor();
	void createSonarOrWater();
	void createHardProtestor();
	
	void updateScore();	//updates the scoreboard on the top of the screen
	std::string formatScoreBoard(int, int, int, int, int, int, int, int);
};

#endif //STUDENTWORLD_H_
