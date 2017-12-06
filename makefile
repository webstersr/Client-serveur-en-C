CPPFLAGS= -I../headers
EXEC= run
#on recupère nos .c et .h
XRC=$(wildcard ../*)
SRC=$(wildcard ./*)
HDR=$(wildcard ./*)
#generation des .o
BIN=$(SRC:.c=.o)

all: $(EXEC)

#on utilise la regle implicite .c.o
$(EXEC):

$(EXE): $(BIN)
	$(CC) $(BIN) -o $(EXEC)

#on fait dépendre chaque .o de chaque point .h
#si un .h est modifié tout est recompilé !
$(OBJ)::$(HDR)

#si il y a un fichier de meme nom dans /src la regle n'est pas executé
#.PHONY reconstruit les dépendances
.PHONY: run
