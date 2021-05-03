#include "StudentWorld.h"
#include <string>
#include<algorithm>
#include<sstream>
#include<iostream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

//------------------------------------Main Functions----------------------
StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
}
int StudentWorld::move()
{
	vector<std::shared_ptr<TunnelMan>> ::iterator it = TPointer.begin();
	updTxt();
	if ((*it)->isAlive())
	{
		(*it)->doSomething();
		Squirts_move_helper();
		Boulders_move_helper();
		Goodie_move_helper();
		Oil_move_helper();
		Protestor_move_helper();
	}
	if (!(*it)->isAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (barrels == 0)
	{
		return GWSTATUS_FINISHED_LEVEL;
	}
	createProtestors();
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::init()
{
	first = true;
	last = 0;
	protestorLives = 0;
	barrels = 0;
	createEarth();
	createTMan();
	
	createBA();
	createOA();
	createGN();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			delete earth_tracker[x][y];
		}
	}
	vector<std::shared_ptr<TunnelMan>>::iterator it;
	for (it = TPointer.begin(); it != TPointer.end();)
	{
		*it = nullptr;

		it = TPointer.erase(it);
	}
	for (auto it = BPointer.begin(); it != BPointer.end(); )
	{
		*it = nullptr;

		it = BPointer.erase(it);
	}
	for (auto it = GPointer.begin(); it != GPointer.end(); )
	{
		*it = nullptr;

		it = GPointer.erase(it);
	}
	for (auto it = BPointer.begin(); it != BPointer.end(); )
	{
		*it = nullptr;

		it = BPointer.erase(it);
	}
	for (auto it = OPointer.begin(); it != OPointer.end(); )
	{
		*it = nullptr;

		it = OPointer.erase(it);
	}
	for (auto it = SPointer.begin(); it != SPointer.end(); )
	{
		*it = nullptr;

		it = SPointer.erase(it);
	}
	for (auto it = PPointer.begin(); it != PPointer.end(); )
	{
		*it = nullptr;

		it = PPointer.erase(it);
	}
}

//--------------------------------Creating Functions-------------------------------------
void StudentWorld::createEarth()
{
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if (x < 30 || x>33 || y < 4)
				earth_tracker[x][y] = new Earth(this, x, y);
		}
	}
}

bool StudentWorld::removeEarth(int X, int Y)
{
	bool go = false;
	for (int i = X; i < X + 4; i++)
		for (int j = Y; j < Y + 4; j++)
		{
			if (earth_tracker[i][j] != nullptr)
			{
				delete earth_tracker[i][j];
				earth_tracker[i][j] = nullptr;
				go = true;
			}
		}
	return go;
}
void StudentWorld::createTMan()
{
	shared_ptr<TunnelMan> TN(new TunnelMan(this));
	TPointer.push_back(std::move(TN));
}

void StudentWorld::createGoodie(std::shared_ptr<Goodies> g)
{
	GPointer.push_back(std::move(g));
}

void StudentWorld::createBoulder(std::shared_ptr<Boulder> b)
{
	BPointer.push_back(std::move(b));
}

void StudentWorld::createSquirts(std::shared_ptr<Squirt> S)
{
	SPointer.push_back(std::move(S));
}

//-----------------------------------other Helper Functions---------------------------
bool StudentWorld::isClear(int gX, int gY)
{
	for (int i = gX; i < gX + 4; i++) {

		for (int j = gY; j < gY + 4; j++) {

			if (earth_tracker[i][j] != nullptr)
				return false;
		}
	}
	return true;
}
bool StudentWorld::isClearBelow(int x, int y)
{
	for (int i = x; i < x + 4; i++)
	{
		if (earth_tracker[i][y] != nullptr)
			return true;
	}
	return false;
}
bool StudentWorld::findR(int x1, int y1, int x2, int y2, int radius)
{
	if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= radius * radius)
		return true;
	else return false;
}

bool StudentWorld::OccupiedByBoulder(int gX, int gY)
{
	vector<std::shared_ptr<Boulder>>::iterator bIt = BPointer.begin();

	while (bIt != BPointer.end())
	{
		if (findR(gX, gY, (*bIt)->getX(), (*bIt)->getY(),3))
			return true;

		bIt++;
	}
	return false;
}
bool StudentWorld::withinRadius(int X, int Y)
{
	vector<std::shared_ptr<Boulder>>::iterator Bit = BPointer.begin();
	vector<std::shared_ptr<Goodies>>::iterator Git = GPointer.begin();
	vector<std::shared_ptr<OilBarrel>>::iterator Oit = OPointer.begin();

	while (Bit != BPointer.end())
	{
		if (findR(X, Y, (*Bit)->getX(), (*Bit)->getY(),6))
		{
			return false;
		}
		Bit++;
	}

	while (Git != GPointer.end())
	{
		if (findR(X, Y, (*Git)->getX(), (*Git)->getY(),6))
		{
			return false;
		}
		Git++;
	}

	while (Oit != OPointer.end())
	{
		if (findR(X, Y, (*Oit)->getX(), (*Oit)->getY(), 6))
		{
			return false;
		}
		Oit++;
	}
	return true;
}

bool StudentWorld::checkTManRange(int X, int Y, int r)
{
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	if (findR(X, Y, (*Tit)->getX(), (*Tit)->getY(), r))
	{
		return true;
	}
	return false;
}

shared_ptr<Protestor> StudentWorld::checkProRange(Actor* b, int r)
{
	vector<shared_ptr<Protestor>>::iterator Pit;
	for (Pit = PPointer.begin(); Pit != PPointer.end(); Pit++)
	{
		if (findR(b->getX(), b->getY(), (*Pit)->getX(), (*Pit)->getY(), r))
		{
			return (*Pit);
		}
	}
	return nullptr;
}
void StudentWorld::showMe()
{
	vector<std::shared_ptr<OilBarrel>>::iterator Oit = OPointer.begin();
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	vector<std::shared_ptr<Goodies>>::iterator Git = GPointer.begin();
	
	
		while (Git != GPointer.end())
		{
			if (findR((*Git)->getX(), (*Git)->getY(), (*Tit)->getX(), (*Tit)->getY(), 12))
			{
				(*Git)->setVisible(true);
			}
			Git++;
		}
		while (Oit != OPointer.end())
		{
			if (findR((*Oit)->getX(), (*Oit)->getY(), (*Tit)->getX(), (*Tit)->getY(), 12))
			{
				(*Oit)->setVisible(true);
			}
			Oit++;
		}
}

void StudentWorld::incr(int image, int x, int score)
{
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	(*Tit)->inc(image, x);
	increaseScore(score);
}

void StudentWorld::decBarrel()
{
	barrels--;
}

void StudentWorld::decreasePLives()
{
	protestorLives--;
}

bool StudentWorld::checkD(int x, int y, char c)
{
	
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	if (c == 'l')
	{
		if ((*Tit)->getX() <= x)
		{
			return true;
		}
		return false;
	}
	else if (c == 'r')
	{
		if ((*Tit)->getX() >= x)
		{
			return true;
		}
		return false;
	}
	else if (c == 'u')
	{
		if ((*Tit)->getY() >= y)
		{
			return true;
		}
		return false;
	}
	else if (c == 'd')
	{
		if ((*Tit)->getY() <= y)
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}
void StudentWorld::Annoy(int hp)
{
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	(*Tit)->isAnnoyed(hp);
}
int StudentWorld::returnTX()
{
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	return (*Tit)->getX();
}
int StudentWorld::returnTY()
{
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	return (*Tit)->getY();
}
//-----------------------------------Helpers For main Functions-------------------------
void StudentWorld::Squirts_move_helper()
{
	//--------------Move Squirts when Required-----------------------------
	if (SPointer.size() == 0)
		return;

	auto it = SPointer.begin();

	while (it != SPointer.end())
	{
		(*it)->doSomething();

		if (!(*it)->isAlive()) 
		{
			*it = nullptr;
			it = SPointer.erase(it);
		}

		else it++;
	}
}

void StudentWorld::Boulders_move_helper()
{
	if (BPointer.size() == 0)
		return;
	auto Bit = BPointer.begin();
	while (Bit != BPointer.end())
	{
		(*Bit)->doSomething();

		if (!(*Bit)->isAlive()) 
		{
			*Bit = nullptr;
			Bit = BPointer.erase(Bit);
		}

		else Bit++;
	}
}
void StudentWorld::Oil_move_helper()
{
	if (OPointer.size() == 0)
		return;
	auto Oit = OPointer.begin();
	while (Oit != OPointer.end())
	{
		(*Oit)->doSomething();

		if (!(*Oit)->isAlive()) 
		{
			*Oit = nullptr;
			Oit = OPointer.erase(Oit);
		}

		else Oit++;
	}
}
void StudentWorld::Goodie_move_helper()
{
	int x, y;
	int G = (int)getLevel() + 300;
	if (int(rand() % G) + 1 == 1) {
		if (int(rand() % 5) + 1 == 1) 
		{
			shared_ptr<Goodies> Good(new Sonar(this, 0, 60));
			createGoodie(std::move(Good));
		}
		else {
			do 
			{
				x = rand() % 60 + 1;
				y = rand() % 60 + 1;
			} while (!isClear(x, y));
			shared_ptr<Goodies> WP(new WaterPool(this, x, y));
			createGoodie(std::move(WP));
		}
	}

	vector<std::shared_ptr<Goodies>>::iterator Git = GPointer.begin();
	while (Git != GPointer.end())
	{

		if (!((*Git)->isAlive())) 
		{
			*Git = nullptr;
			Git = GPointer.erase(Git);
		}

		else 
		{
			(*Git)->doSomething();
			Git++;
		}
	
	}
}
void StudentWorld::Protestor_move_helper()
{
	vector<shared_ptr<Protestor>>::iterator Pit = PPointer.begin();

	while (Pit != PPointer.end()) {

		if (!(*Pit)->isAlive()) 
		{
			Pit = PPointer.erase(Pit);
			protestorLives--;
		}

		else {
			(*Pit)->doSomething();
			Pit++;
		}
	}
}
//-------------------------Init helper Functions------------------------------------------
void StudentWorld::createBA()
{
	//----------------------Create Boulders-------------------------
	int B = std::min(static_cast<int>(getLevel() / 2 + 2), 9);

	int x = rand() % 60 + 1;
	int y = rand() % 36 + 1 + 20;

	for (int i = 0; i < B; i++)
	{
		while (!withinRadius(x, y) || (x > 26 && x < 34 && y>0))
		{
			x = rand() % 60 + 1;
			y = rand() % 36 + 1 + 20;
		}
		shared_ptr<Boulder> Bould(new Boulder(this, x, y));
		createBoulder(std::move(Bould));
	}

	

}
void StudentWorld::createOA()
{
	//--------------------Create Oil Barrels--------------------------
	int G = std::min(static_cast<int>(2 + getLevel()), 21);

	int x = rand() % 60 + 1;
	int y = rand() % 56 + 1;
	barrels = G;
	for (int i = 0; i < G; i++)
	{
		while (!withinRadius(x, y) || (x > 26 && x < 34 && y>0))
		{
			x = rand() % 60 + 1;
			y = rand() % 56 + 1;
		}

		shared_ptr<OilBarrel> ob(new OilBarrel(this, x, y));
		OPointer.push_back(std::move(ob));
	}
}
void StudentWorld::createGN()
{
	int G = std::max(static_cast<int>(5 - (getLevel()/2)), 2);
	int x = rand() % 60 + 1;
	int y = rand() % 56 + 1;
	for (int i = 0; i < G; i++)
	{
		while (!withinRadius(x, y) || (x > 26 && x < 34 && y>0))
		{
			x = rand() % 60 + 1;
			y = rand() % 56 + 1;
		}
		shared_ptr<GoldNugg> gn(new GoldNugg(this, x, y, true));
		GPointer.push_back(std::move(gn));
	}
}
void StudentWorld::createProtestors()
{
	int T = std::max(25, static_cast<int>(200 - getLevel()));
	int P = std::min(15, static_cast<int>(2 + getLevel() * 1.5));
	int probabilityOfHardcore = std::min(90, static_cast<int>(getLevel() * 10 + 30));
	if (first || (last > T && protestorLives < P))
	{
		if (rand() % 100+1 <= probabilityOfHardcore)
		{
			shared_ptr<Protestor> Hp(new HardCoreProtestor(this));
			PPointer.push_back(std::move(Hp));
		}
		else
		{
			shared_ptr<Protestor> Rp(new RegularProtestor(this));
			PPointer.push_back(std::move(Rp));
		}

		last = 0;
		protestorLives++;
		first = false;
	}
	last++;
}
// ----------------------------------Display Helper Funcions --------------------------

string StudentWorld::dTxt(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels)
{
	std::stringstream s;

	s.fill('0');
	s << "Scr: ";
	s << setw(6) << score;

	s.fill(' ');
	s << " Lvl: ";
	s << setw(2) << level;

	s << " Lives: ";
	s << setw(1) << lives;

	s << "  Hlth: ";
	s << setw(3) << health * 10;
	s << '%';

	s << "  Wtr: ";
	s << setw(2) << squirts;

	s << "  Gld: ";
	s << setw(2) << gold;

	s << "  Sonar: ";
	s << setw(2) << sonar;

	s << "  Oil Left: ";
	s << setw(2) << barrels;

	return s.str();
}

void StudentWorld::updTxt()
{
	vector<std::shared_ptr<TunnelMan>>::iterator Tit = TPointer.begin();
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = (*Tit)->ghp();
	int squirts = (*Tit)->returnwSquirt();
	int gold = (*Tit)->returngoldN();
	int sonar = (*Tit)->returnSonar();
	string s = dTxt(score, level, lives, health, squirts, gold, sonar, barrels);
	setGameStatText(s);

}
//-----------------------------------Destructor----------------------------------------
StudentWorld::~StudentWorld()
{
	cleanUp();
}
