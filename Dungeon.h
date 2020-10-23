#ifndef DUNGEON_INCLUDED
#define DUNGEON_INCLUDED

#include<vector>
#include<iostream>
#include"utilities.h"
#include"GameObject.h"
#include"Actors.h"

using namespace std;


class Dungeon
{
public:
	Dungeon(int goblinSmellDistance);

	void nextLevel();
	void setGrid(int r, int c, char ch);

	bool isMonster(int r, int c);
	bool isWall(int r, int c);
	bool isObject(int r, int c);
	bool isPlayer(int r, int c);
	void movePlayerTo(int r, int c);
	bool isNextTo(Actor* a, Actor* b);

	void appendGameObject(GameObject* g);
	void eraseGameObject(int i);

	void appendMonster(Monster* m);
	void eraseMonster(int i);

	void pickUp();
	void read();
	void display();
	void monstersTurn();
	void GoblinMove(Monster* g);
	void attack(Actor* attacker, Actor* defender);

	//helper functions
	bool canBuildEmptyBlock(int startR, int startC, int width, int height);
	bool buildEmptyBlock(int& startR, int& startC, int& width, int& height);



	//////New Floor
	void buildWall();
	void addMonsters();
	void addObjects();
	void addStairway();
	void addGoldenIdol();

	void addPlayer();
	void setPlayerPos(int r, int c);
	Player* getPlayer()
	{
		return player;
	}

	
	int getGoldenR() const
	{
		return GoldenIdolRow;
	}
	int getGoldenC() const
	{
		return GoldenIdolCol;
	}
	int getStairR() const
	{
		return stairwayRow;
	}
	int getStairC() const
	{
		return stairwayCol;
	}
	bool isOnStair()
	{
		return (stairwayRow == getPlayer()->row() && stairwayCol == getPlayer()->col());
	}
	char getCell(int r, int c) const
	{
		return myGrid[r][c];
	}
	int getLevel() const { return m_level; }


	void clearMessage();
	void printMessage() const
	{
		cout << message << endl;
	}
	void setMessage(string str);
	void appendMessage(string str);
	
	void setSmellDistance(int s) {
		smellDistance = s;
	}
	int getSmellDistance() {
		return smellDistance;
	}

	~Dungeon();

	//helper
	void PrintAllMonsters();
	void PrintAllObjects();
	Monster* firstM();
	std::pair<int, char> goblin(char maze[][70], int sr, int sc, int er, int ec, int smellDistance);

private:
	char myGrid[18][70];
	bool Wall[18][70] = { 1 };
	Player* player;
	vector<GameObject*> m_gameObjects;
	vector<Monster*> m_monsters;
	int GoldenIdolRow = -1;
	int GoldenIdolCol = -1;
	int stairwayRow = -1;
	int stairwayCol = -1;
	string message;
	int m_level;
	int smellDistance=-1;
};



#endif

