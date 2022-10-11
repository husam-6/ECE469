#include "board.h"
#include <unordered_map>

using namespace std;

int main(){
    // CheckersBoard test = CheckersBoard("none", 1); 

    //Prompt user for input text file representing board
    string fileName;
    string player;
    string gameState;
    
    cout << "Would you like to load in a game state? \n(Y/N):";
    cin >> gameState; 
    if(gameState == "Y")
    {
        cout << "Enter a text file to load in a previous game: ";
	    cin >> fileName;
    }
    else{
        fileName = "startGame.txt";
        cout << "Enter which player should start. \nEnter 1 for player BLUE and 2 for player RED: ";
        cin >> player;
    }
    
    int color = 1;
    if (player == "2"){
        color = -1;
    }
    CheckersBoard test = CheckersBoard(fileName, color); 
    test.printBoard();
    test.printOptions();
    return 0;
}