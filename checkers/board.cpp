#include "board.h"

using namespace std; 

string pieceChar = " ";
string kingChar = "#";
int width = 7;


CheckersBoard::CheckersBoard(string loadFile, int playerTurn){
    // NOTE: BLUE ALWAYS GOING DOWN, RED GOING UP (from initial state)
    
    string line; 
    ifstream stateFile;
    stateFile.open(loadFile); 

    int i = 0;
    int counter = 0;
    while(getline(stateFile, line))
    {

        if (i == 8){
            turn = stoi(line);
            continue;
        }
        else if (i == 9){
            timeLimit = stoi(line);
            continue;
        }

        for(int j = 0; j < 8; j++){
            char piece = line[j];
            if (counter % 2 == 0){
                counter++;
                continue;
            }
            if (piece == '3'){
                board[i][j] = (PieceType)-1;
            }
            else if(piece == '4'){
                board[i][j] = (PieceType)-2;
            }
            else{    
                board[i][j] = (PieceType)int(piece - '0');
            }
            counter++;
        }
        counter++;
        i++;
    }
    
    turn = playerTurn;
    stateFile.close();

};


int CheckersBoard::printOptions(){
    getMoves();
    return 0;
}


int CheckersBoard::checkDiagonal(int i, int j, vector<dataItem> * moves){
    
    // Still need to take care of king backwards... (maybe multiply turn by -1 and repeat)
    if (i + turn == -1 || i + turn == 8){
        return -1;
    }

    // Turn tells us if the player is going up or down
    // Skip if it isnt your turn!
    if ((turn < 0 && board[i][j] > 0) || (turn > 0 && board[i][j] < 0))
        return -1;

    if (j != 7){
        if (board[i + turn][j + 1] == 0){   // Checking if the diagonal square is empty
            dataItem move = {i, j, i + turn, j + 1};
            (*moves).push_back(move);
        }
    }
    
    if (j != 0){
        if (board[i + turn][j - 1] == 0){
            dataItem move = {i, j, i + turn, j - 1};
            (*moves).push_back(move);
        }
    }

    return 0;
}


int CheckersBoard::getMoves(){
    int skip = 0;
    vector<dataItem> moves;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j ++){
            PieceType tmp = board[i][j];
            
            //skip blank squares
            if(skip % 2 == 0){
                skip++;
                continue;
            } else if (tmp != 0){
                // check diagonal, jumps, and backwards jumps (if king)
                checkDiagonal(i, j, &moves);
            }
            skip++;
        }
        skip++;
    }

    // print out moves...
    string color; 
    if (turn == -1)
        color = "RED";
    else
        color = "BLUE";
    cout << "Valid moves for player: " << color << "\n";
    for(auto tmp : moves){
        cout << "Initial position: " << "(" << tmp.y_initial + 1 <<  ", " << 8 - tmp.x_initial << ") ";
        cout << "Final position: " << "(" << tmp.y + 1 << ", " << 8 - tmp.x << ")" << "\n";
    }

    return 0;
}

/*! Center-aligns string within a field of width w. Pads with blank spaces to enforce alignment. 
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