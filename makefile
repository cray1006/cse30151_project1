all:  nfa

nfa:  main.o nfa.o
	g++ -o nfa main.o nfa.o -Wall

main.o:  main.cpp nfa.h
	g++ -c main.cpp -Wall

nfa.o:  nfa.cpp nfa.h
	g++ -c nfa.cpp -Wall

clean:
	rm -f *.o nfa
