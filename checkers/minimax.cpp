#include "board.h"

using namespace std;

// Helper function for evaluation function
int sign(int num){
    if (num > 0) return 1;
    if (num < 0) return -1;
    return 0;
}

// Find the distance between two coordinates
float distance(int x1, int y1, int x2, int y2){
    int x_dist, y_dist;

    x_dist = (x1 - x2);
    x_dist *= x_dist;
    y_dist = (y1 - y2);
    y_dist *= y_dist;
    if (x_dist + y_dist > 0) 
        return sqrt(x_dist + y_dist);
    else
        return 0;
}

// Heuristic evaluation of the board state
// NEED TO ACCOUNT FOR DEFINITE WINS!!!!!!!
int CheckersBoard::eval(int (&state)[8][8], int playerTurn, int cut){
    // RED is always positive (MAX), BLUE is always negative (MIN)
    int pawn = 300000;
    int king = 500000;
    int promotion = 10000;
    int goalie = 1000;
    int trades = 100;
    int numRed = 0;
    int numBlue = 0;
    float maxDistRed = 0;
    float maxDistBlue = 0; 

    // Add bonus to protect your back rank

    // Simple heuristic for now - 5 * kings + 3 * pawns
    int tmp;
    int weight = 0;
    int counter = 0;
    int pieceCounter = 0; 
    int kpTotal = 0;
    int aveDistRed = 0;
    int aveDistBlue = 0;
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
                    aveDistRed += i;
                    numRed++;
                    if (i == 7){
                        numRedGoalies+=3;
                        if (j == 2 || j == 4)
                            numRedGoalies+=5; 
                    }
                }
                else if (tmp > 0){
                    aveDistBlue += (8 - i);
                    numBlue++;
                    if (i == 0){
                        numBlueGoalies+=3;
                        if (j == 2 || j == 4)
                            numBlueGoalies+=5; 
                    }
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

    int ties = rand() % 11 - 5;
    int ret = kpTotal + ties;
    
    // Bonus for having pieces on the back rank
    ret += goalie * numBlueGoalies;
    ret -= goalie * numRedGoalies; 
    
    // Emphasize trades... Lower total number of pieces is better (if winning)! 
    int reward = 24 - pieceCounter;

    // Check whose currently winning
    // If MAX / BLUE is winning, higher eval for lower pieces
    if (kpTotal > 0)
        ret += trades * reward;

    // // If MIN / RED is winning, lower eval for lower pieces
    else if (kpTotal < 0)
        ret -= trades * reward; 

    // Add value for how far piece is from promotion... (for Red, index is distance to back rank, 
    // for Blue, 8 - index gives us this)
    ret -= promotion * aveDistBlue;
    ret += promotion * aveDistRed;  

    // Trying to account for definite wins / losses (this is still flawed)
    if (cut == 3 && playerTurn == -1 && this->turn == 1){
        ret += (1000000 - 10 * currentDepth);
    }
    if (cut == 3 && playerTurn == 1 && this->turn == -1){
        ret -= (1000000 + 10 * currentDepth);
    }
    return ret;
}

// Function to check if the current state is a cutoff state
// Returns 0 if it is not a cut off state
// Returns 1 if the depth limit has been reached
// Returns 2 if time limit has been reached
// Returns 3 if no valid moves are left
int CheckersBoard::isCutOff(int depth, vector<vector<dataItem>> &jumps, vector<dataItem> &moves){
    if (depth == currentDepth){
        return 1;
    }
    else if ((clock() - float(startTime)) / CLOCKS_PER_SEC >= (timeLimit - 0.1)){
        return 2;
    }
    else if ((jumps.size() + moves.size()) == 0){
        return 3;
    }
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
    startTime = clock();
    bool debug = false;

    int turn = this->turn;

    // For iterative deepening - call maxValue again incrementing how deep we can go 
    while(true){
        int state[8][8];
        copyArr(board, state);
        if (turn == 1)
            tmp = maxValue(state, INT_MIN, INT_MAX, 0, debug);
        else if (turn == -1)
            tmp = minValue(state, INT_MIN, INT_MAX, 0, debug);
        if (!tmp[2])
            max = tmp;
        else
            break;
        currentDepth++;
        if ((clock() - float(startTime)) / CLOCKS_PER_SEC >= (timeLimit - 0.1) / 1.5)
            break;
    }

    cout << "Time spent searching to depth " << currentDepth << ": " << (clock() - float(startTime)) / CLOCKS_PER_SEC << " seconds" << "\n";
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
        cout << "Evaluation: " << eval(state, playerTurn, 0) << '\n';
        printBoard(state);
        printOptions(jumps, moves, turn);
    }


    if (int cut = isCutOff(depth, jumps, moves)){
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, playerTurn, cut);
        // v[0] = evaluate(state, playerTurn);
        v[1] = 0;
        v[2] = 0;
        if (cut == 2)
            v[2] = 1;
        if (debug){
            cout << "EVAL: " << eval(state, playerTurn, 0) << " for player " << playerTurn << "\n";
            cout << "DEPTH: " << depth << ", depth limit " << "\n";
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
                cout << "EVAL: " << eval(state, playerTurn, 0) << " for player " << playerTurn << "\n";
                cout << "DEPTH: " << depth << ", PRUNE " << "\n";
                printBoard(stateCopy);
            }
            return v;
        }
    }
    jumps.clear();
    moves.clear();

    if (debug){
        cout << "EVAL: " << eval(state, playerTurn, 0) << " for player " << playerTurn << "\n";
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
        cout << "Evaluation: " << eval(state, playerTurn, 0) << '\n';
        printBoard(state);
        printOptions(jumps, moves, turn);
    }

    if (int cut = isCutOff(depth, jumps, moves)){
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, playerTurn, cut);
        // v[0] = evaluate(state, playerTurn);
        v[1] = 0;
        v[2] = 0;
        if (cut == 2)
            v[2] = 1;
        if (debug){
            cout << "EVAL: " << eval(state, playerTurn, 0) << " for player " << playerTurn << "\n";
            cout << "DEPTH: " << depth << ", Depth limit" << "\n";
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
    // cout << "GET HERE" << '\n' << "SIZE = " << size << '\n';
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
                cout << "EVAL: " << eval(state, playerTurn, 0) << " for player " << playerTurn << "\n";
                cout << "DEPTH: " << depth << ", PRUNE" << "\n";
                printBoard(stateCopy);
            }
            return v;
        }
    }
    
    if (debug){
        cout << "EVAL: " << eval(state, playerTurn, 0) << " for player " << playerTurn << "\n";
        cout << "DEPTH: " << depth << "\n";
        printBoard(stateCopy);
    }
    jumps.clear();
    moves.clear();
    return v;

}