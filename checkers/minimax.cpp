#include "board.h"

using namespace std; 

//black is more positive
//red is more negative
//aabbccddee
//aa is a count of all pieces: king is worth a total of 3, regular pieces are 2 (black - red)
//bb is measuring how close a normal piece is to becoming a king, give last row a bonus
//cc is a piece count difference
//dd is a measurement near end game, double corners add a bonus for losing player
//ee is pseudo-random in the case that multiple moves tie for best
int evaluate(int (&arr)[8][8], int playerTurn){
	int a1 = 0, a2 = 0, b = 0, c = 0, d = 0;
	for (int i = 0; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == 1)
			{
				a1 += 2;
				if (i == 0)
					b += 9;
				else b += i;
				c += 1;
			}
			else if (arr[i][j] == -1)
			{
				a2 -=2;
				if (i == 7)
					b -= 9;
				else b -= (7 - i);
				c -= 1;
			}
			else if (arr[i][j] == 2)
			{
				a1 += 3;
				c += 1;
			}
			else if (arr[i][j] == -2)
			{
				a2 -= 3;
				c -= 1;
			}
		}
	// if (c > 0 && a2 >= -8)
	// 	d -= cornerDiagonal('r', 'b');
	// else if (c < 0 && a1 <= 8)
	// 	d += cornerDiagonal('b', 'r');
	a1 *= 100000000;
	a2 *= 100000000;
	b *= 1000000;
	c *= 10000;
	d *= 100;
	int e = rand() % 100;
	if (playerTurn == -1)
		e = -e;
	return a1 + a2 + b + c + d + e;
}


// Heuristic evaluation of the board state
// NEED TO ACCOUNT FOR DEFINITE WINS!!!!!!!
int CheckersBoard::eval(int (&state)[8][8], int playerTurn, int cut){
    // RED is always positive (MAX), BLUE is always negative (MIN)
    int w1 = 300;
    int w2 = 500;
    int tmp;
    int weight = 0;

    // Simple heuristic for now - 5 * kings + 3 * pawns
    int counter = 0;
    int total = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (counter % 2 == 0){
                counter++;
                continue;
            }
            tmp = state[i][j];
            if (abs(tmp) == 1)
                weight = w1;
            else if(abs(tmp) == 2)
                weight = w2; 

            total += weight * tmp;
            counter++;
        }
        counter++;
    }

    int ties = rand() % 4 - 3;
    
    // Trying to account for definite wins / losses
    int ret = total + ties;

    // if (cut == 3 && playerTurn == -1 && this->turn == 1){
    //     ret += 100000;
    // }
    // if (cut == 3 && playerTurn == 1 && this->turn == -1){
    //     ret -= 100000;
    // }
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


// Wrapper function for Mini Max Algorithm
int CheckersBoard::miniMax(){
    array<int, 3> max;
    array<int, 3> tmp;
    startTime = clock();
    bool debug = false;

    int turn = this->turn;

    // For iterative deepening - call maxValue again incrementing how deep we can go 
    while(true){
        // currentDepth = 3;
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
            v[0]++;
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
            v[0]--;
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