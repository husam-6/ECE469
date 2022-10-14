#include "game.h"
#include "board.h"

using namespace std;

Game::Game(){
    //Prompt user for input text file representing board
    string fileName;
    string player;
    string gameState;
    
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
    }

    // Convert color to match convention in CheckersBoard class
    int color = 1;
    if (player == "2"){
        color = -1;
    }

    int numHumansInput;
    cout << "Who's playing? \nChoose Computer vs Computer, Human vs Computer, or Human vs Human (1 / 2 / 3): ";
    cin >> numHumansInput;
    
    numHumans = numHumansInput;
    playerStart = color;

    board = CheckersBoard(fileName, color); 
}

int Game::playGame(){
    int type, option, move;

    cout << board.testEnd() << "\n";

    board.printBoard();
    type = board.printOptions();
    while (board.testEnd()){
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