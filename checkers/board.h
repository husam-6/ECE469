#ifndef _BOARD_H
#define _BOARD_H
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <stdlib.h>
#include <queue>

// For colors
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define BLUE_BACK "\x1b[44m"
#define RED_BACK "\x1b[45m"
#define EMPTY_BACK "\x1b[47m"

// Class for storing the board state and pieces
class CheckersBoard {
    public:
        // Function to print numbered list of move options
        int printOptions();

        // Display board state
        void printBoard();

        // Moves a piece
        int movePiece(int option, int jump = 0);

        // Constructor creates game defaulting to regular loaded board
        // 8 x 8 with 24 pieces (12 each player)
        // If a load_file is specified, that game state will be loaded instead
        // Turn variable indicates blue player (-1) and red player (1)
        CheckersBoard(std::string load_file = "startGame.txt", int playerTurn = 1);

        private:
            // Item to store coordinates of a move 
            class dataItem{
                public:
                    int x_initial;
                    int y_initial;
                    int x; 
                    int y;
            };
        
            // Returns pointer to array of valid coordinate moves for a given point
            // returns array of all valid moves for player
            int getMoves();

            // Check for valid diagonal moves at given coordinate
            // Returns array of valid diagonal moves
            int checkDiagonal(int i, int j);

            // Check for valid jumps at given coordinate
            int checkJumps(int i, int j, int boardCopy[8][8], int pos = -1);
        
            // Int array bitmap for each square on the board
            // Assign 0 as empty
            // 1 as black piece
            // 2 as black king
            // -1 as red piece
            // -2 as red king
            int board[8][8];

            int timeLimit; 

            // Indicates whose turn it is -> 1 for Blue and -1 for Red
            int turn;

            // Counters to keep track of the number of pieces
            int numRed;
            int numBlue;
            int numRedKings;
            int numBlueKings;

            // vectors to store possible jumps / diagonal moves
            std::vector<std::vector<dataItem>> jumps;
            std::vector<dataItem> moves;

};



#endif //_BOARD_H