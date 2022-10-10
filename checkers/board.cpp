#include "board.h"

using namespace std; 

int CheckersBoard::printOptions(int turn){
    getMoves(turn);
    return 0;
}

int CheckersBoard::checkDiagonal(int i, int j, int turn, vector<dataItem> * moves){
    
    // Still need to take care of king backwards... (multiply turn by -1)
    if (i - turn == -1 || i - turn == 8){
        return -1;
    }

    // Turn tells us if the player is going up or down
    // sign flip on turn bc y index is increasing...
    if (j != 7){
        if (board[i - turn][j + 1] == 0){   // Checking if the diagonal square is empty
            dataItem move = {i, j, i - turn, j + 1};
            (*moves).push_back(move);
        }
    }
    
    if (j != 0){
        if (board[i - turn][j - 1] == 0){
            dataItem move = {i, j, i - turn, j - 1};
            (*moves).push_back(move);
        }
    }

    // cout << "Initial: " << board[i][j] << "\n";
    // cout << board[i - turn][j + 1] << "  " << board[i - turn][j - 1] << "\n";

    return 0;
}


int CheckersBoard::getMoves(int turn){
    int skip = 0;
    vector<dataItem> moves;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j ++){
            // cout << "j = " << j + 1 << " " << "i = " << 8 - i << "\n";
            PieceType tmp = board[i][j];
            // cout << "tmp val = " << tmp << "\n";
            if(skip % 2 == 0){ //skip blank squares
                skip++;
                continue;
            } else if (tmp != 0){
                // check diagonal, jumps, and backwards jumps (if king) going UP
                checkDiagonal(i, j, turn, &moves);
            }
            else
                continue;
            skip++;
        }
    }

    // print out moves...
    for(auto tmp : moves){
        cout << "Initial position: " << 8 - tmp.x_initial << " " << tmp.y_initial + 1 << " ";
        cout << "Final position: " << 8 - tmp.x << " " << tmp.y + 1 << "\n";
    }

    return 0;
}

string pieceChar = ".";
string kingChar = "#";
int width = 7;

CheckersBoard::CheckersBoard(string load_file){
    if(load_file == "none"){
        // Player 1
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 8; j++){
                // First and third rows should get a piece if column is odd
                if((i % 2 == 0) && (j % 2 == 1)){
                    PieceType tmp = PIECE_TYPE_BLUE_PIECE;
                    board[i][j] = tmp;
                }
                
                // Middle row gets pieces if column is even
                else if((i % 2 == 1) && (j % 2 == 0)){
                    PieceType tmp = PIECE_TYPE_BLUE_PIECE;
                    board[i][j] = (PieceType)1;
                }

                // Other piece spots should be empty
                else{
                    PieceType tmp = PIECE_TYPE_EMPTY;
                    board[i][j] = (PieceType)0;
                }
            }
        }
        
        // Empty center
        for(int i = 3; i < 5; i++){
            for(int j = 0; j < 8; j++){
                PieceType tmp = PIECE_TYPE_EMPTY;
                board[i][j] = (PieceType)0;
            }
        }


        // Player 2
        for(int i = 5; i < 8; i++){
            for(int j = 0; j < 8; j++){
                // Middle Row gets pieces if column is odd
                if((i % 2 == 0) && (j % 2 == 1)){
                    PieceType tmp = PIECE_TYPE_RED_PIECE;
                    board[i][j] = tmp;
                }
                
                // Top and bottom row get piece if column is even
                else if((i % 2 == 1) && (j % 2 == 0)){
                    PieceType tmp = PIECE_TYPE_RED_PIECE;
                    board[i][j] = (PieceType)-1;
                } else{
                    PieceType tmp = PIECE_TYPE_EMPTY;
                    board[i][j] = (PieceType)0;
                }
            }
        }
    }

};


/*! Center-aligns string within a field of width w. Pads with blank spaces
    to enforce alignment. 
    https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c    
*/
string center(const string s, const int w, const string color, const string color2 = "none") {
    string tmp;
    if (color2 == "none"){
        tmp = RESET;
    } else{
        tmp = color2;
    }
    stringstream ss, spaces;
    int padding = w - s.size();                 // count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << color << s << " " << tmp << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " " << RESET;
    return ss.str();
}


// Fill in empty cells completely
void fillEmpty(int counter){
    for(int k = 0; k < 8; k++){
        string col = RESET;
        if(counter % 2 == 0){
            col = EMPTY_BACK;
        }
        counter++;
        cout << col << center(" ", width, col, col) << RESET;
    }
}

void CheckersBoard::printBoard(){
    int counter = 0;
    string padding = "  ";

    //iterate rows
    for(int i = 0; i < 8; i++){

        // Top border
        cout << RESET << "\n";
        cout << padding;

        // Fill 3 x 3 cell completely
        fillEmpty(counter);
        cout << "\n";

        // Iterate columns
        for(int j = 0; j < 8; j ++){
            cout << RESET;
            if (j == 0){
                cout << 7 - i + 1 << " ";
            }

            // Color pieces based on player / empty state
            PieceType tmp = board[i][j];
            if (counter % 2 == 0){
                cout << EMPTY_BACK;
                cout << center(" ", width, EMPTY_BACK, EMPTY_BACK);
                cout << RESET;
                counter++;
                continue;
            }
            if (tmp == -1){
                cout << center(pieceChar, width, RED_BACK);
                cout << RESET;
            } else if (tmp == 1){
                cout << center(pieceChar, width, BLUE_BACK);
                cout << RESET;
            } else if (tmp == -2){
                cout << center(kingChar, width, RED_BACK);
                cout << RESET;
            } else if (tmp == 2){
                cout << center(kingChar, width, BLUE_BACK);
                cout << RESET;
            } else{
                cout << center(" ", width, RESET);
            }
            cout << RESET;
            counter++;
        }
        cout << "\n" << padding;
        fillEmpty(counter);
        counter++;
    }
    cout << "\n";
    
    // numbers at bottom
    cout << RESET << padding;
    for (int j = 0; j < 8; j++){
        cout << center(to_string(j + 1), width, RESET);
    }
    cout << "\n";
}