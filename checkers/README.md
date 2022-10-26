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
between 1-4. 1 & 2 represent BLUE pieces and kings (respectively), 3 & 4 represent
RED pieces and kings. The last two lines indicate which players turn (line 8) and
the time limit specified for the computer. By convention, RED will always attack going UP,
BLUE going DOWN. Spaces will not affect the program, but is included here for readability.

If you would just like to start the game at a regular state, select N when prompted
if you want to load in a game and ensure "startGame.txt" is in the same directory as the executable.

### Sample Run

Compile an executable by running make

<pre>
./checkers.exe

Would you like to load in a game state? (Y/N): N
Enter which player should start.
Enter 1 for player BLUE and 2 for player RED: 2
Who's playing? Choose Computer vs Computer, Human vs Computer, or Human vs Human (0 / 1 / 2): 0

</pre>

The output of an initial board should print with the options for the given player (in this case red)
Choosing 0 will play the computer against itself.