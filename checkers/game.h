#ifndef _GAME_H
#define _GAME_H

#include "board.h"


// Class that users CheckersBoard to play a full game
class Game {
    public:
        // Function to play a game
        int playGame();

        // Constructor starts game using CheckerBoard class
        // Take in options from user
        Game();

        private:
            
            // Variable to indicate how many humans are playing
            // Computer vs Computer | Computer vs Human | Human vs Human
            //          0           |         1         |       2
            int numHumans;

            // Upon initialization, which player starts
            int playerStart;

            CheckersBoard board;

};



#endif //_GAME_H