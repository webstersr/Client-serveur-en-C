CPPFLAGS= -I../headers
EXEC= ../bin/test
#on recupère nos .c et .h
SRC=$(wildcard ./*.c)
HDR=$(wildcard ../headers/*.h)
#generation des .o
BIN=$(SRC:.c=.o)

all: $(EXEC)

#on utilise la regle implicite .c.o
$(EXEC): $(BIN)
	$(CC) $(BIN) -o $(EXEC)
	mv *.o ../bin/

#on fait dépendre chaque .o de chaque point .h
#si un .h est modifié tout est recompilé !
$(OBJ)::$(HDR)

#si il y a un fichier de meme nom dans /src la regle n'est pas executé
#.PHONY reconstruit les dépendances
.PHONY: clean run
run:
	$(EXEC) $(SOCKET)
clean:
	rm ../bin/*.o
