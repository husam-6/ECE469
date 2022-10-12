#include "board.h"

using namespace std; 

string pieceChar = " ";
string kingChar = "#";
int width = 7;


CheckersBoard::CheckersBoard(string loadFile, int playerTurn){
    // NOTE: BLUE ALWAYS GOING DOWN, RED GOING UP (from initial state)
    turn = playerTurn;
    
    string line; 
    ifstream stateFile;
    stateFile.open(loadFile); 

    int i = 0;
    int counter = 0;
    // Loop through each line of the state file
    while(getline(stateFile, line))
    {
        
        // Store turn variable
        if (i == 8){
            int tmpNum = stoi(line);
            if (tmpNum == 1)
                turn = stoi(line);
            else if (tmpNum == 2)
                turn = -1;
            continue;
        }
        
        // Store time limit
        else if (i == 9){
            timeLimit = stoi(line);
            continue;
        }

        // Set the pieces on the board
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
    
    stateFile.close();

};


// helper function to print options
void printCoords(int y, int x){
    cout << "(" << y + 1 << ", " << 8 - x << ")";
}


int CheckersBoard::printOptions(){
    // Store possible moves in a vector
    vector<dataItem> moves;
    vector<vector<dataItem>> jumps;
    getMoves(&moves, &jumps);

    // Print out numbered options...
    string color; 
    if (turn == -1)
        color = "RED";
    else
        color = "BLUE";
    cout << "Valid moves for player " << color << ":\n";
    int i = 0;

    if (jumps.size()){
        for (auto tmp : jumps){
            cout << "OPTION " << i << " -- ";
            int idx = 0;
            for (auto x : tmp){    
                if (!idx)
                    printCoords(x.y_initial, x.x_initial);
                cout << " => ";
                printCoords(x.y, x.x);
                idx++;
            }
            cout << "\n";
            i++;
        }
    }
    else{
        for(auto x : moves){
            cout << "OPTION " << i << " -- ";
            printCoords(x.y_initial, x.x_initial);
            cout << " => ";
            printCoords(x.y, x.x);
            cout << "\n";
            i++;
        }


    }

    return 0;
}

int CheckersBoard::checkJumps(int i, int j, std::vector<vector<dataItem>> * jumps, int pos){
    
    // Turn tells us if the player is going up or down
    // Skip if it isnt your turn!
    if ((turn < 0 && board[i][j] > 0) || (turn > 0 && board[i][j] < 0))
        return -1;

    int forward = 1;
    int backwards = 0;
    
    // Check if a piece can move forwards 
    // Can't step out of bounds
    if (i + 2 * turn == -1 || i + 2 * turn == 8){
        forward = 0;
    }

    // Right side jumps
    if (j <= 5 && forward){
        // Check for opposing side pieces on diagonal squares
        if (board[i + turn][j + 1] == (turn * -1)){
            // Check if square directly after opposing side is empty
            if (board[i + 2 * turn][j + 2] == 0){
                dataItem move = {i, j, i + 2 * turn, j + 2};
                vector<dataItem> tmp;
                if (pos != -1){
                    tmp = (*jumps)[pos];
                    tmp.push_back(move);
                    (*jumps).push_back(tmp);
                }
                else{
                    tmp.push_back(move);
                    (*jumps).push_back(tmp);
                }
                
                int position = (*jumps).size() - 1;
                checkJumps(i + 2 * turn, j + 2, jumps, position);
            }
        }
    }
    
    // Left side jumps
    if (j >= 2 && forward){
        // Check for opposing side pieces on diagonal squares
        if (board[i + turn][j - 1] == (turn * -1)){
            // Check if square directly after opposing side is empty
            if (board[i + 2 * turn][j - 2] == 0){
                dataItem move = {i, j, i + 2 * turn, j - 2};
                vector<dataItem> tmp;
                if (pos != -1){
                    tmp = (*jumps)[pos];
                    tmp.push_back(move);
                    (*jumps).push_back(tmp);
                }
                else{
                    tmp.push_back(move);
                    (*jumps).push_back(tmp);
                }

                int position = (*jumps).size() - 1;
                checkJumps(i + 2 * turn, j - 2, jumps, position);
            }
        }
    }

    return 0;

}


int CheckersBoard::checkDiagonal(int i, int j, vector<dataItem> * moves){

    // Turn tells us if the player is going up or down
    // Skip if it isnt your turn!
    if ((turn < 0 && board[i][j] > 0) || (turn > 0 && board[i][j] < 0))
        return -1;

    // Booleans to indicate if a piece can move forwards / backwards
    // Default - can go forward, cant go back
    int forward = 1; 
    int backwards = 0; 

    // Check if the piece is allowed to move backwards:
    // Needs to be a King && can't step out of bounds
    if ((board[i][j] == 2) || (board[i][j] == -2)){
        if (i - turn != -1 &&  i - turn != 8)
            backwards = 1;
    }

    // Check if a piece can move forwards 
    // Can't step out of bounds
    if (i + turn == -1 || i + turn == 8){
        forward = 0;
    }

    // Check moves to the right
    if (j != 7 && forward){
        // Checking if the diagonal square is empty
        if (board[i + turn][j + 1] == 0){
            dataItem move = {i, j, i + turn, j + 1};
            (*moves).push_back(move);
        }

        if (backwards){
            if (board[i - turn][j + 1] == 0){
                dataItem move = {i, j, i - turn, j + 1};
                (*moves).push_back(move);
            }
        }
    }
    
    // Check moves to the left
    if (j != 0 && forward){
        if (board[i + turn][j - 1] == 0){
            dataItem move = {i, j, i + turn, j - 1};
            (*moves).push_back(move);
        }
        if (backwards){
            if (board[i - turn][j - 1] == 0){
                dataItem move = {i, j, i - turn, j - 1};
                (*moves).push_back(move);
            }
        }
    }

    return 0;
}


int CheckersBoard::getMoves(vector<dataItem> * moves, vector<vector<dataItem>> * jumps){
    int skip = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j ++){
            PieceType tmp = board[i][j];
            
            //skip blank squares
            if(skip % 2 == 0){
                skip++;
                continue;
            } else if (tmp != 0){
                // check jumps, if none then diagonal moves
                checkJumps(i, j, jumps);
                if ((*jumps).size() == 0)
                    checkDiagonal(i, j, moves);
            }
            skip++;
        }
        skip++;
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

// Print the board state
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