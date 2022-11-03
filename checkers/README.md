# ECE469 Artificial Intelligence Project 1
## Checkers
## Husam Almanakly


### Overview
This project implements a checkers program that allows the user to play either
against a computer, another human, or pit the computer against itself. 


### Instructions
To load in a game state, please apply the following conventions. Create a sample
text file as follows:

<pre>
 0 0 0 0
3 0 0 0 
 4 2 3 0
0 0 0 0 
 4 2 4 0
0 0 0 0 
 1 4 2 0 
0 0 3 0 
1 
10
</pre>

The above example includes 8 rows of 8 characters - either a space, 0, or integer
between 1-4. 1 & 2 represent BLUE pawns and RED pawns (respectively), 3 & 4 represent
BLUE kings and RED kings (respectively). The last two lines indicate which players turn (line 8)
and the time limit specified for the computer. By convention, RED will always attack going UP,
BLUE going DOWN. Spaces will not affect the program, but is included here for readability.

If you would just like to start the game at a regular state, select N when prompted
if you want to load in a game and ensure "startGame.txt" is in the same directory as the executable.

This program allows for 3 modes as a user - Computer vs Computer, Computer vs Human, and Human vs Human.
By convention, I require the computer to always play as player 1 if specified. In other words, for the Computer
vs Human case, the computer is always player 1 (Blue) and the human is always player 2 (RED). Player 1 always
attacks going 'down' the board, player 2 going 'up'. 

### Sample Run

Compile an executable by running make. Here is a sample run with the output provided

<pre>
➜  checkers git:(main) make
g++ -c -std=c++17 board.cpp
g++ -c -std=c++17 main.cpp
g++ -c -std=c++17 game.cpp
g++ -c -std=c++17 minimax.cpp
g++ -o checkers.exe main.o board.o game.o minimax.o
➜  checkers git:(main) ./checkers.exe
Would you like to load in a game state? (Y/N): N
Enter which player should start.
Enter 1 for player BLUE and 2 for player RED: 2
Enter a time limit (in seconds): 5
Who's playing? Choose Computer vs Computer, Human vs Computer, or Human vs Human (0 / 1 / 2): 0

</pre>

The output of an initial board should print with the options for the given player (in this case red)
Choosing 0 will play the computer against itself. Resize your terminal window to fit the board and options in 
a single screen and upon each move the board position should remain the same. 

Below is a screenshot sample run:

![alt text](https://github.com/husam-6/ECE469/blob/main/checkers/sampleRun.png?raw=true)
