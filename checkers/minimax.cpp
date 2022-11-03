#include "board.h"
#include <map>

using namespace std;

// Helper function for evaluation function
int sign(int num){
    if (num > 0) return 1;
    if (num < 0) return -1;
    return 0;
}

// Find the distance between two coordinates
float dist(int king_i, int king_j, int i, int j){
  int x_dist, y_dist;
  
  x_dist = (king_i - i);
  x_dist *= x_dist;
  y_dist = (king_j - j);
  y_dist *= y_dist;
  if (x_dist + y_dist > 0) 
    return sqrt(x_dist + y_dist);
  else
    return 0;
}


// Helper function to get total distance for a given piece 
int totalDist(int (&state)[8][8], int playerTurn, int king_i, int king_j){
    float distance = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (sign(state[i][j]) == sign(-1 * playerTurn))
                distance += dist(king_i, king_j, i, j);
            // if (state[i][j] == 2 * playerTurn)
            //     distance -= dist(king_i, king_j, i, j);

        }
    }

    return int(distance);
}

// Function to return penalty equal to total distance between kings and opponent pieces
int endGame(int (&state)[8][8], int playerTurn){
    int distance = 0; 
    for(int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (state[i][j] == 2 * playerTurn){
                distance += totalDist(state, playerTurn, i, j); 
            }
        }
    }

    return distance;   
}

// Heuristic evaluation of the board state
// AGGRETION FACTOR - most divided by least and multiply by constant 
int CheckersBoard::eval(int (&state)[8][8], int playerTurn, int cut, int depth){
    // RED is always positive (MAX), BLUE is always negative (MIN)
    int pawn = 300000;
    int king = 500000;
    int promotion = 10000;
    int goalie = 1000;
    int trades = 100;
    int distance_factor = 1000;
    int numRed = 0;
    int numBlue = 0;
    float maxDistRed = 0;
    float maxDistBlue = 0;
    int blueCorner = 0;
    int redCorner = 0;  

    // Variables to keep track of in loop through all pieces
    int tmp;
    int weight = 0;
    int counter = 0;
    int pieceCounter = 0; 
    int kpTotal = 0;
    int numRedGoalies = 0;
    int numBlueGoalies = 0; 
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (counter % 2 == 0){
                counter++;
                continue;
            }
            tmp = state[i][j];

            // Figure out weights for each piece (ie pawn vs king)
            if (abs(tmp) == 1){
                weight = pawn;
                pieceCounter++;
                if (tmp < 0){
                    numRed++;
                    if (i == 7 && tmp == -1){
                        numRedGoalies+=1;
                        if (j == 2 || j == 4)
                            numRedGoalies+=1; 
                    }
                    if (tmp == -2 && ((i == 7 && j == 6) || (i == 6 && j == 7) || (i == 0 && j == 1) || (i == 1 && j == 0)))
                        redCorner+=1;
                }
                else if (tmp > 0){
                    numBlue++;
                    if (i == 0 && tmp == 1){
                        numBlueGoalies+=1;
                        if (j == 3 || j == 5)
                            numBlueGoalies+=1; 
                    }
                    if (tmp == 2 && ((i == 7 && j == 6) || (i == 6 && j == 7) || (i == 0 && j == 1) || (i == 1 && j == 0)))
                        blueCorner+=1;
                }
                kpTotal += weight * tmp;
                
            }
            else if(abs(tmp) == 2){
                weight = king;
                pieceCounter++;
                kpTotal += weight * sign(tmp);
            }
            counter++;
        }
        counter++;
    }

    // Random digit to break ties
    int ties = rand() % 11 - 5;
    int ret = kpTotal + ties;

    // End Games
    if (kpTotal > 0 && pieceCounter <= 7){
        ret -= distance_factor * endGame(state, playerTurn);
        ret += 100 * distance_factor * blueCorner;
        ret -= 100 * distance_factor * redCorner;
        if (redCorner == 0)
            ret += distance_factor * 100; 

    }
    if (kpTotal < 0 && pieceCounter <= 7){
        ret += distance_factor * endGame(state, playerTurn);
        ret -= 100 * distance_factor * redCorner;
        ret += 100 * distance_factor * blueCorner;
        if (blueCorner == 0)
            ret -= distance_factor * 100;
    }


    // Bonus for having pieces on the back rank
    ret += goalie * numBlueGoalies;
    ret -= goalie * numRedGoalies; 
    
    // Emphasize trades... Lower total number of pieces is better (if winning)! 
    int reward = 24 - pieceCounter;

    // Check whose currently winning
    // If MAX / BLUE is winning, higher eval for lower pieces
    if (kpTotal > 0)
        ret += trades * reward;

    // If MIN / RED is winning, lower eval for lower pieces
    else if (kpTotal < 0)
        ret -= trades * reward; 

    // Add value for how far piece is from promotion... (for Red, index is distance to back rank, 
    // for Blue, 8 - index gives us this)
    // ret -= promotion * aveDistBlue;
    // ret += promotion * aveDistRed;  

    // Trying to account for definite wins / losses
    if (cut == 3 && playerTurn == -1)
        ret += (10000000 - 10 * depth);
    else if (cut == 3 && playerTurn == 1)
        ret -= (10000000 - 10 * depth);

    return ret;
}

// Function to check if the current state is a cutoff state
// Returns 0 if it is not a cut off state
// Returns 1 if the depth limit has been reached
// Returns 2 if time limit has been reached
// Returns 3 if no valid moves are left
int CheckersBoard::isCutOff(int depth, vector<vector<dataItem>> &jumps, vector<dataItem> &moves){
    if (depth == currentDepth)
        return 1;
    else if ((clock() - float(startTime)) / CLOCKS_PER_SEC >= (timeLimit - 0.1))
        return 2;
    else if ((jumps.size() + moves.size()) == 0)
        return 3;
    return 0;
}

// Helper function to copy contents of one array into the second
void copyArr(int (&state)[8][8], int (&stateCopy)[8][8]){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            stateCopy[i][j] = state[i][j];   
        }
    }
}


// Wrapper function for Mini Max Algorithm
int CheckersBoard::miniMax(){
    array<int, 3> max;
    array<int, 3> tmp;
    vector<int> bestOptions; 

    startTime = clock();
    bool debug = false;


    // Get stats on current board (for end game handling...)
    int pieces = 0;
    int winning = 0;
    int redKings = 0;
    int blueKings = 0;  
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (board[i][j] != 0){
                winning += board[i][j];
                pieces++;
                if (board[i][j] == -2)
                    redKings++;
                if (board[i][j] == 2)
                    blueKings++;
            }
        }
    }

    int turn = this->turn;

    // For iterative deepening - call maxValue again incrementing how deep we can go 
    while(true){
        int state[8][8];
        copyArr(board, state);
        if (turn == 1)
            tmp = maxValue(state, INT_MIN, INT_MAX, 0, debug);
        else if (turn == -1)
            tmp = minValue(state, INT_MIN, INT_MAX, 0, debug);
        if (tmp[2] == 2){
            cout << "Time limit reached. ";
            break;
        }
        max = tmp;
        if (tmp[2] == 3){
            sleep(2);
            break;
        }
        currentDepth++;
        if ((clock() - float(startTime)) / CLOCKS_PER_SEC >= (timeLimit - 0.1) / 1.5)
            break;
        
        // Keep track of all the outputs from each level search
        if (pieces <= 7 && (sign(winning) == sign(this->turn)) && (redKings + blueKings == 3))
            bestOptions.push_back(max[1]);
    }

    // If there isnt a definite win, use the mode of all the searched result moves
    // For 2K vs 1K scenario 
    if (max[2] != 3 && (pieces <= 7) && (sign(winning) == sign(this->turn)) && (redKings + blueKings == 3)){
        map<int, int> m;
        for (int i = 0; i < bestOptions.size(); i++){
            if (m.find(bestOptions[i]) == m.end()){
                m[bestOptions[i]] = 0;
            } else{
                m[bestOptions[i]]++;
            }

        }

        map<int, int>::iterator currentEntry;
        pair<int, int> entryWithMaxValue = make_pair(0, 0);
        for (currentEntry = m.begin(); currentEntry != m.end(); ++currentEntry) {
    
            // If this entry's value is more than the max value,
            // set this entry as the max
            if (currentEntry->second > entryWithMaxValue.second) {
                entryWithMaxValue = make_pair(currentEntry->first, currentEntry->second);
            }
        }

        // Update option choice wiht mode
        max[1] = entryWithMaxValue.first;
    }

    cout << "Time spent searching to depth " << currentDepth << ": " << (clock() - float(startTime)) / CLOCKS_PER_SEC << " seconds. ";
    cout << "Evaluation: " << max[0] << "\n";
    currentDepth = 1;

    return max[1];
}

// Max Value for minimax algorithm
array<int, 3> CheckersBoard::maxValue(int (&state)[8][8], int alpha, int beta, int depth, bool debug){
    int stateCopy[8][8];
    int playerTurn = 1;
    // Vectors to store each move
    vector<vector<dataItem>> jumps;
    vector<dataItem> moves; 
    int jump = getMoves(state, jumps, moves, 1);
    array<int, 3> v;

    // Trace output of searched tree 
    if (debug){
        cout << "Board for depth: " << depth << " for player " << playerTurn << "\n";
        cout << "Evaluation: " << eval(state, playerTurn, 0, depth) << '\n';
        printBoard(state);
        printOptions(jumps, moves, turn);
    }


    if (int cut = isCutOff(depth, jumps, moves)){
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, playerTurn, cut, depth);
        v[1] = 0;
        v[2] = cut;
        if (debug){
            cout << "EVAL: " << v[0] << " for player " << playerTurn << "\n";
            cout << "DEPTH: " << depth << ", depth limit " << " cutoff = " << cut << "\n";
        }
        return v;
    }

    // Iterate through all possible actions / children
    v = {INT_MIN, 0};
    array<int, 3> min_v;
    int size = moves.size();
    if(jumps.size() > 0){
        size = jumps.size();
    } 
    for (int i = 0; i < size; i++){
        // Make a copy of the board state
        copyArr(state, stateCopy);
        int copy = playerTurn;
        movePiece(i, jump, stateCopy, copy, jumps, moves);
        min_v = minValue(stateCopy, alpha, beta, depth + 1, debug);
        if (min_v[0] > v[0]){
            v[0] = min_v[0];
            v[1] = i;
            v[2] = min_v[2];
            alpha = max(alpha, v[0]);
        }
        if (v[0] >= beta){
            jumps.clear();
            moves.clear();
            v[0]+=10;
            v[1] = i;
            v[2] = min_v[2];

            if (debug){
                cout << "v = " << v[0] << " Beta = " << beta << "\n";
                cout << "EVAL: " << eval(state, playerTurn, 0, depth) << " for player " << playerTurn << "\n";
                cout << "DEPTH: " << depth << ", PRUNE " << "\n";
                printBoard(stateCopy);
            }
            return v;
        }
    }
    jumps.clear();
    moves.clear();

    if (debug){
        cout << "EVAL: " << v[0] << " for player " << playerTurn << "\n";
        cout << "DEPTH: " << depth << "\n";
        printBoard(stateCopy);
    }
    return v;

}

// Similar function to maxvalue...
array<int, 3> CheckersBoard::minValue(int (&state)[8][8], int alpha, int beta, int depth, bool debug){
    int stateCopy[8][8];
    int playerTurn = -1;
    // Vectors to store each move
    vector<vector<dataItem>> jumps;
    vector<dataItem> moves; 
    int jump = getMoves(state, jumps, moves, playerTurn);
    array<int, 3> v;

    if (debug){
        cout << "Board for depth: " << depth << " for player " << playerTurn << "\n";
        cout << "Evaluation: " << eval(state, playerTurn, 0, depth) << '\n';
        printBoard(state);
        printOptions(jumps, moves, turn);
    }

    if (int cut = isCutOff(depth, jumps, moves)){
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, playerTurn, cut, depth);
        v[1] = 0;
        v[2] = cut;
        if (debug){
            cout << "EVAL: " << v[0] << " for player " << playerTurn << "\n";
            cout << "DEPTH: " << depth << ", Depth limit" << " cutoff = " << cut <<"\n";
            printBoard(state);
        }
        return v;
    }

    // Iterate through all possible actions / children
    v = {INT_MAX, 0};
    array<int, 3> min_v;
    int size = moves.size();
    if(jumps.size() > 0){
        size = jumps.size();
    }
    for (int i = 0; i < size; i++){
        // Make a copy of the board state
        copyArr(state, stateCopy);
        // Get the resulting state for each move (stored in stateCopy)
        int copy = playerTurn;
        movePiece(i, jump, stateCopy, copy, jumps, moves);
        min_v = maxValue(stateCopy, alpha, beta, depth + 1, debug);
        // if (min_v[0] < v[0] || (min_v[0] == v[0] && rand() % 4 == 0) || min_v[2] == 1){
        if (min_v[0] < v[0]){
            v[0] = min_v[0];
            v[1] = i;
            v[2] = min_v[2];
            beta = min(beta, v[0]);
        }
        if (v[0] <= alpha){
            jumps.clear();
            moves.clear();
            v[0]-=10;
            v[1] = i;
            v[2] = min_v[2];
            if (debug){
                cout << "v = " << v[0] << " alpha = " << alpha << "\n";
                cout << "EVAL: " << v[0] << " for player " << playerTurn << "\n";
                cout << "DEPTH: " << depth << ", PRUNE" << "\n";
                printBoard(stateCopy);
            }
            return v;
        }
    }
    
    if (debug){
        cout << "EVAL: " << v[0] << " for player " << playerTurn << "\n";
        cout << "DEPTH: " << depth << "\n";
        printBoard(stateCopy);
    }
    jumps.clear();
    moves.clear();
    return v;

}