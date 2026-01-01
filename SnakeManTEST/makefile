SnakeMAN: main.o piece.o classement.o sauvegarde.o jeu.o
	gcc -o SnakeMAN main.o piece.o classement.o sauvegarde.o jeu.o `pkg-config --libs-only-other --libs-only-L --libs-only-l MLV`

main.o: main.c piece.h  classement.h sauvegarde.h jeu.h
	gcc -c -Wall -W -std=c89 -pedantic -O2 `pkg-config --cflags MLV` main.c -o main.o

piece.o: piece.h piece.c
	gcc -c -Wall -W -std=c89 -pedantic -O2 `pkg-config --cflags MLV` piece.c -o piece.o

classement.o: classement.h classement.c
	gcc -c -Wall -W -std=c89 -pedantic -O2 `pkg-config --cflags MLV` classement.c -o classement.o

sauvegarde.o: sauvegarde.h sauvegarde.c
	gcc -c -Wall -W -std=c89 -pedantic -O2 `pkg-config --cflags MLV` sauvegarde.c -o sauvegarde.o

jeu.o: jeu.h jeu.c
	gcc -c -Wall -W -std=c89 -pedantic -O2 `pkg-config --cflags MLV` jeu.c -o jeu.o -lm

clean:
	rm -rf *.o *~ SnakeMAN