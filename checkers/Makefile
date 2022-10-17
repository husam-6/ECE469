checkers.exe: board.o main.o game.o minimax.o
	g++ -o checkers.exe main.o board.o game.o minimax.o

main.o: main.cpp
	g++ -c -std=c++17 main.cpp

board.o: board.cpp board.h
	g++ -c -std=c++17 board.cpp

game.o: game.cpp game.h
	g++ -c -std=c++17 game.cpp

minimax.o: minimax.cpp board.h
	g++ -c -std=c++17 minimax.cpp

debug:
	g++ -g -o checkers.exe board.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
