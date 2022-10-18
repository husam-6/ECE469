#include "board.h"
#include <unordered_map>

using namespace std; 



// Heuristic evaluation of the board state
int CheckersBoard::eval(int (&state)[8][8], int turn){

    // RED is always positive (MAX), BLUE is always negative (MIN)
    int w1 = 3;
    int w2 = 5;

    int numPawns, numKings; 
    numPawns = numKings = 0; 

    // Simple heuristic for now - 5 * kings + 3 * pawns
    int counter = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (counter % 2 == 0){
                counter++;
                continue;
            }
            if (state[i][j] == turn)
                numPawns++; 
            else if (state[i][j] == 2 * turn)
                numKings++;
            counter++;
        }
        counter++;
    }

    return turn * (w1 * numPawns + w2 * numKings);
}

int CheckersBoard::isCutOff(int depth, vector<vector<dataItem>> &jumps, vector<dataItem> &moves){
    if (depth == currentDepth){
        return 1;
    }
    else if ((clock() - float(startTime)) / CLOCKS_PER_SEC >= (timeLimit - 0.5)){
        return 1;
    }
    else if ((jumps.size() + moves.size()) == 0){
        return 1;
    }
    return 0;
}

void copyArr(int (&state)[8][8], int (&stateCopy)[8][8]){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            stateCopy[i][j] = state[i][j];   
        }
    }
}


// Wrapper function for Mini Max Algorithm
int CheckersBoard::miniMax(){
    int turn = 1;
    int * max;
    startTime = clock();

    // For iterative deepening - call maxValue again incrementing how deep we can go 
    while(true){
        if ((clock() - float(startTime)) / CLOCKS_PER_SEC >= (timeLimit - 0.5)) // time in seconds
            break;
        max = maxValue(board, INT_MIN, INT_MAX, 0, turn);
        currentDepth++;
        // cout << "Option: " << *(max + 1) << '\n';
    }

    cout << "Time spent searching: " << (clock() - float(startTime)) / CLOCKS_PER_SEC << " seconds" << "\n";

    currentDepth = 0;

    return *(max + 1);
}

// Max Value for minimax algorithm
int * CheckersBoard::maxValue(int (&state)[8][8], int alpha, int beta, int depth, int &turn){
    int stateCopy[8][8];
    // Vectors to store each move
    vector<vector<dataItem>> jumps;
    vector<dataItem> moves; 
    int jump = getMoves(state, jumps, moves);

    if (isCutOff(depth, jumps, moves)){
        static int v[2];
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, turn);
        v[1] = 0;
        return v;
    }

    // Iterate through all possible actions / children
    static int v[2] = {INT_MIN, 0};
    int * min_v;
    int size = moves.size();
    if(jumps.size()){
        size = jumps.size();
    } 
    for (int i = 0; i < size; i++){
        // Make a copy of the board state
        copyArr(state, stateCopy);
        // Get the resulting state for each move (stored in stateCopy)
        movePiece(i, jump, stateCopy, turn, jumps, moves);
        min_v = minValue(stateCopy, alpha, beta, depth + 1, turn);
        if ((*min_v) > v[0] || ((*min_v) == v[0] && rand() % 2 == 0)){
            v[0] = *min_v;
            v[1] = i;
            alpha = max(alpha, v[0]);
        }
        if (v[0] >= beta){
            jumps.clear();
            moves.clear();
            v[0]--;
            v[1] = i;
            return v;
        }
    }
    jumps.clear();
    moves.clear();
    return v;

}

int * CheckersBoard::minValue(int (&state)[8][8], int alpha, int beta, int depth, int &turn){
    int stateCopy[8][8];
    // Vectors to store each move
    vector<vector<dataItem>> jumps;
    vector<dataItem> moves; 
    int jump = getMoves(state, jumps, moves);

    if (isCutOff(depth, jumps, moves)){
        static int v[2];
        // Pass back up the evaluation and its corresponding 'option' (ie which move is associated)
        v[0] = eval(state, turn);
        v[1] = 0;
        return v;
    }

    // Iterate through all possible actions / children
    static int v[2] = {INT_MAX, 0};
    int * min_v;
    int size = moves.size();
    if(jumps.size()){
        size = jumps.size();
    }
    for (int i = 0; i < size; i++){
        // Make a copy of the board state
        copyArr(state, stateCopy);

        // Get the resulting state for each move (stored in stateCopy)
        movePiece(i, jump, stateCopy, turn, jumps, moves);
        min_v = maxValue(stateCopy, alpha, beta, depth + 1, turn);
        if ((*min_v) < v[0] || ((*min_v) == v[0] && rand() % 2 == 0)){
            v[0] = *min_v;
            v[1] = i;
            beta = min(beta, v[0]);
        }
        if (v[0] <= alpha){
            jumps.clear();
            moves.clear();
            v[0]++;
            v[1] = i;
            return v;
        }
    }
    jumps.clear();
    moves.clear();
    return v;

}