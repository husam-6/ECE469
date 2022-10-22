#include "board.h"

using namespace std; 

// Heuristic evaluation of the board state
// NEED TO ACCOUNT FOR DEFINITE WINS!!!!!!!
int CheckersBoard::eval(int (&state)[8][8], int playerTurn, int cut){

    // RED is always positive (MAX), BLUE is always negative (MIN)
    // int w1 = 3;
    // int w2 = 5;

    // int numPawns, numKings; 
    // numPawns = numKings = 0; 

    // Simple heuristic for now - 5 * kings + 3 * pawns
    int counter = 0;
    int total = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (counter % 2 == 0){
                counter++;
                continue;
            }
            total += state[i][j];
            counter++;
        }
        counter++;
    }
    
    // Trying to account for definite wins
    int ret = playerTurn * total;
    if (cut == 3){
        ret += playerTurn * 1000000;
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
    else if ((clock() - float(startTime)) / CLOCKS_PER_SEC >= (timeLimit - 0.2)){
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

// Helper function for debugging to print the board state
void printArr(int (&state)[8][8]){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            cout << state[i][j] << "\t";   
        }
        cout << "\n";
    }
}


// Wrapper function for Mini Max Algorithm
int CheckersBoard::miniMax(){
    array<int, 3> max;
    array<int, 3> tmp;
    startTime = clock();

    int turn = this->turn;

    // For iterative deepening - call maxValue again incrementing how deep we can go 
    while(true){
        // currentDepth = 3;
        int state[8][8];
        copyArr(board, state);
        if (turn == 1)
            tmp = maxValue(state, INT_MIN, INT_MAX, 0);
        else if (turn == -1)
            tmp = minValue(state, INT_MIN, INT_MAX, 0);
        if (!tmp[2])
            max = tmp;
        else
            break;
        currentDepth++;
    }

    cout << "Time spent searching: " << (clock() - float(startTime)) / CLOCKS_PER_SEC << " seconds" << "\n";
    cout << "Evaluation: " << max[0] << "\n";

    currentDepth = 1;

    return max[1];
}

// Max Value for minimax algorithm
array<int, 3> CheckersBoard::maxValue(int (&state)[8][8], int alpha, int beta, int depth){
    int stateCopy[8][8];
    int playerTurn = 1;
    // Vectors to store each move
    vector<vector<dataItem>> jumps;
    vector<dataItem> moves; 
    int jump = getMoves(state, jumps, moves, 1);
    array<int, 3> v;


    if (int cut = isCutOff(depth, jumps, moves)){
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, playerTurn, cut);
        v[1] = 0;
        v[2] = 0;
        // cout << string(depth, '\t') << "v = " << v[0] << ", depth = " << depth << "\n";
        if (cut == 2)
            v[2] = 1;
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
        movePiece(i, jump, stateCopy, playerTurn, jumps, moves);
        min_v = minValue(stateCopy, alpha, beta, depth + 1);
        if (min_v[0] > v[0] || (min_v[0] == v[0] && rand() % 2 == 0) || min_v[2] == 1){
            v[0] = min_v[0];
            v[1] = i;
            v[2] = min_v[2];
            alpha = max(alpha, v[0]);
        }
        if (v[0] >= beta){
            jumps.clear();
            moves.clear();
            v[0]--;
            v[1] = i;
            v[2] = min_v[2];
            // cout << string(depth, '\t') << "v = " << v[0] << ", option = " << v[1] << ", depth = " << depth << ", PRUNE" << "\n";
            return v;
        }
    }
    jumps.clear();
    moves.clear();
    // cout << string(depth, '\t') << "v = " << v[0] << ", option = " << v[1] << ", depth = " << depth <<  ", PRUNE" << "\n";
    return v;

}

array<int, 3> CheckersBoard::minValue(int (&state)[8][8], int alpha, int beta, int depth){
    int stateCopy[8][8];
    int playerTurn = -1;
    // Vectors to store each move
    vector<vector<dataItem>> jumps;
    vector<dataItem> moves; 
    int jump = getMoves(state, jumps, moves, playerTurn);
    array<int, 3> v;

    if (int cut = isCutOff(depth, jumps, moves)){
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, playerTurn, cut);
        v[1] = 0;
        v[2] = 0;
        if (cut == 2)
            v[2] = 1;
        // cout << string(depth, '\t') << "v = " << v[0] << ", depth = " << depth << '\n';
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
        movePiece(i, jump, stateCopy, playerTurn, jumps, moves);
        min_v = maxValue(stateCopy, alpha, beta, depth + 1);
        if (min_v[0] < v[0] || (min_v[0] == v[0] && rand() % 2 == 0) || min_v[2] == 1){
            v[0] = min_v[0];
            v[1] = i;
            v[2] = min_v[2];
            beta = min(beta, v[0]);
        }
        if (v[0] <= alpha){
            jumps.clear();
            moves.clear();
            v[0]++;
            v[1] = i;
            v[2] = min_v[2];
            // cout << string(depth, '\t') << "v = " << v[0] << ", option = " << i << ", depth = " << depth << ", PRUNE" << "\n";
            return v;
        }
    }
    jumps.clear();
    moves.clear();
    // cout << string(depth, '\t') << "v = " << v[0] << ", option = " << v[1] << ", depth = " << depth << "\n";
    return v;

}