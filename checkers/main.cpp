#include "board.h"
#include "game.h"
#include <unordered_map>

using namespace std;

int main(){
    // CheckersBoard test = CheckersBoard("none", 1); 

    Game checkers = Game();
    checkers.playGame();

    return 0;
}