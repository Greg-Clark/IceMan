#include "StudentWorld.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init() {
	srand((unsigned)time(0));
	protestorWaitTime = max(25, 200 - int(getLevel()));
	maxNumProtestor = min(15, 2 + (int(getLevel()*1.5)));
	protestorWaitTimeCounter = 1;
	
	// Create & print out ice with 2d array
	createIce();
	
	//create iceman
	player = new IceMan(this);
	
	// THIS ORDER MUST NOT CHANGE
	updateItemCount();
	createBoulder();
	createGold();
	createOil();
	
	createProtestor();
	numProtestor = 1;
	
	// DO NOT MESS WITH THE ORDER OF THESE FUNCTION CALLS

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	if(protestorWaitTimeCounter % protestorWaitTime == 0 && numProtestor < maxNumProtestor)
	{
		createProtestor();
		++numProtestor;
	}
	++protestorWaitTimeCounter;
	
	// If Ice man has lost all of his hit points, Iceman dies and the level
	// restarts
	if (player->getHitPoints() <= 0) {
		decLives();
		playSound(SOUND_PLAYER_ANNOYED);
		return GWSTATUS_PLAYER_DIED;
	}
	player->doSomething();
	updateScore();
	
	createSonarOrWater();
	
	for (unsigned int i = 0; i < gameActors.size(); ++i) {
		if(gameActors[i] != nullptr)
			gameActors[i]->doSomething();
		//if gameactor is dead, delete and then set to nullptr
		if(gameActors[i] != nullptr && gameActors[i]->isDead())
		{
			//decreases the amount of protestors on the board
			if(gameActors[i]->isProtestor())
			{
				--numProtestor;
			}
			delete gameActors[i];
			gameActors[i] = nullptr;
		}
	}
	// If all oil Barells have been picked up, the level has been complete
	if (oilNum - player->getBarrels() == 0) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	for(int i = 0; i < 64; ++i)
	{
		for(int j = 0; j < 64; ++j)
		{
			if(ice[i][j] != nullptr){
				delete ice[i][j];
				ice[i][j] = nullptr;
			}
		}
	}
	if(player != nullptr){
		delete player;
		player = nullptr;
	}
	for (int i = 0; i < gameActors.size(); ++i) {
		if (gameActors[i] != nullptr)
			delete gameActors[i];
	}
	gameActors.clear();
}

void StudentWorld::createIce(){
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 64; ++j){
			ice[i][j] = new Ice(i, j);
			
			if(j >= 60)	//sets top row to clear
				ice[i][j]->setVisible(false);
			
			// This makes the initial cavern
			if (i < 34 && i >= 30 && j > 4)
				ice[i][j]->setVisible(false);
		}
	}
}

void StudentWorld::removeBlocks(int x, int y)
{
	//if the iceman destroys atleast one block then the dig sound gets played
	if(ice[x][y-64]->isVisible())
	{
		playSound(SOUND_DIG);
	}
	
	else if (ice[x][y - 63]->isVisible()) {
		playSound(SOUND_DIG);
	}
	
	else if (ice[x][y - 62]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x][y - 61]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 1][y - 64]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 1][y - 63]->isVisible()) {
		playSound(SOUND_DIG);
	}
	
	else if (ice[x + 1][y - 62]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 1][y - 61]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 2][y - 64]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 2][y - 63]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 2][y - 62]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 2][y - 61]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 3][y - 64]->isVisible()) {
		playSound(SOUND_DIG);
	}


	else if (ice[x + 3][y - 63]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if (ice[x + 3][y - 62]->isVisible()) {
		playSound(SOUND_DIG);
	}

	else if(ice[x+3][y-61]->isVisible())
	{
		playSound(SOUND_DIG);
	}
	//sets all blocks the iceman touches to not visable
	ice[x][y-64]->setVisible(false);
	ice[x][y-63]->setVisible(false);
	ice[x][y-62]->setVisible(false);
	ice[x][y-61]->setVisible(false);
	
	ice[x+1][y-64]->setVisible(false);
	ice[x+1][y-63]->setVisible(false);
	ice[x+1][y-62]->setVisible(false);
	ice[x+1][y-61]->setVisible(false);
	
	ice[x+2][y-64]->setVisible(false);
	ice[x+2][y-63]->setVisible(false);
	ice[x+2][y-62]->setVisible(false);
	ice[x+2][y-61]->setVisible(false);
	
	ice[x+3][y-64]->setVisible(false);
	ice[x+3][y-63]->setVisible(false);
	ice[x+3][y-62]->setVisible(false);
	ice[x+3][y-61]->setVisible(false);
	
}

StudentWorld::~StudentWorld(){
	for(int i = 0; i < 64; ++i)
	{
		for(int j = 0; j < 64; ++j)
		{
			if(ice[i][j] != nullptr)
				delete ice[i][j];
		}
	}
	if(player != nullptr)
		delete player;
}

//puts all values nicely into a string for the scoreboard
string StudentWorld::formatScoreBoard(int level, int lives, int health,
	int squirts, int gold, int barrels, int sonar, int score){
	string str = "Level: " + to_string(level) + " Lives: " + to_string(lives)
		+  " Health: " + to_string(health) + "% Water: " + to_string(squirts)
		+ " Gold: " + to_string(gold) + " Oil Left: " + to_string(barrels)
		+ " Sonar: " + to_string(sonar) + " Score: ";
	
	//checks to see what the score is an determines how many zeroes to put in front
	//Honestly Jonas there is probably a better way of doing this using setprecision 
	//but this seemed easier to do. If you want to change it feel free to
	if((score / 10) <= 0)
		str += "00000" + to_string(score);
	else if((score / 100) <= 0)
		str += "0000" + to_string(score);
	else if((score / 1000) <= 0)
		str += "000" + to_string(score);
	else if((score / 10000) <= 0)
		str +=  "00" + to_string(score);
	else if((score / 100000) <= 0)
		str +=  "0" + to_string(score);
	else
		str += to_string(score);
	return str;
}

void StudentWorld::updateScore(){
	int level = getLevel();
	int lives = getLives();
	
	//Needs to be implented properly
	//Is currently only holding dummy variables
	int health = player->getHitPoints()*10;
	int squirts = player->getSquirts();
	int gold = player->getGold();
	int barrels = oilNum - player->getBarrels();
	int sonar = player->getSonar();
	
	int score = getScore();
	string s = formatScoreBoard(level, lives, health, squirts, gold, barrels, sonar, score);
	setGameStatText(s);
}

void StudentWorld::removeBoulderIce(int x, int y)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			ice[x + i][y + j]->setVisible(false);
		}
	}
}

//creates both boulders
void StudentWorld::createBoulder() {
	
	int total = bouldNum;
	for (int i = 0; i < total; ++i) {
		
		int randomX = (rand() % 60);
		while((gameActors.size() == 0) && (randomX > 26 && randomX < 34)) {
			randomX = (rand()%60);
		}
		
		int randomY = (rand() % 50) + 6;
		
		checkForObject(randomX, randomY);
		//if (i < bouldNum) {
		Boulder* temp = new Boulder(randomX, randomY, this);
		gameActors.push_back(temp);
		removeBoulderIce(randomX, randomY);
	}
}

bool StudentWorld::isBoulderThereL(Actor* p)
{
	for (int i = 0; i < bouldNum; ++i) {
		if (gameActors[i] != nullptr) {
			double deltaX = gameActors[i]->getX() - p->getX() + 1;
			double deltaY = gameActors[i]->getY() - p->getY();
			double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if (radius <= 3)
				return true;
		}
	}
	return false;
}

bool StudentWorld::isBoulderThereU(Actor* p)
{
	for (int i = 0; i < bouldNum; ++i) {
		if (gameActors[i] != nullptr) {
			double deltaX = gameActors[i]->getX() - p->getX();
			double deltaY = gameActors[i]->getY() - p->getY() - 1;
			double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if (radius <= 3)
				return true;
		}
	}
	return false;
}

bool StudentWorld::isBoulderThereR(Actor* p)
{
	for (int i = 0; i < bouldNum; ++i) {
		if (gameActors[i] != nullptr) {
			double deltaX = gameActors[i]->getX() - p->getX() - 1;
			double deltaY = gameActors[i]->getY() - p->getY();
			double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if (radius <= 3)
				return true;
		}
	}
	return false;
}

bool StudentWorld::isBoulderThereD(Actor* p)
{

	for (int i = 0; i < bouldNum; ++i) {
		if (gameActors[i] != nullptr && gameActors[i] != p) {
			double deltaX = gameActors[i]->getX() - p->getX();
			double deltaY = gameActors[i]->getY() - p->getY() + 1;
			double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if (radius <= 3)
				return true;
		}
	}
	return false;
}

void StudentWorld::checkForObject(int &x, int &y) {

	for (unsigned int i = 0; i < gameActors.size(); i++)
	{
		if (gameActors[i] != nullptr) {
			double radius;
			int deltaX = abs(x - gameActors[i]->getX());
			int deltaY = abs(y - gameActors[i]->getY());
			radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			bool inCavern = (x > 26 && x < 34);
			bool invalidRadius = false;
			if (radius < 6)
				invalidRadius = true;
			
			if (inCavern || invalidRadius) {
				int tempX = rand() % 60;
				x = tempX;
				int tempY = (rand() % 50) + 6;
				y = tempY;
				i = -1;
			}
		}
	}
}

bool StudentWorld::isIceVisable(int x, int y, Actor::Direction dir)
{
	if(x < 0 || x > 63 || y < 0 || y > 62)
		return true;
	if(dir == Actor::up || dir == Actor::down)
	{
		for (int i = 0; i <= 3; i++)
		{
			//if ice does not exist just return true to avoid bad access error
			if (ice[x + i][y] == nullptr)
				return true;
			if (ice[x + i][y]->isVisible())
				return true;
		}
	}
	else if(dir == Actor::right || dir == Actor::left)
	{
		for (int i = 0; i <= 3; i++)
		{
			//if ice does not exist just return true to avoid bad access error
			if (ice[x][y + i] == nullptr)
				return true;
			if (ice[x][y + i]->isVisible())
				return true;
		}
	}
	return false;
}

void StudentWorld::updateItemCount() {
	// A function that is called during initializaion of the level 
	// to update the number of boulders, gold nuggets, and barrels
	bouldNum = min(int(getLevel() / 2) + 2, 9);
	goldNum = max(int(5 - getLevel() / 2), 2);
	oilNum = min(int(2 + getLevel()), 21);
}

void StudentWorld::createGold(){
	for (int i = bouldNum; i < bouldNum+goldNum; ++i) {
		
		int randomX = (rand() % 60);
		while((gameActors.size() == 0) && (randomX > 26 && randomX < 34))
		{
			randomX = (rand()%60);
		}
		
		int randomY = (rand() % 50) + 6;
		
		checkForObject(randomX, randomY);
		
	Gold * temp = new Gold(randomX, randomY, int(gameActors.size()), this);
	gameActors.push_back(temp);
	}
}

void StudentWorld::createOil() {
	int bouldGold = bouldNum + goldNum;
	int bouldGoldOil = bouldGold + oilNum;
	for (int i = bouldGold; i < bouldGoldOil; ++i) {
		int randomX = (rand() % 60);
		while ((gameActors.size() == 0) && (randomX > 26 && randomX < 34)) {
			randomX = (rand() % 60);
		}
		int randomY = (rand() % 50) + 6;
		checkForObject(randomX, randomY);

		OilBarrel * temp = new OilBarrel(randomX, randomY, this);
		gameActors.push_back(temp);
	}
}

// Needs to be updated for protestors
void StudentWorld::isMapObjectThere(int x, int y)
{
	int bouldAndGold = bouldNum + goldNum;
	int bouldGoldAndOil = bouldAndGold + oilNum;
	double radius = 0;
	double deltaX, deltaY;
	for(int i = 0; i < bouldGoldAndOil; ++i) {
		if(gameActors[i] != nullptr)
		{
			deltaX = abs(gameActors[i]->getX() - x);
			deltaY = abs(gameActors[i]->getY() - y);
			radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if(radius <= 3){
				if (i < bouldNum)
				{
					player->makeHimDead();
				}
				else if (i < bouldAndGold) {
					// If picked up gold, increase count, delete from
					// student world, increase score, play sound
					delete gameActors[i];
					gameActors[i] = nullptr;
					playSound(SOUND_GOT_GOODIE);
					increaseScore(10);
					player->incGold();
					return;
				}
				else if (i < bouldGoldAndOil) {
					// If picked up oil, increase count, delete from
					// student world, increase score, play sound
					delete gameActors[i];
					gameActors[i] = nullptr;
					playSound(SOUND_FOUND_OIL);
					increaseScore(1000);
					player->incOil();
					//--oilNum;
					return;
				}
			}
			else if(radius <= 4) {
				gameActors[i]->setVisible(true);
			}
		}
	}
}

//checks to see if a boulder is falling on a reg protestor or if it's being bribbed
void StudentWorld::isMapObjectThereRegProtestor(int x, int y, RegularProtestor* p)
{
	int deltaX, deltaY, radius;
	//if its a boulder
	for(int i = 0; i < bouldNum; ++i) {
		if(gameActors[i] != nullptr)
		{
			deltaX = abs(gameActors[i]->getX() - x);
			deltaY = abs(gameActors[i]->getY() - y);
			radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if(radius <= 3){
				p->setHitpointsToZero();
				gameActors[i]->setDead();
				return;
			}
		}
	}
	//if its a gold nugget
	for(int i = (bouldNum+goldNum+oilNum); i < gameActors.size(); ++i)
	{
		if(gameActors[i] != nullptr && gameActors[i]->isItGold())
		{
			deltaX = abs(gameActors[i]->getX() - x);
			deltaY = abs(gameActors[i]->getY() - y);
			radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if(radius <= 3){
				gameActors[i]->setDead();
				p->setToLeaving();
				playSound(SOUND_PROTESTER_FOUND_GOLD);
				increaseScore(25);
				return;
			}
		}
	}
}
//checks to see if a boulder is falling on a hard protestor or if it's being bribbed
void StudentWorld::isMapObjectThereHardProtestor(int x, int y, HardcoreProtestor* p)
{
	int deltaX, deltaY, radius;
	//if its a boulder
	for(int i = 0; i < bouldNum; ++i) {
		if(gameActors[i] != nullptr)
		{
			deltaX = abs(gameActors[i]->getX() - x);
			deltaY = abs(gameActors[i]->getY() - y);
			radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if(radius <= 3){
				p->setHitpointsToZero();
				gameActors[i]->setDead();
				return;
			}
		}
	}
	//if its a gold nugget
	for(int i = (bouldNum+goldNum+oilNum); i < gameActors.size(); ++i)
	{
		if(gameActors[i] != nullptr && gameActors[i]->isItGold())
		{
			deltaX = abs(gameActors[i]->getX() - x);
			deltaY = abs(gameActors[i]->getY() - y);
			radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if(radius <= 3){
				gameActors[i]->setDead();
				playSound(SOUND_PROTESTER_FOUND_GOLD);
				increaseScore(50);
				p->setIsStaring(true);
				p->setTicksToStare(max(50, 100 - int(getLevel())*10));
				return;
			}
		}
	}
}


void StudentWorld::dropGold(int x, int y)
{
	if (player->getGold() > 0) {
		Gold* temp = new Gold(x, y, int(gameActors.size()), this);
		temp->setVisible(true);
		temp->updateisBribeState(true);
		gameActors.push_back(temp);
		player->decGold();
	}
}

void StudentWorld::createSquirt(int x, int y, Actor::Direction dir){
	if (player->getSquirts() > 0)
	{
		Squirt* temp = new Squirt(x, y, int(gameActors.size()), dir, this);
		temp->updateIsSquirt(true);
		gameActors.push_back(temp);
		player->decSquirt();
	}
}
// Needs to be updated for protestors
void StudentWorld::checkSquirtRadius(int x, int y, int pos){
	int bouldAndGold = bouldNum + goldNum;
	int bouldGoldAndOil = bouldAndGold + oilNum;
	double radius = 0;
	double deltaX, deltaY;
	for(int i = 0; i < bouldGoldAndOil; ++i) {
		if(gameActors[i] != nullptr)
		{
			deltaX = abs(gameActors[i]->getX() - x);
			deltaY = abs(gameActors[i]->getY() - y);
			radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if(radius <= 3){
				gameActors[pos]->setDead();
			}
		}
	}
	for(int i = bouldGoldAndOil; i < gameActors.size(); ++i)
	{
		if(gameActors[i] != nullptr)
		{
			if(gameActors[i]->returnProtestor())
			{
				deltaX = abs(gameActors[i]->getX() - x);
				deltaY = abs(gameActors[i]->getY() - y);
				radius = sqrt(deltaX * deltaX + deltaY * deltaY);
				if(radius <= 3){
					gameActors[pos]->setDead();
					gameActors[i]->hitBySquirt();
				}
			}
		}
	}
}

bool StudentWorld::checkInitialSquirt(int x, int y)
{
	if(x < 0 || x > 60)
		return true;
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(ice[x+i][y+j]->isVisible())
			{
				return true;
			}
		}
	}
	return false;
}

// Sonar Related Functions

void StudentWorld::createSonar() {
	Sonar* temp = new Sonar( int(gameActors.size()), this);
	temp->setActive();
	gameActors.push_back(temp);
}

int StudentWorld::sonarTimeMax(){
	return max(100, 300 - 10*int(getLevel()));
}

void StudentWorld::useSonar() {
	// Only runs if the player has enough sonar charges

	if (player->getSonar() <= 0)
		return;
	for (int i = 0; i < gameActors.size(); ++i) {
		if (gameActors[i] != nullptr) {
			double deltaX = gameActors[i]->getX() - player->getX();
			double deltaY = gameActors[i]->getY() - player->getY();
			double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
			if (radius <= 12)
				gameActors[i]->setVisible(true);
		}
	}
	playSound(SOUND_SONAR);
	player->decSonar();
}

void StudentWorld::pickUpSonar(int pos) {
	double deltaX = player->getX() - gameActors[pos]->getX();
	double deltaY = player->getY() - gameActors[pos]->getY();
	double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (radius <= 3) {
		gameActors[pos]->setDead();
		player->incSonar();
		playSound(SOUND_GOT_GOODIE);
	}
}

//Water Pool related Functions

void StudentWorld::createWaterPool(){
	int randX = rand()%60, randY = rand()%60;
	while(isThereIce(randX, randY))
	{
		randX = rand()%60;
		randY = rand()%60;
	}
	WaterPool* temp = new WaterPool(randX, randY, int(gameActors.size()) , this);
	temp->setActive();
	gameActors.push_back(temp);
}

void StudentWorld::pickUpWaterPool(int pos) {
	double deltaX = player->getX() - gameActors[pos]->getX();
	double deltaY = player->getY() - gameActors[pos]->getY();
	double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (radius <= 3) {
		gameActors[pos]->setDead();
		player->incSquirt();
		playSound(SOUND_GOT_GOODIE);
	}
}

int StudentWorld::waterPoolTimeMax(){
	return max(100, 300 - 10*int(getLevel()));
}

void StudentWorld::createSonarOrWater(){
	int g = int(getLevel())*25+300;
	int randNum = rand()%g;
	if(randNum != 0)
	{
		return;
	}
	int randomNumber = rand()%5;
	if(randomNumber == 0)
	{
		createSonar();
	}
	else
	{
		createWaterPool();
	}
}

bool StudentWorld::isThereIce(int x, int y)
{
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(ice[x+i][y+i]== nullptr)
				return true;
			if(ice[x+i][y+i]->isVisible())
				return true;
		}
	}
	return false;
}

void StudentWorld::createProtestor(){
	srand((unsigned)time(0));
	int HardProbability = min(90, int(getLevel())*10 + 30);
	int randNum = rand()%100;
	if(randNum >= HardProbability)
	{
		createHardProtestor();
	}
	else{
		createRegProtestor();
	}
}

void StudentWorld::createHardProtestor(){
	HardcoreProtestor* temp = new HardcoreProtestor(this);
	gameActors.push_back(temp);
}

// Regular Protestor related functions

void StudentWorld::createRegProtestor() {
	RegularProtestor* temp = new RegularProtestor(this);
	gameActors.push_back(temp);
}


bool StudentWorld::isIceManThere(int x, int y) const {
	if (player->getX() == x && player->getY() == y)
		return true;
	return false;
}

bool StudentWorld::iceProtestorH(int x, int y) const {
	for (int i = 0; i < 4; ++i) {
		if (ice[x][y + i]->isVisible())
			return true;
	}
	return false;
}

bool StudentWorld::iceProtestorV(int x, int y) const {
	for (int i = 0; i < 4; ++i) {
		if (ice[x + i][y]->isVisible())
			return true;
	}
	return false;
}

int StudentWorld::setTicksToWait(){
	return max(0, 3 - int(getLevel()) / 4);
}

bool StudentWorld::isTouchingIceman(Protestor* p){
	double deltaX = player->getX() - p->getX();
	double deltaY = player->getY() - p->getY();
	double radius = sqrt(deltaX * deltaX + deltaY * deltaY);
	if(radius <= 4)
	{
		return true;
	}
	return false;
}

void StudentWorld::decreaseIcemanHealth(){
	player->decHealth();
}
