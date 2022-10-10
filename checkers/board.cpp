#include "board.h"

using namespace std; 

CheckersBoard::CheckersBoard(string load_file){
    if(load_file == "none"){
        // Player 1
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 8; j++){
                // First and third rows should get a piece if column is odd
                if((i % 2 == 0) && (j % 2 == 1)){
                    PieceType tmp = PIECE_TYPE_BLACK_PIECE;
                    board[i][j] = tmp;
                }
                
                // Middle row gets pieces if column is even
                else if((i % 2 == 1) && (j % 2 == 0)){
                    PieceType tmp = PIECE_TYPE_BLACK_PIECE;
                    board[i][j] = (PieceType)1;
                }

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
    ss << spaces.str() << color << s << tmp << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " " << RESET;
    return ss.str();
}

void longBar(){
    for(int i = 0; i < 90; i++){
        cout << "-";
    }
}

void fillEmpty(int counter){
    // Fill in empty cells completely
    for(int k = 0; k < 8; k++){
        string col = RESET;
        if(counter % 2 == 0){
            col = EMPTY_BACK;
        }
        counter++;
        cout << "|" << col << center(" ", 10, col, col);
    }
    cout << "|";
}

void CheckersBoard::printBoard(){
    int counter = 0;
    string padding = " ";

    //iterate rows
    for(int i = 0; i < 8; i++){

        // Top border
        cout << "\n";
        longBar();
        cout << "\n";
        cout << padding;

        // Fill 3 x 3 cell completely
        fillEmpty(counter);
        cout << "\n";

        // Iterate columns
        for(int j = 0; j < 8; j ++){
            // cout << RESET;
            if (j == 0){
                cout << i + 1;
            }
            cout << "|";

            // Color pieces based on player / empty state
            PieceType tmp = board[i][j];
            if (counter % 2 == 0){
                cout << EMPTY_BACK;
                cout << center(" ", 10, EMPTY_BACK, EMPTY_BACK);
                counter++;
                continue;
            }
            if (tmp < 0){
                cout << center("*", 10, RED_BACK);
                cout << RESET;
            } else if (tmp > 0){
                cout << center("*", 10, BLACK_BACK);
                cout << RESET;
            } else{
                cout << center(" ", 10, RESET);
            }
            cout << RESET;
            counter++;
        }
        cout << "|";
        cout << "\n" << padding;
        fillEmpty(counter);
        counter++;
    }
    cout << "\n";
    longBar();
    cout << "\n"; 
    
    // numbers at bottom
    cout << padding;
    for (int j = 0; j < 8; j++){
        cout << center(to_string(j + 1), 11, RESET);
    }
    cout << "\n";
}