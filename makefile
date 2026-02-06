# Compilateur
CC = gcc

# Flags GTK
GTK_FLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

# Nom de l'exécutable
EXEC = metro

# Fichiers sources
SRCS = main.c backend.c frontend.c

# Règle par défaut
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(SRCS)
	$(CC) $(SRCS) -o $(EXEC) $(GTK_FLAGS) $(GTK_LIBS)

# Compilation et exécution
run: $(EXEC)
	./$(EXEC)

# Nettoyage
clean:
	rm -f $(EXEC)

# Recompilation complète
rebuild: clean all
