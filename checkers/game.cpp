#include "game.h"
#include "board.h"
#include<unistd.h>

using namespace std;

Game::Game(){
    //Prompt user for input text file representing board
    string fileName;
    string player;
    string gameState;
    string timeLim;
    int lim = 5;
    
    // Get file name if loading in a state
    cout << "Would you like to load in a game state? (Y/N): ";
    cin >> gameState; 
    if(gameState == "Y")
    {
        cout << "Enter a text file to load in a previous game: ";
	    cin >> fileName;
    }
    else{
        // If not, load in a regular game and prompt user asking which player should start
        fileName = "startGame.txt";
        cout << "Enter which player should start. \nEnter 1 for player BLUE and 2 for player RED: ";
        cin >> player;
        while(true){
            try{
                cout << "Enter a time limit (in seconds): ";
                cin >> timeLim;
                lim = stoi(timeLim);
                break;
            }
            catch(...){
                cout << "Enter a valid time..." << "\n";
            }
        }
    }

    // Convert color to match convention in CheckersBoard class
    // Initialize as blue start unless specified
    int color = 1;
    if (player == "2"){
        color = -1;
    }

    int numHumansInput;
    cout << "Who's playing? Choose Computer vs Computer, Human vs Computer, or Human vs Human (0 / 1 / 2): ";
    cin >> numHumansInput;
    
    numHumans = numHumansInput;
    playerStart = color;

    board = CheckersBoard(fileName, color, lim);
    
    // In case a board state was loaded in...
    playerStart = board.getTurn();
}


// Make program pick a move (random for now)
int Game::computerMove(int * type){
    int numOptions = board.getNumberOfOptions();
    int choice;
    if (numOptions == 1){
        sleep(2);
        choice = 0;
    }
    else
        choice = board.miniMax();
    // int choice = rand() % numOptions;
    cout << "Computer choosing option: " << choice << "\n";
    board.movePiece(choice, (*type));
    board.printBoard();
    (*type) = board.printOptions();
    return 0; 
}


int Game::playGame(){
    int type, option, move;

    // For Human vs Human
    // Initialize loop
    board.printBoard();
    type = board.printOptions();

    // Keep making moves until no moves are possible (for either player) 
    while (board.testEnd()){
        // Make computer play as -1 by default.
        // If one computer is playing...
        if (board.getTurn() == 1 && numHumans <= 1){
            computerMove(&type);
            continue;
        }

        // If both players are computers...
        if (board.getTurn() == -1 && numHumans == 0){
            computerMove(&type);
            continue;
        }

        // Otherwise, human picks a move (numHumans == 2)
        cout << "Pick a move: ";
        cin >> option;
        
        move = board.movePiece(option, type);
        if (move == -1)
            continue;
        board.printBoard();
        type = board.printOptions();
    }

    string win = board.checkWinner();
    cout << win << " WINS!" << "\n";
    return 0;
}