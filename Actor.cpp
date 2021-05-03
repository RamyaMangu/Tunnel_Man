#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include <algorithm>
using namespace std;

class StudentWorld;
//-----------------------------Actor Class Implmentation-------------------------------
Actor::Actor(StudentWorld* g, int ID, int dX, int dY, Direction d, double s, int de)
	:GraphObject(ID, dX, dY, d, s, de), sGame(g)
{
	lifeStatus = true;
	setVisible(true);
}

bool Actor::isAlive()
{
	return lifeStatus;
}

void Actor::dead()
{
	lifeStatus = false;
}

void Actor::moveObj(int x, int y)
{
	if (x < 0)
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}
	if (x > 60)
	{
		x = 60;
	}
	if (y > 60)
	{
		y = 60;
	}
	GraphObject::moveTo(x, y);
}

StudentWorld* Actor::rWorld()
{
	return sGame;
}

Actor::~Actor()
{
	setVisible(false);
}

//--------------------Earth Class Implementation----------------------------------------------
Earth::Earth(StudentWorld *world, int Xs, int Xy) :Actor(world, TID_EARTH, Xs, Xy, right, 0.25, 3)
{
	setVisible(true);
}

void Earth::doSomething()
{
	//does nothing
}
Earth::~Earth()
{
	setVisible(false);
}
//--------------------Boulder Class Implmentation-------------------------------------------------
Boulder::Boulder(StudentWorld* world, int Xs, int Xy) :Actor(world, TID_BOULDER, Xs, Xy, down, 1.0, 1)
{
	setVisible(true);
	condition = true;
	falling = false;
	ticks = 0;
	rWorld()->removeEarth(Xs, Xy);
}
void Boulder::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	if (condition)
	{
		if (rWorld()->isClearBelow(getX(), getY() - 1))
		{
			return;
		}
		condition = false;
	}
	if (ticks == 30)
	{
		falling = true;
		rWorld()->playSound(SOUND_FALLING_ROCK);
		
	}
	ticks++;
	if (falling)
	{
		shared_ptr<Protestor> P = rWorld()->checkProRange(this, 3);
		if (rWorld()->checkTManRange(getX(), getY(), 3))
		{
			rWorld()->Annoy(100);
		}
		else if (rWorld()->isClearBelow(getX(), getY() - 1) || rWorld()->OccupiedByBoulder(getX(), getY() - 4) || getY() == 0)
		{
			dead();
		}
		else if (P != nullptr)
		{
			P->isAnnoyed(100);
			moveTo(getX(), getY() - 1);
		}
		else
		{
			moveTo(getX(), getY() - 1);
		}		
	}	
}
Boulder::~Boulder()
{
	setVisible(false);
}
//------------------Squirt Class Implementation-----------------------------------------------------
Squirt::Squirt(StudentWorld* world, int Xs, int Xy, Direction d) :Actor(world, TID_WATER_SPURT, Xs, Xy, d, 1.0, 1)
{
	//Intialize other variables
	setVisible(true);
	travelComplete = 0;
}
void Squirt::doSomething()
{
	if (annoyPro()||travelComplete == 4)
	{
		dead();
		return;
	}
	switch (getDirection())
	{
	case right:
		if ((!rWorld()->isClear(getX() + 1, getY())) || (rWorld()->OccupiedByBoulder(getX() + 1, getY())))
		{
			dead();
			return;
		}
		else
		{
			moveTo(getX() + 1, getY());
		}
		break;

	case left:
		if ((!rWorld()->isClear(getX() - 1, getY())) || (rWorld()->OccupiedByBoulder(getX() - 1, getY())))
		{
			dead();
			return;
		}
		else
		{
			moveTo(getX() - 1, getY());
		}
		break;

	case up:
		if ((!rWorld()->isClear(getX(), getY() + 1)) || (rWorld()->OccupiedByBoulder(getX(), getY() + 1)))
		{
			dead();
			return;
		}
		else
		{
			moveTo(getX(), getY() + 1);
		}
		break;

	case down:
		if ((!rWorld()->isClear(getX(), getY() - 1)) || (rWorld()->OccupiedByBoulder(getX(), getY() - 1)))
		{
			dead();
			return;
		}
		else
		{
			moveTo(getX(), getY() - 1);
		}
		break;
	}
	travelComplete++;
}
bool Squirt::annoyPro()
{
	shared_ptr<Protestor> p = rWorld()->checkProRange(this, 3);
	if (p != nullptr)
	{
		p->isAnnoyed(2);
		return true;
	}
	return false;
}
Squirt::~Squirt()
{
	setVisible(false);
}
//-----------------Goodies Class Implmentation ------------------------------------------------------
Goodies::Goodies(StudentWorld* world, int ID, int Xs, int Xy)
	:Actor(world, ID, Xs, Xy, right, 1.0, 2)
{
	setVisible(true);
}
Goodies::~Goodies()
{
	setVisible(false);
}
//----------------- Oil Barrel Implementation ------------------------------------------------------
OilBarrel::OilBarrel(StudentWorld* world, int Xs, int Xy) 
	:Goodies(world, TID_BARREL, Xs, Xy)
{
	//Intialize other variables
	setVisible(false);
	visible = false;
}
void OilBarrel::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	//Once Check Again
	if (visible == false && rWorld()->checkTManRange(getX(), getY(), 4))
	{
		setVisible(true);
		visible = true;
		return;
	}
	if (rWorld()->checkTManRange(getX(), getY(), 3))
	{
		dead();
		rWorld()->playSound(SOUND_FOUND_OIL);
		rWorld()->increaseScore(1000);
		rWorld()->decBarrel();
	}
}
OilBarrel::~OilBarrel()
{
	setVisible(false);
}
//----------------Gold Nuggets Implementation -----------------------------------------------------
GoldNugg::GoldNugg(StudentWorld* world, int Xs, int Xy, bool what)
	:Goodies(world, TID_GOLD, Xs, Xy)
{
	if (what == true)
	{
		setVisible(false);
		pickable = 0;
	}
	else
	{
		setVisible(true);
		pickable = 1;
		ticks = 0;
	}
}
void GoldNugg::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	if (rWorld()->checkTManRange(getX(), getY(), 4))
	{
		setVisible(true);
	}
	if (!pickable && rWorld()->checkTManRange(getX(), getY(), 3))
	{
			dead();
			rWorld()->playSound(SOUND_GOT_GOODIE);
			rWorld()->incr(TID_GOLD, 1, 10);
	}
	else if (pickable == 1)
	{
		if (ticks == 100)
		{
			dead();
			return;
		}
		shared_ptr<Protestor> p = rWorld()->checkProRange(this, 3);
		if (p!=nullptr)
		{
			dead();
			p->pickupGold();
		}
		ticks++;
		//Check for Protestors
	}
}
GoldNugg::~GoldNugg()
{
	setVisible(false);
}
//--------------Sonar Implementation ----------------------------------------------------------------
Sonar::Sonar(StudentWorld* world, int Xs, int Xy)
	:Goodies(world, TID_SONAR, Xs, Xy)
{
	//Intialize other variables
	setVisible(true);
	valid = max(100, static_cast<int>(300 - 10 * rWorld()->getLevel()));
	ticks = 0;
}
void Sonar::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	ticks++;
	if (ticks == valid)
	{
		dead();
	}
	if (rWorld()->checkTManRange(getX(), getY(),3))
	{
		dead();
		rWorld()->playSound(SOUND_GOT_GOODIE);
		rWorld()->incr(TID_SONAR, 1, 75);
	}
}
Sonar::~Sonar()
{
	setVisible(false);
}
//-------------Water Pool Implementation -------------------------------------------------------------
WaterPool::WaterPool(StudentWorld* world, int Xs, int Xy)
	:Goodies(world, TID_WATER_POOL, Xs, Xy)
{
	//Intialize other variables
	setVisible(true);
	valid = max(100, static_cast<int>(300 - 10 * rWorld()->getLevel()));
	ticks = 0;
}
void WaterPool::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	ticks++;
	if (ticks == valid)
	{
		dead();
	}
	if (rWorld()->checkTManRange(getX(), getY(),3))
	{
		dead();
		rWorld()->playSound(SOUND_GOT_GOODIE);
		rWorld()->incr(TID_WATER_POOL, 5, 100);
	
	}
}
WaterPool::~WaterPool()
{
	setVisible(false);
}

//----------------Hero_Villain Implementation---------------------------------------------------
Hero_Villain::Hero_Villain(StudentWorld* w, int IMG_ID, int dX, int dY, Direction d, double size, int depth, int hp)
	:Actor(w, IMG_ID, dX, dY, d, size, depth)
{
	hitPoints = hp;
}

int Hero_Villain::ghp()
{
	return hitPoints;
}

int Hero_Villain::chp(int hp)
{
	hitPoints = hitPoints - hp;
	return hitPoints;
}

Hero_Villain::~Hero_Villain()
{
	setVisible(false);
}

//----------------------TUNNEL MAN IMPLEMENTATION--------------------------------------
TunnelMan::TunnelMan(StudentWorld* w) :Hero_Villain(w, TID_PLAYER, 30, 60, right, 1.0, 0, 10)
{
	wSquirt = 5;
	sonar = 1;
	goldN = 0;
	hitPoints = 10;
}
TunnelMan::~TunnelMan()
{
	setVisible(false);
}
void TunnelMan::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	if (rWorld()->removeEarth(getX(), getY()))
	{
		rWorld()->playSound(SOUND_DIG);
	}
	
	int play;

	if (rWorld()->getKey(play) == true)
	{
		if (play == KEY_PRESS_ESCAPE)
		{
			dead();
			return;
		}
		else if (play == KEY_PRESS_LEFT)
		{
			setDirection(left);
			if(!rWorld()->OccupiedByBoulder(getX()-1,getY()))
			moveObj(getX()-1, getY());
		}
		else if (play == KEY_PRESS_RIGHT)
		{
			setDirection(right);
			if (!rWorld()->OccupiedByBoulder(getX() + 1, getY()))
			moveObj(getX()+1, getY());
		}
		else if (play == KEY_PRESS_UP)
		{
			setDirection(up);
			if (!rWorld()->OccupiedByBoulder(getX(), getY()+1))
			moveObj(getX(), getY()+1);
		}
		else if (play == KEY_PRESS_DOWN)
		{
			setDirection(down);
			if (!rWorld()->OccupiedByBoulder(getX(), getY() - 1))
			moveObj(getX(), getY()-1);
		}
		else if (play == KEY_PRESS_SPACE)
		{
			if (wSquirt > 0)
			{
				fire();
			}
			return;
		}
		else if (play == 'Z' || play == 'z')
		{
			if (sonar > 0)
			{
				sonar--;
				rWorld()->showMe();
			}
		}
		else if (play == KEY_PRESS_TAB)
		{
			if (goldN > 0)
			{
				shared_ptr<Goodies> GN(new GoldNugg(rWorld(), getX(), getY(), false));
				rWorld()->createGoodie(GN);
				goldN--;
			}
		}
	}
}

//--------------------------------------Helper Functions-----------------------------------------
void TunnelMan::fire()
{
		switch (getDirection())
		{
		case left:
			if (rWorld()->isClear(getX() - 4, getY()) && !rWorld()->OccupiedByBoulder(getX() - 4, getY()))
			{
				shared_ptr<Squirt> Sq(new Squirt(rWorld(), getX() - 4, getY(), getDirection()));
				rWorld()->createSquirts(Sq);
			}
			break;

		case right:
			if (rWorld()->isClear(getX() + 4, getY()) && !rWorld()->OccupiedByBoulder(getX() + 4, getY()))
			{
				shared_ptr<Squirt> Sq1(new Squirt(rWorld(), getX() + 4, getY(), getDirection()));
				rWorld()->createSquirts(Sq1);
			}
			break;

		case up:
			if (rWorld()->isClear(getX(), getY() + 4) && !rWorld()->OccupiedByBoulder(getX(), getY() + 4))
			{
				shared_ptr<Squirt> Sq(new Squirt(rWorld(), getX(), getY() + 4, getDirection()));
				rWorld()->createSquirts(Sq);
			}
			break;

		case down:
			if (rWorld()->isClear(getX(), getY() - 4) && !rWorld()->OccupiedByBoulder(getX(), getY() - 4))
			{
				shared_ptr<Squirt> Sq(new Squirt(rWorld(), getX(), getY() - 4, getDirection()));
				rWorld()->createSquirts(Sq);
			}
			break;
		}
	rWorld()->playSound(SOUND_PLAYER_SQUIRT);
	wSquirt--;
}
void TunnelMan::isAnnoyed(int hitP)
{
	int hp = chp(hitP);
	if (hp <= 0)
	{
		dead();
		rWorld()->playSound(SOUND_PLAYER_GIVE_UP);
	}
}
void TunnelMan::inc(int image, int x)
{
	if (image == TID_WATER_POOL)
	{
		wSquirt += x;
	}
	else if (image == TID_SONAR)
	{
		sonar += x;
	}
	else 
	{
		goldN += x;
	}
}

int TunnelMan::returnwSquirt()
{
	return wSquirt;
}
int TunnelMan::returnSonar()
{
	return sonar;
}
int TunnelMan::returngoldN()
{
	return goldN;
}
int TunnelMan::returnHitPoints()
{
	return hitPoints;
}
//--------------------------------Protestor Class Implmentation-----------------------------------------
Protestor::Protestor(StudentWorld* w, int ID, int hp)
	:Hero_Villain(w, ID, 60, 60, left, 1.0, 0, hp)
{
	move = rand() % 53 + 8;
	leaveTheOilField = false;
	wait = std::max(0, 3 - (int)rWorld()->getLevel() / 4);
	noYell = 15;
	turn = 200;
	signal = none;
}
void Protestor::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	if (wait > 0)
	{
		wait--;
		return;
	}
	else
	{
		turn++;
		wait = std::max(0, 3 - (int)rWorld()->getLevel() / 4);
		noYell++;
	}
	if (leaveTheOilField)
	{
		if (getX() == 60 && getY() == 60)
		{
			dead();
			rWorld()->decreasePLives();
			return;
		}
		else
		{
			whereIsExitorSignal(this, 'E');
			return;
		}
	}
	else
	{
		if (rWorld()->checkTManRange(getX(), getY(), 4))
		{
			bool t = false;
			switch (getDirection())
			{
			case left:
				if (rWorld()->checkD(getX(), getY(), 'l'))
					t = true;
			case right:
				if (rWorld()->checkD(getX(), getY(), 'r'))
					t = true;
			case up:
				if (rWorld()->checkD(getX(), getY(), 'u'))
					t = true;
			case down:
				if (rWorld()->checkD(getX(), getY(), 'd'))
					t = true;
			}

			if (t == true)
			{
				if (noYell > 15)
				{
					rWorld()->Annoy(2);
					rWorld()->playSound(SOUND_PROTESTER_YELL);
					noYell = 0;
					return;
				}
				return;
			}
		}
		if (getID() == TID_HARD_CORE_PROTESTER)
		{
			whereIsExitorSignal(this, 'e');
			Direction dir = returnSignal();
			if (dir != none)
			{
				pMove(dir);
				return;
			}
		}
		Direction d = getD();
		if (d != none && isStraight(d) && !(rWorld()->checkTManRange(getX(), getY(), 4)))
		{
			setDirection(d);
			pMove(d);
			move = 0;
			return;
		}
		move--;
		if (move < 0)
		{
			Direction m;
			m = randomD();
			while (true)
			{
				m = randomD();
				if (canMove(getX(), getY(), m))
					break;
			}
			setDirection(m);
			move = rand() % 53 + 8;
		}
		else if (isInterction() && turn > 200)
		{
			pickViable();
			turn = 0;
			move = rand() % 53 + 8;
		}
		pMove(getDirection());
		if (!canMove(getX(), getY(), getDirection()))
		{
			move = 0;
		}
	}
}
void Protestor::isAnnoyed(int hp)
{
	if (leaveTheOilField) return;
	int hitP = chp(hp);
	rWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	freeze();
	if (ghp() <= 0)
	{
		leaveTheOilField = true;
		rWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		wait = 0;
		if (hp == 100)
		{
			rWorld()->increaseScore(500);
		}
		else if (getID() == TID_HARD_CORE_PROTESTER)
		{
			rWorld()->increaseScore(250);
		}
		else
		{
			rWorld()->increaseScore(100);
		}

	}
}
void Protestor::freeze()
{
	wait = max(50, 100 - (int)rWorld()->getLevel() * 10);
}
void Protestor::pickupGold()
{
	rWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	if (getID() == TID_PROTESTER)
	{
		rWorld()->increaseScore(25);
		leaveTheOilField = true;
	}
	else
	{
		rWorld()->increaseScore(50);
		wait= max(50, 100 - int(rWorld()->getLevel()) * 10);
	}
}
GraphObject:: Direction Protestor::getD()
{
	int x = rWorld()->returnTX();
	int y = rWorld()->returnTY();
	if (getY() == y && getX() == x)
	{
		return getDirection();
	}
	if (getX() == x) {
		if (getY() <y)
			return up;
		if (getY() > y)
			return down;
	}
	if (getY() == y) {
		if (getX() > x)
			return left;
		if (getY() < y)
			return right;
	}

	return none;
}
bool Protestor::isStraight(Direction d)
{
	int x = rWorld()->returnTX();
	int y = rWorld()->returnTY();
	switch (d) 
	{
	case left:
		for (int i = getX(); i >= x; i--) {
			if (!rWorld()->isClear(i,getY()) || rWorld()->OccupiedByBoulder(i, getY()))
				return false;
		}
		return true;
		break;
	case right:
		for (int i = getX(); i <= x; i++) {
			if (!rWorld()->isClear(i, getY()) || rWorld()->OccupiedByBoulder(i, getY()))
				return false;
		}
		return true;
		break;
	case up:
		for (int j = getY(); j <= y; j++) {
			if (!rWorld()->isClear(getX(), j) || rWorld()->OccupiedByBoulder(getX(), j))
				return false;
		}
		return true;
		break;
	case down:
		for (int j = getY(); j >= y; j--) {
			if (!rWorld()->isClear(getX(), j) || rWorld()->OccupiedByBoulder(getX(), j))
				return false;
		}
		return true;
		break;
	case none:
		return false;
	}
}

void Protestor::pMove(Direction d)
{
	switch (d) 
	{
	case left:
		if (getDirection() == left) {
			if (getX() == 0) setDirection(right);
			moveTo(getX() - 1, getY());
		}
		else setDirection(left);
		break;
	case right:
		if (getDirection() == right) {
			if (getX() == 60) setDirection(left);
			moveTo(getX() + 1, getY());
		}
		else setDirection(right);
		break;
	case up:
		if (getDirection() == up) {
			if (getY() == 60) setDirection(down);
			moveTo(getX(), getY() + 1);
		}
		else setDirection(up);
		break;
	case down:
		if (getDirection() == down) {
			if (getY() == 0) setDirection(up);
			moveTo(getX(), getY() - 1);
		}
		else setDirection(down);
		break;
	case none: return;
	}
}
GraphObject::Direction Protestor::randomD()
{
	int r;
	r = rand() % 4;
	switch (r) 
	{
	case 0: return left;
	case 1: return  right;
	case 2: return up;
	case 3: return down;
	}
	return none;
}
bool Protestor::canMove(int x, int y, Direction d)
{
	switch (d)
	{
	case left:
		return (x != 0 && rWorld()->isClear(x - 1, y) && !rWorld()->OccupiedByBoulder(x-1, y));
	case right:
		return (x != 60 && rWorld()->isClear(x + 1, y) && !rWorld()->OccupiedByBoulder(x + 1, y));
	case up:
		return (y != 60 && rWorld()->isClear(x, y + 1) && !rWorld()->OccupiedByBoulder(x, y + 1));
	case down:
		return (y != 0 && rWorld()->isClear(x, y - 1) && !rWorld()->OccupiedByBoulder(x, y - 1));
	case none:
		return false;
	}
	return false;
}
bool Protestor::isInterction()
{
	if (getDirection() == up || getDirection() == down)
	{
		return (canMove(getX(), getY(), left) || canMove(getX(), getY(), right));
	}
	else
		return (canMove(getX(), getY(), up) || canMove(getX(), getY(), down));
}
void Protestor::pickViable()
{
	if(getDirection() == up || getDirection() == down) 
	{
		if (!canMove(getX(), getY(), left)) setDirection(right);
		else if (!canMove(getX(), getY(), right)) setDirection(left);
		else {
			switch (rand() % 2) {
			case 0: setDirection(left);
			case 1: setDirection(right);
			}
		}
	}
	else {
	if (!canMove(getX(), getY(), up)) 
		setDirection(down);
	else if (!canMove(getX(), getY(), down)) 
		setDirection(up);
	else {
		switch (rand() % 2) 
		{
		case 0: setDirection(up);
		case 1: setDirection(down);
		}
	}
	}
}
void Protestor::whereIsExitorSignal(Protestor* p, char c)
{
	bool flag = false;
	int HP = 16;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			PMaze[i][j] = 0;
		}
	}
	int x = p->getX();
	int y = p->getY();
	queue<Maze> M;
	if (c == 'E')
	{
		M.push(Maze(60, 60));
		PMaze[60][60] = 1;
		HP = 0;
	}
	else
	{
		
		int X1 = rWorld()->returnTX();
		int Y1 = rWorld()->returnTY();
		M.push(Maze(X1, Y1));
		PMaze[X1][Y1] = 1;
		flag = true;
	}
	while (!M.empty()) 
	{
		Maze g = M.front();
		M.pop();
		int x = g.p;
		int y = g.q;

		
		if (canMove(x, y, GraphObject::left) && PMaze[x - 1][y] == 0) 
		{
			M.push(Maze(x - 1, y));
			PMaze[x - 1][y] = PMaze[x][y] + 1;
		}
		if (canMove(x, y, GraphObject::right) && PMaze[x + 1][y] == 0) {
			M.push(Maze(x + 1, y));
			PMaze[x + 1][y] = PMaze[x][y] + 1;
		}
		if (canMove(x, y, GraphObject::up) && PMaze[x][y + 1] == 0) {
			M.push(Maze(x, y + 1));
			PMaze[x][y + 1] = PMaze[x][y] + 1;
		}
		if (canMove(x, y, GraphObject::down) && PMaze[x][y - 1] == 0) 
		{
			M.push(Maze(x, y - 1));
			PMaze[x][y - 1] = PMaze[x][y] + 1;
		}
	}
	if (PMaze[x][y] <= HP + 1 || HP == 0)
	{
		if (canMove(x, y, left) && PMaze[x - 1][y] < PMaze[x][y])
		{
			p->pMove(left);
			if (flag == true)
			{

				setSignal(left);
				return;

			}
		}
		else if (canMove(x, y, right) && PMaze[x + 1][y] < PMaze[x][y])
		{
			p->pMove(right);
			if (flag == true)
			{
				setSignal(right);
				return;
			}
		}

		else if (canMove(x, y, up) && PMaze[x][y + 1] < PMaze[x][y])
		{
			p->pMove(up);
			if (flag == true)
			{
				setSignal(up);
				return;
			}
		}
		else if (canMove(x, y, down) && PMaze[x][y - 1] < PMaze[x][y])
		{
			p->pMove(down);
			if (flag == true)
			{
				setSignal(down);
				return;
			}
		}
	}
	setSignal(none);
	return;
}
void Protestor::setSignal(Direction d)
{
	signal = d;
}
GraphObject::Direction Protestor::returnSignal()
{
	return signal;
}
//------------------------------Regular Protestor Implmentation----------------------
RegularProtestor::RegularProtestor(StudentWorld* w):Protestor(w,TID_PROTESTER,5)
{
	setVisible(true);
}
//------------------------------HardCore Protestor Implementation
HardCoreProtestor::HardCoreProtestor(StudentWorld* w) : Protestor(w, TID_HARD_CORE_PROTESTER, 20)
{
	setVisible(true);
}
