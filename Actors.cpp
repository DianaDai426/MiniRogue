#include"Actors.h"
#include"utilities.h"

Actor::Actor(Dungeon* dun, char symbol, Weapon* weapon, int r, int c, int hitPoints, int armor, int strength, int dexterity, bool isDead, string name)
{
	dungeon = dun;
	m_row = r;
	m_col = c;
	m_name = name;
	m_symbol = symbol;
	m_weapon = weapon;
	m_sleepTime = 0;
	m_dead = false;
	m_hitPoints = hitPoints;
	m_armor = armor;
	m_strength = strength;
	m_dexterity = dexterity;
}

void Actor::setPos(int r, int c)
{ 
	m_row = r;
	m_col = c; 
}

void Actor::setDead()
{
	m_dead = true;
}

void Actor::setMaxHitPoint(int x)
{
	maxHitPoint = x;
}

void Actor::setHitPoint(int x)
{
	m_hitPoints = x;
	if (x <= 0)
	{
		setDead();
	}
	if (x > maxHitPoint)
	{
		m_hitPoints = maxHitPoint;
	}

}

void Actor::setArmor(int x)
{
	if (x > 99)
	{
		m_armor = 99;
	}
	else
	{
		m_armor = x;
	}
}

void Actor::setStrength(int x)
{
	if (x > 99)
	{
		m_strength = 99;
	}
	else
	{
		m_strength = x;
	}
}

void Actor::setDexterity(int x)
{
	if (x > 99)
	{
		m_dexterity = 99;
	}
	else
	{
		m_dexterity = x;
	}
}

void Actor::setSleepTime(int x)
{
	if (x > 9)
	{
		m_sleepTime = 9;
	}
	else
	{
		if (x > m_sleepTime)
		{
			m_sleepTime = x;
		}
	}
}
void Actor::decreaseSleep()
{
	if (m_sleepTime > 0)
	{
		m_sleepTime--;
	}
}


Actor::~Actor()
{
	delete m_weapon;
	dungeon = NULL;
}



///////////////////////

Player::Player(Dungeon* dun) :Actor(dun,'@', new ShortSword(-1,-1), -1, -1, 100, 2, 2, 2, false, "Player")
{
	isWin = false;
	appendInventory(getWeapon());
}

void Player::setWin()
{
	isWin = true;
}

void Player::appendInventory(GameObject* o)
{
	if (inventory.size() <= 25)
	{
		inventory.push_back(o);
	}

}
void Player::printInventory()
{
	char ch = 'a';
	vector<GameObject*>::iterator it = inventory.begin();
	while (it != inventory.end())
	{
		cout << ch << '.' << (*it)->getName() << endl;
		ch++;
		it++;
	}
}



string Player::wield()
{
	clearScreen();
	printInventory();
	char ch = getCharacter();
	char c = 'a';
	for (int i = 0; i != inventory.size(); i++)
	{
		if (ch == c)
		{
			Weapon* w = dynamic_cast<Weapon*>(inventory[i]);
			if (w != nullptr)
			{
				setWeapon(w);
				return ("You are wielding " + inventory[i]->getName());
			}
			else
			{
				return ("You can't wield " + inventory[i]->getName());
			}

		}
		c++;
	}
	return "";
}

string Player::read(char ch)
{
	char c = 'a';
	for (int i = 0; i != inventory.size(); i++)
	{
		if (ch == c)
		{
			Scroll* w = dynamic_cast<Scroll*>(inventory[i]);
			string name = inventory[i]->getName();
			if (w != nullptr)
			{
				//do the effect 
				
				delete inventory[i];
				inventory.erase(inventory.begin() + i);
				return (name);
			}
			else
			{
				return (name);
			}

		}
		c++;
	}
	return "";
}


///////////////////////
Bogeyman::Bogeyman(Dungeon* dun,int r, int c)
	:Monster(dun,'B', new ShortSword(-1,-1), r, c, randInt(5,10), 2, randInt(2,3), randInt(2,3), false,"the Bogeyman") {}


Snakewoman::Snakewoman(Dungeon* dun, int r, int c)
	: Monster(dun,'S', new MagicFangs(-1,-1), r, c, randInt(3,6), 3, 2, 3, false,"the Snakewoman") {}

Dragon::Dragon(Dungeon* dun, int r, int c)
	: Monster(dun,'D', new LongSword(-1,-1), r, c, randInt(20, 25), 4, 4, 4, false,"the Dragon") {}

Goblin::Goblin(Dungeon* dun, int r, int c)
	: Monster(dun,'G', new ShortSword(-1,-1), r, c, randInt(15, 20), 1, 3, 1, false,"the Goblin") {}