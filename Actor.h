#ifndef ACTOR_H
#define ACTOR_H
#include "GraphObject.h"
#include<queue>
//#include "StudentWorld.h"
class StudentWorld;

//Base Class for every graph object
class Actor : public GraphObject
{
public:
	/*
	Base Class Constructor:
	- Takes in the basic values of the graph object and student world and passes it to the graph object.
	- sets the setVisible to true to display the object on the screen.
	- Initalizes the private member variables.
	*/
	Actor(StudentWorld* game, int Image_ID, int dX, int dY, Direction d, double size, int depth);

	//can be called by the World to get one of the game’s actors to do something.
	virtual void doSomething() = 0;

	//Returning the status of the graph object (whether it is alive or not)
	bool isAlive();

	//Sets the life status to dead.
	void dead();

	//Moves the graphObject to passed in x,y coordinate.
	void moveObj(int x, int y);

	//returns the sGame
	StudentWorld* rWorld();

	//Destructor
	virtual ~Actor();

private:
	bool lifeStatus;
	StudentWorld* sGame;

};

//------------------------------------Earth Class---------------------------------------
class Earth : public Actor
{
public:
	Earth(StudentWorld *world, int Xs, int Ys);
	virtual void doSomething();
	~Earth();
};
//-----------------------------------Boulder Class---------------------------------------
class Boulder :public Actor
{
public:
	Boulder(StudentWorld* world, int Xs, int Ys);
	virtual void doSomething();
	~Boulder();
private:
	bool condition;
	int ticks;
	bool falling;
};
//-----------------------------------Squirt Class-----------------------------------------
class Squirt : public Actor
{
public:
	Squirt(StudentWorld* world, int Xs, int Xy, Direction d);
	virtual void doSomething();
	~Squirt();
private:
	bool annoyPro();
	int travelComplete;
};
//------------------Goodies Class (Base class for all bonus materials)-----------------------------
class Goodies : public Actor
{
public:
	Goodies(StudentWorld* world, int ID, int Xs, int Xy);
	virtual void doSomething() = 0;
	~Goodies();
};
//----------------------------------Oil Barrel Class---------------------------------------
class OilBarrel : public Goodies
{
public:
	OilBarrel(StudentWorld* world, int Xs, int Xy);
	virtual void doSomething();
	~OilBarrel();
private:
	bool visible;
};
//---------------------------------Gold Nugget Class ----------------------------------------
class GoldNugg :public Goodies
{
public:
	GoldNugg(StudentWorld* world, int startX, int startY, bool what);
	virtual void doSomething();
	~GoldNugg();

private:
	int ticks;
	bool pickable;
};
//---------------------------------Sonar Class ----------------------------------------------
class Sonar :public Goodies 
{
public:
	Sonar(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
	~Sonar();
private:
	int valid;
	int ticks;
};
//---------------------------------WaterPool Class -------------------------------------------
class WaterPool : public Goodies
{
public:
	WaterPool(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
	~WaterPool();
private:
	int valid;
	int ticks;
};
//------------------------------------Protesters and Tunnel Man Base Class------------------
class Hero_Villain : public Actor
{
public:
	Hero_Villain(StudentWorld* w, int IM_ID, int dX, int dY, Direction d, double size, int depth ,int hp);
	int ghp();
	int chp(int hitp);
	virtual void isAnnoyed(int hitp) = 0;
	virtual ~Hero_Villain();
private: 
	int hitPoints;
};

//------------------------------------TunnelMan Class -------------------------------------------
class TunnelMan : public Hero_Villain
{
public:
	//Constructor And destructor
	TunnelMan(StudentWorld * w);
	~TunnelMan();

	//Virtual Methods
	virtual void doSomething();
	virtual void isAnnoyed(int hitP);

	
	

	int returnwSquirt();
	int returnSonar();
	int returngoldN();
	int returnHitPoints();

	void inc(int image, int x);
	
private:
	int wSquirt;
	int sonar;
	int goldN;
	int hitPoints;

	//helper Functions
	void fire();
};
//---------------------------------Protestor Base Class-------------------------------------------
class Protestor : public Hero_Villain
{
public:
	Protestor(StudentWorld* w, int ID, int hp);
	virtual void doSomething();
	virtual void isAnnoyed(int hp);
	
	void freeze(); 
	void pickupGold();

private:
	bool leaveTheOilField;
	int wait;
	int move;
	int noYell;
	int turn;
	Direction signal;

	Direction getD();
	bool isStraight(Direction d);
	void pMove(Direction d);
	Direction randomD();
	bool canMove(int x, int y, Direction d);
	bool isInterction();
	void pickViable();
	int PMaze[64][64];
	void whereIsExitorSignal(Protestor* p, char c);
	void setSignal(Direction d);
	Direction returnSignal();

	struct Maze
	{
		int p;
		int q;
		Maze(int r, int s) :p(r), q(s){}
	};
};
//-----------------------------Regular Protestor-----------------------------------
class RegularProtestor : public Protestor
{
public:
	RegularProtestor(StudentWorld* world);
};
//------------------------------Hard Core Protestor----------------------------------
class HardCoreProtestor : public Protestor
{
public:
	HardCoreProtestor(StudentWorld* world);
};
#endif
