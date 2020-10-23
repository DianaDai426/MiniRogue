// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED
#include"Dungeon.h"
#include"Actors.h"
// You may add data members and other member functions to this class.

class Game
{
public:
    Game(int goblinSmellDistance);
    //void nextLevel();

    void play();
private:
    Dungeon* dungeon;

};

#endif // GAME_INCLUDED

