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

    timeLimit = 5;

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
                board[i][j] = 0;
                counter++;
                continue;
            }
            if (piece == '3'){
                board[i][j] = -1;
                numRed++;
            }
            else if(piece == '4'){
                board[i][j] = -2;
                numRedKings++;
            }
            else{
                int tmp = int(piece - '0');
                if (tmp == 1)
                    numBlue++;
                else if (tmp == 2)
                    numBlueKings++;

                board[i][j] = tmp;
            }
            counter++;
        }
        counter++;
        i++;
    }
    
    stateFile.close();

};

int CheckersBoard::testEnd(){
    int total;
    total = jumps.size() + moves.size();
    return total; 
}

string CheckersBoard::checkWinner(){
    if ((turn * -1 ) == -1){
        return "RED";
    } 
    return "BLUE";
}

int CheckersBoard::getTurn(){
    return turn; 
}

int CheckersBoard::getNumberOfOptions(){
    if (jumps.size() > 0){
        return jumps.size();
    }
    return moves.size();
}


// helper function to print options
void printCoords(int y, int x){
    cout << "(" << y + 1 << ", " << 8 - x << ")";
}


// Returns -1 when no moves are present
// Returns 1 when a jump was made
// Returns 0 when a diagonal move was made
int CheckersBoard::printOptions(){
    // Store possible moves in a vector (jumps and moves member functions)
    getMoves();

    // Print out numbered options...
    string color; 
    if (turn == -1)
        color = "RED";
    else
        color = "BLUE";
    cout << "Options in (x, y) coordinate format" << "\n"; 
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
        return 1;

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
        return 0;
    }

    return -1;
}

void CheckersBoard::makeKing(int i, int j){
    if (board[i][j] == -2 || board[i][j] == 2)
        return;
    if (turn == -1){
        if (i == 0){
            board[i][j] *= 2; 
        }
    }
    else if (turn == 1){
        if (i == 7){
            board[i][j] *= 2;
        }
    }
}


// Helper function for movePiece function
int midPoint(int x1, int x2){
    return (x1 + x2) / 2;
}


int CheckersBoard::movePiece(int option, int jump){
    // Check if option given is out of bounds..
    if (jump){
        if (option >= jumps.size() || option < 0)
            return -1;
    }
    else{
        if (option >= moves.size() || option < 0)
            return -1;
    }
    
    // Diagonal moves
    if (!jump){
        dataItem move = moves[option];
        board[move.x][move.y] = board[move.x_initial][move.y_initial];
        board[move.x_initial][move.y_initial] = 0;
        moves.clear();

        // Check if we can turn the piece into a king
        makeKing(move.x, move.y);

        turn = turn * -1; 
        return 0;
    }

    // Jump moves - loop through each jump (if there are multiple)
    int i = 0;
    int tmpPiece = 0;
    for (auto move : jumps[option]){
        if(i == 0){
            tmpPiece = board[move.x_initial][move.y_initial];
            board[move.x_initial][move.y_initial] = 0;
        }
        
        int skip_i, skip_j;
        skip_i = midPoint(move.x_initial, move.x);
        skip_j = midPoint(move.y_initial, move.y);
        
        // Update piece counters
        switch (board[skip_i][skip_j]){
            case 1:
                numBlue--;
                break;
            case 2:
                numBlueKings--;
                break;
            case -1:
                numRed--;
                break;
            case -2:
                numRedKings--;
                break;
        }

        board[skip_i][skip_j] = 0; 
        i++;
    }
    
    dataItem move = jumps[option].back();
    board[move.x][move.y] = tmpPiece;

    // Remove elements from array 
    jumps.clear();
    moves.clear();

    // Check if we can turn the piece into a king
    makeKing(move.x, move.y);

    // Change turn to the other player
    turn = turn * -1;


    return 0;
}


int CheckersBoard::checkJumps(int i, int j, int boardCopy[8][8], int pos){
    
    // Turn tells us if the player is going up or down
    // Skip if it isnt your turn!
    if ((turn < 0 && boardCopy[i][j] > 0) || (turn > 0 && boardCopy[i][j] < 0))
        return -1;

    // Boolean variables to store possible directions
    int forward = 1;
    int backwards = 0;
    
    // Check if a piece can move forwards 
    // Can't step out of bounds
    if ((i + 2 * turn <= -1) || (i + 2 * turn >= 8)){
        forward = 0;
    }

    // Check if the piece is allowed to move backwards:
    // Needs to be a King && can't step out of bounds
    if ((boardCopy[i][j] == 2) || (boardCopy[i][j] == -2)){
        if (i - 2 * turn > -1  &&  i - 2 * turn < 8){
            backwards = 1;
        }
    }

    int countDuplicates = 0;

    // Right side jumps
    if (j <= 5 && forward){
        // Check for opposing side pieces on diagonal squares
        if (boardCopy[i + turn][j + 1] == (turn * -1) || boardCopy[i + turn][j + 1] == (turn * -2)){
            // Check if square directly after opposing side is empty
            if (boardCopy[i + 2 * turn][j + 2] == 0){
                dataItem move = {i, j, i + 2 * turn, j + 2};
                vector<dataItem> tmp;
                if (pos != -1){
                    // Make a copy of the last vector of moves to expand on
                    tmp = jumps[pos];
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                    countDuplicates++;
                }
                else{
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                }

                // Remove captured piece from board copy, pass into recursive call
                int tmpPiece = boardCopy[i + turn][j + 1];
                int tmpCurr = boardCopy[i][j];
                boardCopy[i + 2 * turn][j + 2] = boardCopy[i][j];
                boardCopy[i + turn][j + 1] = 0;
                boardCopy[i][j] = 0;
                int position = jumps.size() - 1;
                checkJumps(i + 2 * turn, j + 2,boardCopy, position);

                // Put piece back for other checks
                boardCopy[i + turn][j + 1] = tmpPiece;
                boardCopy[i + 2 * turn][j + 2] = 0;
                boardCopy[i][j] = tmpCurr;
            }
        }
    }
    
    // Left side jumps
    if (j >= 2 && forward){
        // Check for opposing side pieces on diagonal squares
        if (boardCopy[i + turn][j - 1] == (turn * -1) || boardCopy[i + turn][j - 1] == (turn * -2)){
            // Check if square directly after opposing side is empty
            if (boardCopy[i + 2 * turn][j - 2] == 0){
                dataItem move = {i, j, i + 2 * turn, j - 2};
                vector<dataItem> tmp;
                if (pos != -1){
                    tmp = jumps[pos];
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                    countDuplicates++;
                }
                else{
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                }

                // Remove captured piece from board copy
                int tmpPiece = boardCopy[i + turn][j - 1];
                int tmpCurr = boardCopy[i][j];
                boardCopy[i + 2 * turn][j - 2] = boardCopy[i][j];
                boardCopy[i + turn][j - 1] = 0;
                boardCopy[i][j] = 0;

                int position = jumps.size() - 1;
                checkJumps(i + 2 * turn, j - 2, boardCopy, position);

                // Put pieces back
                boardCopy[i + turn][j - 1] = tmpPiece;
                boardCopy[i + 2 * turn][j - 2] = 0;
                board[i][j] = tmpCurr;
            }
        }
    }

    // Right jumps (backwards)
    if (j <= 5 && backwards){
        if (boardCopy[i - turn][j + 1] == (turn * -1) || boardCopy[i - turn][j + 1] == (turn * -2)){
            // Check if square directly after opposing side is empty
            if (boardCopy[i - 2 * turn][j + 2] == 0){
                dataItem move = {i, j, i - 2 * turn, j + 2};
                vector<dataItem> tmp;
                if (pos != -1){
                    tmp = jumps[pos];
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                    countDuplicates++;
                }
                else{
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                }
                
                // Remove captured piece from board copy
                int tmpPiece = boardCopy[i - turn][j + 1];
                int tmpCurr = boardCopy[i][j];
                boardCopy[i - 2 * turn][j + 2] = board[i][j];
                boardCopy[i - turn][j + 1] = 0;
                boardCopy[i][j] = 0;
                int position = jumps.size() - 1;
                checkJumps(i - 2 * turn, j + 2, boardCopy, position);

                // Put piece back
                boardCopy[i - turn][j + 1] = tmpPiece;
                boardCopy[i - 2 * turn][j + 2] = 0;
                boardCopy[i][j] = tmpCurr;
            }
        }
    }
    
    // Left jumps (backwards)
    if (j >=2 && backwards){
        if (boardCopy[i - turn][j - 1] == (turn * -1) || boardCopy[i - turn][j - 1] == (turn * -2)){
            // Check if square directly after opposing side is empty
            if (boardCopy[i - 2 * turn][j - 2] == 0){
                dataItem move = {i, j, i - 2 * turn, j - 2};
                vector<dataItem> tmp;
                if (pos != -1){
                    tmp = jumps[pos];
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                    countDuplicates++;
                }
                else{
                    tmp.push_back(move);
                    jumps.push_back(tmp);
                }

                // Remove captured piece from board copy
                int tmpPiece = boardCopy[i - turn][j - 1];
                int tmpCurr = boardCopy[i][j];
                boardCopy[i - 2 * turn][j - 2] = board[i][j];
                boardCopy[i - turn][j - 1] = 0;
                boardCopy[i][j] = 0;
                int position = jumps.size() - 1;
                checkJumps(i - 2 * turn, j - 2, boardCopy, position);

                // Put piece back
                boardCopy[i - turn][j - 1] = tmpPiece;
                boardCopy[i - 2 * turn][j - 2] = 0;
                boardCopy[i][j] = tmpCurr;
            }
        }

    } 

    if (pos != -1 && countDuplicates > 0)
        jumps.erase(jumps.begin() + pos);


    return 0;

}


int CheckersBoard::checkDiagonal(int i, int j){

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
            moves.push_back(move);
        }

    }
    if (j != 7 && backwards){
        if (board[i - turn][j + 1] == 0){
            dataItem move = {i, j, i - turn, j + 1};
            moves.push_back(move);
        }
    }
    
    // Check moves to the left
    if (j != 0 && forward){
        if (board[i + turn][j - 1] == 0){
            dataItem move = {i, j, i + turn, j - 1};
            moves.push_back(move);
        }
    }
    if (j != 0 && backwards){
        if (board[i - turn][j - 1] == 0){
            dataItem move = {i, j, i - turn, j - 1};
            moves.push_back(move);
        }
    }

    return 0;
}


int CheckersBoard::getMoves(){
    int skip = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j ++){
            int tmp = board[i][j];
            
            //skip blank squares
            if(skip % 2 == 0){
                skip++;
                continue;
            } else if (tmp != 0){
                // check jumps, if none then diagonal moves
                checkJumps(i, j, board, -1);
                if (jumps.size() == 0)
                    checkDiagonal(i, j);
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
            int tmp = board[i][j];
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