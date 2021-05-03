#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include<utility>
#include<memory>
#include "vector"
#include <string>


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();

	virtual int init();
	virtual int move();

	virtual void cleanUp();

	bool removeEarth(int x, int y);

	//Creating Functions
	void createEarth(); //Creates Earth Objects
	void createTMan(); // Creates TunnelMan Objects
	void createGoodie(std::shared_ptr<Goodies>); //Creates Goodies - (Sonar, WaterPool)
	void createBoulder(std::shared_ptr<Boulder>); //Creates Boulders
	void createSquirts(std::shared_ptr<Squirt>); // Creates Squirts
	void createBA(); //Creates Boulders actors
	void createOA(); //Creates Oil actors
	void createGN(); //Creates Gold Nuggets
	void createProtestors();//Creates Protestors in the Oil field
	//Other Helper Functions

	//Return true if there there is no earth at position x,y or false.
	bool isClear(int gX, int gY);
	bool isClearBelow(int gX, int gY);
	//Returns True if certain place is occupied by a boulder
	bool OccupiedByBoulder(int gX, int gY);
	//Checks if any goodies and boulders are within minimum radius.
	bool withinRadius(int X, int Y);
	//Checks if tunnelMan is within range
	bool checkTManRange(int X, int Y, int r);
	//Checks Protestor Range
	std::shared_ptr<Protestor> checkProRange(Actor* b, int r);
	// Shows all the items visible in range.
	void showMe();
	//Updates Goodies
	void incr(int image, int x, int score);
	//decreases Oil Barrel
	void decBarrel();
	//decreases protestor lives
	void decreasePLives();
	//Compares direction of player and protestor
	bool checkD(int x, int y, char c);
	//Annoys
	void Annoy(int hp);
	//Returns Players X value
	int returnTX();
	//Returns Players Y value
	int returnTY();


private:

	// To Calcualate radius
	bool findR(int X1, int X2, int Y1, int Y2, int R);

	//Actor Containers
	Earth* earth_tracker[64][64];
	std::vector< std::shared_ptr<TunnelMan>> TPointer;
	std::vector<std::shared_ptr<Goodies>>GPointer;
	std::vector< std::shared_ptr<Boulder>>BPointer;
	std::vector< std::shared_ptr<OilBarrel>>OPointer;
	std::vector< std::shared_ptr<Squirt>>SPointer;
	std::vector<std::shared_ptr<Protestor> >PPointer;

	//Private Helper Functions
	void Squirts_move_helper();
	void Boulders_move_helper();
	void Goodie_move_helper();
	void Oil_move_helper();
	void Protestor_move_helper();


	int barrels = 0;
	int gn = 0;
	int protestorLives;
	bool first;
	int last;
	//Display Setter
	std::string dTxt(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels);
	//Updates the inventory
	void updTxt();
};

#endif // STUDENTWORLD_H_
