#include"Dungeon.h"
#include"utilities.h"
#include<cmath>

Dungeon::Dungeon(int goblinSmellDistance)
{
	m_level = 0;
	buildWall();

	//add player
	addPlayer();

	//add monsters
	addMonsters();

	//add gameObjects
	addObjects();

	//add stariway
	addStairway();

	//add Goldem idol
	addGoldenIdol();

	setSmellDistance(goblinSmellDistance);

}

void Dungeon::nextLevel()
{
	m_level++;
	for (int i = 0; i != m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}
	for (int i = 0; i != m_monsters.size(); i++)
	{
		delete m_monsters[i];
	}
	m_gameObjects.clear();
	m_monsters.clear();

	//reset wall
	buildWall();
	//reset player's postition
	bool findPos = false;
	while (!findPos)
	{
		int row = randInt(0, 16);
		int col = randInt(0, 69);
		if (!Wall[row][col] ) //not wall
		{
			findPos = true;
			player->setPos(row, col);
		}
	}
	//add monsters
	addMonsters();

	//add gameObjects
	addObjects();

	//add stariway
	addStairway();

	//add Goldem idol
	addGoldenIdol();
}

void Dungeon::pickUp()
{
	if (getPlayer()->row() == getGoldenR() && getPlayer()->col() == getGoldenC())
	{
		setMessage("You pick up the golden idol. Congratulation, you won!");
		getPlayer()->setWin();
	}
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->row() == getPlayer()->row() && m_gameObjects[i]->col() == getPlayer()->col())
		{
			getPlayer()->appendInventory(m_gameObjects[i]);
			setMessage("You pick up" + m_gameObjects[i]->getName() + ".");
			eraseGameObject(i);
		}
	}
}

void Dungeon::read()
{
	clearScreen();
	getPlayer()->printInventory();
	char ch = getCharacter();
	string result = getPlayer()->read(ch);
	if (result == "scroll of strength")
	{
		getPlayer()->setStrength(getPlayer()->getStrength() + randInt(1, 3));
		setMessage("You read the scroll called " + result);
	}
	else if (result == "scroll of enhance armor")
	{
		getPlayer()->setArmor(getPlayer()->getArmor() + randInt(1, 3));
		setMessage("You read the scroll called " + result);
	}
	else if (result == "scroll of enhance health")
	{
		getPlayer()->setMaxHitPoint(getPlayer()->getMaxHitPoint() + randInt(3, 8));
		setMessage("You read the scroll called " + result);
	}
	else if (result == "scroll of enhance dexterity")
	{
		getPlayer()->setDexterity(getPlayer()->getDexterity() + 1);
		setMessage("You read the scroll called " + result);
	}
	else if(result == ""){ }
	else
	{
		setMessage("You can't read a" + result);
	}
}

bool Dungeon::isWall(int r, int c)
{
	return Wall[r][c];
}

bool Dungeon::isMonster(int r, int c)
{
	vector <Monster*>::iterator it = m_monsters.begin();
	while (it != m_monsters.end())
	{
		if ((*it)->row() == r && (*it)->col() == c)
			return true;
		it++;
	}
	return false;
}

bool Dungeon::isObject(int r, int c)
{
	vector<GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it)->row() == r && (*it)->col() == c)
			return true;
		it++;
	}
	return false;
}

bool Dungeon::isPlayer(int r, int c)
{
	return (r == player->row() && c == player->col());
}

void Dungeon::movePlayerTo(int r, int c)
{
	if (!isMonster(r, c) && !isWall(r,c))
	{
		getPlayer()->setPos(r, c);
	}
	else
	{
		if (isMonster(r, c))
		{
			for (int i = 0; i < m_monsters.size(); i++)
			{
				if (m_monsters[i]->row() == r && m_monsters[i]->col() == c)
				{
					attack(player, m_monsters[i]); //append message (might set monster as dead)
					if (m_monsters[i]->getHitPoint()<=0)
					{
						if (!isObject(r, c))
						{
							if (m_monsters[i]->getSymbol() == 'B')
							{
								if (randInt(1, 10) == 1)
								{
									GameObject* g = new MagicAxe(r, c);
									appendGameObject(g);
								}
							}
							if (m_monsters[i]->getSymbol() == 'S')
							{
								if (randInt(1, 3) == 1)
								{
									GameObject* g = new MagicFangs(r, c);
									appendGameObject(g);
								}
							}
							if (m_monsters[i]->getSymbol() == 'D')
							{
								//scroll of some kind
								int type = randInt(1, 5);
								GameObject* g;
								switch (type)
								{
								case 1:
									g = new Teleportation(r, c);
									appendGameObject(g);
									break;
								case 2:
									g = new EnhanceArmor(r, c);
									appendGameObject(g);
									break;
								case 3:
									g = new EnhanceHealth(r, c);
									appendGameObject(g);
									break;
								case 4:
									g = new EnhanceDexterity(r, c);
									appendGameObject(g);
									break;
								case 5:
									g = new Strength(r, c);
									appendGameObject(g);
									break;
								}
							}
							if (m_monsters[i]->getSymbol() == 'G')
							{
								if (randInt(1, 3) == 1)
								{
									if (randInt(1, 2) == 1)
									{
										GameObject* g = new MagicAxe(r, c);
										appendGameObject(g);
									}
									else
									{
										GameObject* g = new MagicFangs(r, c);
										appendGameObject(g);
									}
								}
						
							}
						}
						delete m_monsters[i];
						eraseMonster(i);
					}
				}
			}
		}
	}
}

bool Dungeon::isNextTo(Actor* a, Actor* b)
{
	int ar = a->row();
	int ac = a->col();
	int br = b->row();
	int bc = b->col();
	return((abs(ar - br) + abs(ac - bc)) == 1);
}

void Dungeon::setGrid(int r, int c, char ch)
{
	myGrid[r][c] = ch; //EXCEPTION--write access violation 
}

void Dungeon::appendGameObject(GameObject* g)
{
	m_gameObjects.push_back(g);
}

void Dungeon::eraseGameObject(int i)
{
	m_gameObjects.erase(m_gameObjects.begin() + i);
}

void Dungeon::appendMonster(Monster* m)
{
	m_monsters.push_back(m);
}

void Dungeon::eraseMonster(int i)
{
	m_monsters.erase(m_monsters.begin() + i);
}

///
void Dungeon::buildWall()
{
	//outer boundary
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 70; j++)
		{
			Wall[i][j] = true;
		}	
	}


	//interior
	int width, height, row, col;
	//left one room up
	width = randInt(7, 15);
	height = randInt(5, 7);
	row = randInt(1, 2);
	col = randInt(1, 3);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Wall[row +j][col+i] = false;
		}
	}
	//its corridor
	width = randInt(7, 20);
	height = randInt(1,1);
	row = randInt(7,8);
	col = randInt(8,17);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Wall[row + j][col + i] = false;
		}
	}

	//left one room down
	width = randInt(7, 15);
	height = randInt(7,10);
	row = randInt(1, 2);
	col = randInt(1, 3);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Wall[7+row + j][col + i] = false;
		}
	}

	//left two room up
	width = randInt(12, 15);
	height = randInt(5, 7);
	row = randInt(1, 3);
	col = randInt(25,28);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Wall[row + j][col + i] = false;
		}
	}
	//left two room down
	width = randInt(14,16);
	height = randInt(5, 7);
	row = randInt(1, 2);
	col = randInt(25,28);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Wall[8 + row + j][col + i] = false;
		}
	}

	//right two room up
	width = randInt(7, 15);
	height = randInt(5, 9);
	row = randInt(1, 2);
	col = randInt(50,52);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Wall[row + j][col + i] = false;
		}
	}
	//right two room down
	width = randInt(15, 18);
	height = randInt(5, 7);
	row = randInt(1, 2);
	col = randInt(50,52);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Wall[8 + row + j][col + i] = false;
		}
	}
	//outer boundary
	for (int i = 0; i != 18; i++)
	{
		Wall[i][0] = true;
		Wall[i][69] = true;
	}
	for (int j = 0; j != 70; j++)
	{
		Wall[0][j] = true;
		Wall[17][j] = true;
	}
}
///

void Dungeon::setPlayerPos(int r, int c)
{
	player->setPos(r, c);
}

void Dungeon::addPlayer()
{
	player = new Player(this);
	while (1)
	{
		int row = randInt(0, 16);
		int col = randInt(0, 69);
		if (!Wall[row][col] && !isMonster(row,col)) //not wall
		{
			player->setPos(row, col);
			return;
		}
	}
}

void Dungeon::addMonsters()
{
	int level = m_level;
	int monsters = 1;
		//randInt(2, 5 * (level + 1) + 1);
	int type = 0;
	int row, col;
	for (int i = 0; i != monsters; i++)
	{
		if (level < 2)
		{
			type = randInt(1, 1);
		}
		if (level == 3)
		{
			type = randInt(0, 2);
		}
		if (level > 3)
		{
			type = randInt(0, 3);
		}

		while (1)
		{
			row = randInt(0, 16);
			col = randInt(0, 69);
			if (!Wall[row][col] && !isPlayer(row,col) && !isMonster(row,col)) //not wall, not player
			{
				break;
			}
		}

		switch (type)
		{
		case 0:
			appendMonster(new Snakewoman(this,row, col));
			break;
		case 1:
			appendMonster(new Goblin(this,row, col));
			break;
		case 2:
			appendMonster(new Bogeyman(this,row, col));
			break;
		case 3:
			appendMonster(new Dragon(this,row, col));
			break;
		}
	}

}

void Dungeon::addObjects()
{
	int objects = randInt(2, 3);
	int category,row,col;
	for (int i = 0; i != objects; i++)
	{
		while (1)
		{
			row = randInt(0, 16);
			col = randInt(0, 69);
			if (!Wall[row][col] && !isPlayer(row, col)&& !isMonster(row,col) && !isObject(row,col)) //not wall, not player,monster or object
			{
				break;
			}
		}
		category = randInt(0, 1);
		if (category == 0) //add a weapon
		{
			int type = randInt(0, 2);
			switch (type)
			{
			case 0:
				appendGameObject(new Mace(row, col));
				break;
			case 1:
				appendGameObject(new ShortSword(row, col));
				break;
			case 2:
				appendGameObject(new LongSword(row, col));
				break;
			}
		}
		if (category == 1)
		{
			int type = randInt(0, 3);
			switch (type)
			{
			case 0:
				appendGameObject(new Strength(row, col));
				break;
			case 1:
				appendGameObject(new EnhanceArmor(row, col));
				break;
			case 2:
				appendGameObject(new EnhanceHealth(row, col));
				break;
			case 3:
				appendGameObject(new EnhanceDexterity(row, col));
				break;
			}
		}
	}

}

void Dungeon::addStairway()
{
	if (m_level == 4){}
	else
	{
		int row, col;
		while (1)
		{
			row = randInt(0, 16);
			col = randInt(0, 69);
			if (!Wall[row][col] && !isMonster(row, col) &&!isObject(row,col)) //not wall, not player
			{
				break;
			}
		}
		stairwayRow = row;
		stairwayCol = col;
	}
}

void Dungeon::addGoldenIdol()
{
	if (m_level == 4)
	{
		int row, col;
		while (1)
		{
			row = randInt(0, 16);
			col = randInt(0, 69);
			if (!Wall[row][col] && !isMonster(row, col) && !isObject(row,col)) //not wall, not player,monster,or object
			{
				break;
			}
		}
		GoldenIdolRow = row;
		GoldenIdolCol = col;
	}
}

void Dungeon::display()
{

	//put all symbols onto the grid
	//lay out the wall
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 70; j++)
		{
			setGrid(i, j, ' ');
			if (Wall[i][j])
			{
				setGrid(i, j, '#');
			}
		}
	}

	//put the monsters in
	if (!m_monsters.empty())
	{
		vector<Monster*>::iterator it = m_monsters.begin();
		while (it != m_monsters.end())
		{
			setGrid((*it)->row(), (*it)->col(), (*it)->getSymbol());
			it++;
		}
	}

	//put the objects in
	if (!m_gameObjects.empty())
	{
		vector<GameObject*>::iterator it = m_gameObjects.begin();
		while (it != m_gameObjects.end())
		{
			setGrid((*it)->row(), (*it)->col(), (*it)->getSymbol());
			it++;
		}
	}

	//put the stairway
	if (stairwayRow != -1 && stairwayCol != -1)
	{
		setGrid(stairwayRow, stairwayCol, '<');
	}
	//put the golden idol
	if (GoldenIdolRow != -1 && GoldenIdolCol != -1)
	{
		setGrid(GoldenIdolRow, GoldenIdolCol, '&');
	}

	//put the player in
	setGrid(player->row(), player->col(), '@');

	//print the grid
	clearScreen();
	for (int r = 0; r != 18; r++)
	{
		for (int c = 0; c != 70; c++)
			cout << myGrid[r][c];
		cout << endl;
	}
	//print stats
	cout << "Dungeon Level: " << m_level << ", "
		<< "Hit points: " << getPlayer()->getHitPoint() << ", "
		<< "Armor: " << getPlayer()->getArmor() << ", "
		<< "Strength: " << getPlayer()->getStrength() << ", "
		<< "Dexerity: " << getPlayer()->getDexterity() << endl;
	printMessage();
	cout << "player is at (" << player->row() << ',' << player->col() << ')' << endl;
	//PrintAllMonsters();
	//PrintAllObjects();
}

void Dungeon::setMessage(string str)
{
	message = str;
}

void Dungeon::appendMessage(string str)
{
	message += str;
}

void Dungeon::clearMessage()
{
	message = "";
}

///
void Dungeon::monstersTurn()
{
	char gWall[18][70] = { '.' };
	vector <Monster*>::iterator it = m_monsters.begin();
	while (it != m_monsters.end())
	{
		int r = (*it)->row();
		int c = (*it)->col();
		int pr = getPlayer()->row();
		int pc = getPlayer()->col();

		//if it is right next to the player...attack!
		if ((*it)->getSleepTime() > 0) 
		{
			(*it)->decreaseSleep();
		}
		else if (isNextTo(player, *it))
		{
			attack(*it, player);
		}
		else
		{
			if ((*it)->getSymbol() == 'B')
			{
				//smell
				if ((abs(pr - r) + abs(pc - c)) <= 5)
				{
					if (pr > r && !isWall(r + 1, c) && !isMonster(r + 1, c) && !isPlayer(r + 1, c))
					{

						(*it)->setPos(r + 1, c);
					}
					else if (pr < r && !isWall(r - 1, c) && !isMonster(r - 1, c) && !isPlayer(r - 1, c))
					{
						(*it)->setPos(r - 1, c);
					}
					else if (pc < c && !isWall(r, c - 1) && !isMonster(r, c - 1) && !isPlayer(r, c - 1))
					{
						(*it)->setPos(r, c - 1);
					}
					else if (pc > c && !isWall(r, c + 1) && !isMonster(r, c + 1) && !isPlayer(r, c + 1))
					{
						(*it)->setPos(r, c + 1);
					}
					else
					{
						(*it)->setPos(r, c);
					}
				}
			}
			if ((*it)->getSymbol() == 'S')
			{
				//smell
				if ((abs(pr - r) + abs(pc - c)) <= 3)
				{
					if (pr > r && !isWall(r + 1, c) && !isMonster(r + 1, c) && !isPlayer(r + 1, c))
					{

						(*it)->setPos(r + 1, c);
					}
					else if (pr < r && !isWall(r - 1, c) && !isMonster(r - 1, c) && !isPlayer(r - 1, c))
					{
						(*it)->setPos(r - 1, c);
					}
					else if (pc < c && !isWall(r, c - 1) && !isMonster(r, c - 1) && !isPlayer(r, c - 1))
					{
						(*it)->setPos(r, c - 1);
					}
					else if (pc > c && !isWall(r, c + 1) && !isMonster(r, c + 1) && !isPlayer(r, c + 1))
					{
						(*it)->setPos(r, c + 1);
					}
					else
					{
						(*it)->setPos(r, c);
					}
				}
			}
			if ((*it)->getSymbol() == 'D')
			{
				//regain hitpoint
				if (randInt(1, 10) == 1)
				{
					(*it)->setHitPoint((*it)->getHitPoint() + 1);
				}
			}
			if ((*it)->getSymbol() == 'G')
			{
				appendMessage(" looping goblin ");
				///
				//(*it)->setPos(r + 1, c);
				for (int j = 0; j != 70; j++)
				{
					for (int i = 0; i != 18; i++)
					{
						if (isWall(i, j) || isMonster(i, j))
						{
							gWall[i][j] = 'X';
						}
						else {
							gWall[i][j] = '.';
						}
					}
				} //setting up the Wall
				std::string w(1, gWall[3][3]);
				appendMessage(w);
				appendMessage(std::to_string(getSmellDistance()));
				std::pair<int, char>result = goblin(gWall, r, c, pr, pc, getSmellDistance());
				std::string s(1, result.second);
				appendMessage("result is "+ std::to_string(result.first)+s);
				switch (result.second) {
				case 'S':
					(*it)->setPos(r + 1, c);
					break;
				case 'N':
					(*it)->setPos(r - 1, c);
					break;
				case 'W':
					(*it)->setPos(r, c - 1);
					break;
				case 'E':
					(*it)->setPos(r, c + 1);
					break;
				default:
					(*it)->setPos(r, c);
				}
			}
		}

		it++;
	}
}



void Dungeon::attack(Actor* attacker, Actor* defender)
{
	int attackerPoints = attacker->getDexterity() + attacker->getWeapon()->getDexerityBonus();
	int defenderPoints = defender->getDexterity() + defender->getArmor();
	if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)) //make the attack, hit 
	{
		int damagePoints = randInt(0, attacker->getStrength() + attacker->getWeapon()->getDamage() - 1);
		defender->setHitPoint(defender->getHitPoint() - damagePoints);

		if (defender->getHitPoint()<=0)
		{
			defender->setDead();
			appendMessage('\n'+ attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName()
			+ " " + "at the " + defender->getName() + " dealing a final blow.");
		}
		else
		{
			if (attacker->getWeapon()->getName() == "magic fangs of sleep" && randInt(1, 5) == 1)
			{
				defender->setSleepTime(randInt(2, 6));
				appendMessage('\n' + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName()
					+ " " + "at the " + defender->getName() + " and hits, putting "+ defender->getName()+" to sleep.");
			}
			else
			{
				appendMessage('\n' + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName()
				+ " " + "at the " + defender->getName() + " and hits.");
			}

		}
	}
	else //misses
	{
		appendMessage('\n'+ attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName()
			+ " " + "at the " + defender->getName() + " and misses.");
	}
}

Dungeon::~Dungeon()
{
	delete player;
	for (int i = 0; i != m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}
	for (int i = 0; i != m_monsters.size(); i++)
	{
		delete m_monsters[i];
	}

}



//helper
void Dungeon::PrintAllMonsters()
{
	if (m_monsters.empty())
	{
		cout << "there is no monsters on this level" << endl;
	}
	else
	{
		vector<Monster*>::iterator it = m_monsters.begin();
		while (it != m_monsters.end())
		{
			cout << (*it)->getName() <<" at "<<'('<<(*it)->row()<<','<<(*it)->col()<<')'<< ", isNextTo="<<isNextTo(player,*it)<<", hitPoint= "
				<<(*it)->getHitPoint()<< endl;
			it++;
		}
	}
}

void Dungeon::PrintAllObjects()
{
	if (m_gameObjects.empty())
	{
		cout << "there is no object on this level" << endl;
	}
	else
	{
		vector<GameObject*>::iterator it = m_gameObjects.begin();
		while (it != m_gameObjects.end())
		{
			cout << (*it)->getName()<<" at " << '(' << (*it)->row() << ',' << (*it)->col() << ')' << endl;
			it++;
		}
	}
}

Monster* Dungeon::firstM()
{
	return m_monsters[0];
}

//NEED EXAMINATION LATER
std::pair<int, char> Dungeon::goblin(char maze[][70], int sr, int sc, int er, int ec, int smellDistance) //put in the array position (everything -1)
{
	maze[sr][sc] = 'X';
	char newMaze[18][70];
	//make a copy of the current maze
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 70; j++) {
			newMaze[i][j] = maze[i][j];
		}
	}
	std::pair<int, char> stepsN(smellDistance + 1, 'N'), stepsS(smellDistance + 1, 'S'), stepsE(smellDistance + 1, 'E'), stepsW(smellDistance + 1, 'W');
	int x = 0;
	if (sr == er && sc == ec) {
		std::pair<int, char> noStep(0, 'X');
		return noStep;
	}
	if (maze[sr + 1][sc] == '.') //if you can move to the SOUTH and is unvisited
	{
		x = goblin(newMaze, sr + 1, sc, er, ec, smellDistance - 1).first;
		if (x > -1) {
			stepsS.first = x + 1;
		}
	}
	if (maze[sr][sc - 1] == '.') //if you can move to the WEST...
	{
		x = goblin(newMaze, sr, sc - 1, er, ec, smellDistance - 1).first;
		if (x > -1) {
			stepsW.first = x + 1;
		}
	}
	if (maze[sr - 1][sc] == '.')//if you can move to NORTH...
	{
		x = goblin(newMaze, sr - 1, sc, er, ec, smellDistance - 1).first;
		if (x > -1) {
			stepsN.first = x + 1;
		}
	}
	if (maze[sr][sc + 1] == '.') //if you can move to EAST	
	{
		x = goblin(newMaze, sr, sc + 1, er, ec, smellDistance - 1).first;
		if (x > -1) {
			stepsE.first = x + 1;
		}
	}

	//return the rest...
	/*cout << stepsN.first << ' ' << stepsS.first << " " << stepsW.first << " " << stepsE.first << " " << endl;*/
	std::pair<int, char> steps = stepsN.first <= stepsS.first ? stepsN : stepsS;
	if (steps.first > stepsW.first) {
		steps = stepsW;
	}
	if (steps.first > stepsE.first) {
		steps = stepsE;
	}
	if (steps.first > smellDistance) {
		steps.first = -1;
		steps.second = 'X';
	}
	return steps;//returns the steps, -1 if its not within smellDistance
}