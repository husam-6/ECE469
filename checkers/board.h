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


class CheckersBoard {
    public:
        // Function to print numbered list of move options
        int * printOptions();

        // Display board state
        void printBoard();

        // Constructor creates game defaulting to regular loaded board
        // 8 x 8 with 24 pieces (12 each player)
        // if a load_file is specified, that game state will be loaded instead
        CheckersBoard(std::string load_file = NULL);

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
                PIECE_TYPE_EMPTY,
                PIECE_TYPE_BLACK_PIECE,
                PIECE_TYPE_BLACK_KING,
                PIECE_TYPE_RED_PIECE,
                PIECE_TYPE_RED_KING
            };

            enum PieceType board[8][8];

};



#endif //_HASH_H