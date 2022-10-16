#include "board.h"
#include "game.h"
#include <unordered_map>
#include<time.h>

using namespace std;

int main(){

    srand(time(NULL));
    Game checkers = Game();
    checkers.playGame();

    return 0;
}