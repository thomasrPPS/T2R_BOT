# Modèle de fichier Makefile, à adapter pour le TP

# options de compilation
CC = gcc
CCFLAGS = -Wall
LIBS =
LIBSDIR =

# fichiers du projet
SRC = clientAPI.c TicketToRideAPI.c BZH_VIE.c move.c
OBJ = $(SRC:.c=.o)
EXEC = run.out

# règle initiale
all: $(EXEC)

# dépendance des .h
clientAPI.o: clientAPI.h
TicketToRideAPI.o: clientAPI.h TicketToRideAPI.h
BZH_VIE.o: TicketToRideAPI.h move.h game.h
move.o: game.h move.h
# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)

# règles supplémentaires
clean:
	rm -f *.o
mproper:
	rm -f $(EXEC) *.o
