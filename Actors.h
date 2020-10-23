#ifndef ACTORS_INCLUDED
#define ACTORS_INCLUDED

#include<iostream>
#include<string>
#include<vector>
#include"GameObject.h"


using namespace std;

class Dungeon;

class Actor
{
public:
	Actor(Dungeon* dungeon, char symbol, Weapon* weapon, int r, int c, int hitPoints, int armor, int strength, int dexterity, bool isDead, string name);

	void setPos(int r, int c);

	int  row() const { return m_row; }
	int  col() const { return m_col; }

	string getName() const { return m_name; }
	bool isDead() const { return m_dead; }
	void setDead();

	char getSymbol() const { return m_symbol; }


	int getHitPoint() const { return m_hitPoints; }
	void setHitPoint(int x);

	int getMaxHitPoint() const { return maxHitPoint; }
	void setMaxHitPoint(int x);

	int getArmor() const { return m_armor; }
	void setArmor(int x);

	int getStrength() const { return m_strength; }
	void setStrength(int x);

	int getDexterity() const { return m_dexterity; }
	void setDexterity(int x);

	int getSleepTime() const { return m_sleepTime; }
	void setSleepTime(int x);
	void decreaseSleep();

	Weapon* getWeapon() const { return m_weapon; }
	void setWeapon(Weapon* w) { m_weapon = w; }

	//Dungeon* getDungeon()  { return dungeon; }

	bool canMoveTo(int r, int c);
	void moveTo(int r, int c);

	void attack(Actor* attacker, Actor* defender, Weapon* Weapon);

	void addHealth();

	virtual ~Actor();

private:
	string m_name;
	char m_symbol;
	int    m_row;
	int    m_col;
	bool   m_dead;
	int maxHitPoint = 99;
	int m_hitPoints;
	int m_armor;
	int m_strength;
	int m_dexterity;
	int m_sleepTime = 0;
	Weapon* m_weapon;
	//Dungeon* dungeon;
};

class Player :public Actor
{
public:
	Player(Dungeon* dun);

	void setWin();
	bool hasWon() { return isWin; }
	void appendInventory(GameObject* o);

	vector<GameObject*> getInventory() const {return inventory;}
	void printInventory();

	string wield();
	string read(char ch);
	void teleport();

	void cheat()
	{
		setStrength(9);
		setHitPoint(50);
	}

	~Player()
	{

		for (int i = 0; i != inventory.size(); i++)
		{
			delete inventory[i];
		}
	}
private:
	vector <GameObject*> inventory;
	bool isWin;
};

class Monster : public Actor
{
public:
	Monster(Dungeon* dun,char symbol, Weapon* weapon, int r, int c, int hitPoints, int armor, int strength, int dexterity, bool isDead, string name)
		:Actor(dun, symbol, weapon, r, c, hitPoints, armor, strength, dexterity, isDead, name) {}
	//int stepsToPlayerNoWall(int mR, int mC, int pR, int pC);
	//virtual void drop() = 0;
	//virtual void myturn() = 0;
	~Monster() {}
};


class Bogeyman : public Monster //level 2 or deeper
{
public:
	Bogeyman(Dungeon* dun,int r, int c);

	//virtual void drop(); //1/10 chance to drop a magic axe **if there is nothing at that position
	//smells the player(5 steps or less) //ignore wall//
	//virtual void myturn(int pr, int pc);
	~Bogeyman() {}
};

class Snakewoman : public Monster //any level
{
public:
	Snakewoman(Dungeon* dun, int r, int c);
	//virtual void drop(); //1/3 chance to drop magic fangs of sleep ** if there is nothing at there
	//smells the player(3 steps or less)
	//virtual void myturn();

	~Snakewoman() {}
};

class Dragon : public Monster //level 3 or deeper
{
public:
	Dragon(Dungeon* dun, int r, int c);
	//virtual void drop(); //100% will drop a scroll
	//1/10 chance to regain hit point before each turn (till max!!)
	//virtual void myturn();

	~Dragon() {}
};

class Goblin :public Monster
{
public:
	Goblin(Dungeon* dun, int r, int c);

	//virtual void drop(); //1/3 chance to drop either a magic axe or a magic fangs
	//smells the player 15 steps(if reachable), make an optimal move 
	//virtual void myturn();

	//int moveOptimal(int gR, int gC, int pR, int pC, int distance);
	~Goblin() {}

};

#endif
