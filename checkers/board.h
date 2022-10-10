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
#define BLACK_BACK "\x1b[44m"
#define RED_BACK "\x1b[45m"
#define EMPTY_BACK "\x1b[47m"


class CheckersBoard {
    public:
        // Function to print numbered list of move options
        int * printOptions();

        // Display board state
        void printBoard();

        // Constructor creates game defaulting to regular loaded board
        // 8 x 8 with 24 pieces (12 each player)
        // if a load_file is specified, that game state will be loaded instead
        CheckersBoard(std::string load_file = "none");

        private:
            // Struct for coordinate values for legal moves
            struct coords{
                int x;
                int y;
            };
        
            // Returns pointer to array of valid coordinate moves for a given point
            coords * getMoves(std::string &out);
        
            // Enum for each square on the board
            // Assign 0 as empty
            // 1 as black piece
            // 2 as black king
            // -1 as red piece
            // -2 as red king
            enum PieceType{
                PIECE_TYPE_EMPTY = 0,
                PIECE_TYPE_BLACK_PIECE = 1,
                PIECE_TYPE_BLACK_KING = 2,
                PIECE_TYPE_RED_PIECE = -1,
                PIECE_TYPE_RED_KING = -2
            };

            enum PieceType board[8][8];

};



#endif //_HASH_H