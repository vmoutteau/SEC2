# Ce Makefile est là pour vous aider 
# Vous pouvez le modifier, ajouter des règles, en enlever ...
# Vous pouvez ne pas vous en servir, mais ce serait un tort

# Compilateur a utilliser
CC=gcc 

# Fichier à contruire
EXE=minishell9

# Quelles options pour le compilateur ? 
CFLAGS=-Wall -Wextra

# Options pour l'édition de liens
LDFLAGS=

# Nom du fichier de test
TEST_FILE=test_readcmd

# Les fichiers .o nécessaires pour contruire le fichier EXE :
# Ils sont obtenus à partir de tous les fichiers .c du répertoire auquel on enlève le programme de test
OBJECTS = $(patsubst %c,%o,$(filter-out minishell.c test_readcmd.c sleep.c test_liste_jobs.c, $(wildcard *.c)))

all: $(EXE)

test: $(TEST_FILE)

test2: test_liste_jobs

$(EXE): $(OBJECTS)

$(TEST_FILE): test_readcmd.o readcmd.o

test_liste_jobs: test_liste_jobs.o liste_jobs.o
	$(CC) $^ -o $@

liste_jobs.o: liste_jobs.c liste_jobs.h
	$(CC) $(CFLAGS) -c liste_jobs.c

clean:
	\rm -f *.o *~
	\rm -f $(EXE)
	\rm -f $(TEST_FILE)
	\rm -f test_liste_jobs
	\rm -f sleep

archive: clean
	(cd .. ; tar cvf minishell-`whoami`.tar minishell)

help:
	@echo "Makefile for minishell."
	@echo "Targets:"
	@echo " all             Build the minishell"
	@echo " archive	 Archive the minishell"
	@echo " clean           Clean artifacts"