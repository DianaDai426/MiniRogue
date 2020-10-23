// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance){
    //create a dungeon
    dungeon = new Dungeon(goblinSmellDistance);
}

void Game::play()
{
    dungeon->display();
    //dungeon->setPlayerPos(dungeon->firstM()->row(), dungeon->firstM()->col() - 1);
    dungeon->getPlayer()->printInventory();
    cout << "Press q to exit game." << endl;
    int loopTime = 0;
    while (1)
    {
        loopTime++;
        dungeon->clearMessage();
        char input = getCharacter();
        if (dungeon->getPlayer()->isDead() || dungeon->getPlayer()->hasWon())
        {
            while (1)
            {
                if (getCharacter() == 'q')
                {
                    exit(0);
                }
            }
        }

        else if (dungeon->getPlayer()->getSleepTime() > 0)
        {
            dungeon->getPlayer()->decreaseSleep();
            dungeon->appendMessage("player is put to sleep, cannot make any move");
        }

        else
        {
            switch (input)
            {
            case ARROW_LEFT:
                dungeon->movePlayerTo(dungeon->getPlayer()->row(), dungeon->getPlayer()->col() - 1);
                break;
            case ARROW_RIGHT:
                dungeon->movePlayerTo(dungeon->getPlayer()->row(), dungeon->getPlayer()->col() + 1);
                break;
            case ARROW_UP:
                dungeon->movePlayerTo(dungeon->getPlayer()->row()-1, dungeon->getPlayer()->col());
                break;
            case ARROW_DOWN:
                dungeon->movePlayerTo(dungeon->getPlayer()->row() +1, dungeon->getPlayer()->col());
                break;
            case 'i':
                clearScreen();
                dungeon->getPlayer()->printInventory();
                getCharacter();
                break;
            case 'g':
                dungeon->pickUp();
                break;
            case 'c':
                dungeon->getPlayer()->cheat();
            case '<':
                if (dungeon->isOnStair())
                {
                    dungeon->nextLevel();
                }
                break;
            case 'w':
                dungeon->setMessage(dungeon->getPlayer()->wield());
                getCharacter();
                break;
            case 'r':
                dungeon->read();
                getCharacter();
                break;
            case 'q':
                exit(0);

            }
        }

        dungeon->monstersTurn();
        dungeon->display();
        dungeon->getPlayer()->printInventory();
        cout<<"loopTime="<<loopTime<<endl;
        cout << "Press q to exit game." << endl;
    }

    //cout << "Press q to exit game." << endl;
    //while (getCharacter() != 'q')
    //    ;
    
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
