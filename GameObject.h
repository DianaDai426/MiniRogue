# ifndef GAMEOBJECT_INCLUDED
# define GAMEOBJECT_INCLUDED

#include <iostream>
#include<string>
using namespace std;

class GameObject
{
public:
	GameObject(int r, int c, char symbol, string name) : m_row(r), m_col(c), m_symbol(symbol), m_name(name) {}

	virtual ~GameObject() {}

	std::string getName() const
	{
		return m_name;
	}
	int row() const
	{
		return m_row;
	}

	int col() const
	{
		return m_col;
	}

	char getSymbol() const
	{
		return m_symbol;
	}


private:
	int m_row;
	int m_col;
	char m_symbol;
	string m_name;

};

class Weapon : public GameObject
{
public:
	Weapon(int r, int c, string name, string action, int dexterityBonus, int damage)
		:GameObject(r, c, ')', name)
	{
		m_action = action;
		m_dexterityBonus = dexterityBonus;
		m_damage = damage;
	}
	virtual ~Weapon() {}

	string getAction() const
	{
		return m_action;
	}
	int getDexerityBonus() const
	{
		return m_dexterityBonus; //use formula to decide whether to use me
	}
	int getDamage() const
	{
		return m_damage; //use formula to determine ho wmuch damage to the player
	}

private:
	string m_action;
	int m_dexterityBonus;
	int m_damage;
};

class Mace :public Weapon
{
public:
	Mace(int r , int c ) :Weapon(r, c, "mace", "swings", 0, 2) {}
	~Mace() {}
};

class ShortSword :public Weapon
{
public:
	ShortSword(int r , int c ) :Weapon(r, c, "short sword", "slashes", 0, 2) {}
	~ShortSword() {}
};

class LongSword :public Weapon
{
public:
	LongSword(int r , int c ) :Weapon(r, c, "long sword", "swings", 2, 4) {}
	~LongSword() {}
};

class MagicAxe :public Weapon
{
public:
	MagicAxe(int r, int c) :Weapon(r, c, "magic axe", "chops", 5, 5) {}
	~MagicAxe() {}
};

class MagicFangs :public Weapon
{
public:
	MagicFangs(int r , int c) :Weapon(r, c, "magic fangs of sleep", "strikes", 3, 2) {}
	~MagicFangs() {}
	//there is a 1 in 5 chance that the magic fangs 
	//of sleep will put the defender to sleep. 
	//The number of moves the sleep will last (the 
	//sleep time) is a random integer from 2 to 6, call 
	//it X. If the defender is already asleep, with 
	//sleep time Y, then the defender's sleep time becomes 
	//the maximum of X and Y (and not, say, Y+X).
};

class Scroll :public GameObject //it self-destruct after performed its effect 
{
public:
	Scroll(int r, int c, string name, string effect) :GameObject(r, c, '?', name)
	{
		m_effect = effect;
	}
	virtual ~Scroll() {}

	string getEffect() const
	{
		return m_effect;
	}

	//virtual void doEffect() = 0;

private:
	string m_effect;
};

class Teleportation :public Scroll
{
public:
	Teleportation(int r, int c) : Scroll(r, c, "scroll of teleportation",
		"You feel your body wrenched in space and time.") {}
	//virtual void doEffect();
	~Teleportation() {}
};

class Strength :public Scroll
{
public:
	Strength(int r, int c) : Scroll(r, c, "scroll of strength",
		"Your muscles bulge.") {}
	//virtual void doEffect();
	~Strength() {}
};

class EnhanceArmor :public Scroll
{
public:
	EnhanceArmor(int r, int c) : Scroll(r, c, "scroll of enhance armor",
		"Your armor glows blue.") {}
	//virtual void doEffect();
	~EnhanceArmor() {}
};

class EnhanceHealth :public Scroll
{
public:
	EnhanceHealth(int r, int c) : Scroll(r, c, "scroll of enhance health",
		"You feel your heart beating stronger.") {}
	//virtual void doEffect();
	~EnhanceHealth() {}
};

class EnhanceDexterity :public Scroll
{
public:
	EnhanceDexterity(int r, int c) : Scroll(r, c, "scroll of enhance dexterity",
		"You feel like less of a klutz.") {}
	//virtual void doEffect();
	~EnhanceDexterity() {}
};


#endif

