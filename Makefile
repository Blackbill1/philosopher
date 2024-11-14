# Nom de l'exécutable final
NAME = philo

# Compilateur et flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I $(INCDIR)

# Dossiers
SRCDIR = src
INCDIR = inc
OBJDIR = obj

# Récupération des fichiers sources
SRC = inits.c \
		main.c\
		utils.c\
		actions.c\
		libscam.c\
		threads.c\


# Chemin complet pour les fichiers objets dans obj/
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

# Règle par défaut
all: $(NAME)

# Compilation de l'exécutable avec les bibliothèques
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "Compilation de $(NAME) terminée."

# Compilation des fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier obj si nécessaire
$(OBJDIR):
	@mkdir -p $(OBJDIR)


debug: CFLAGS += -fsanitize=thread
debug: re

leak: CFLAGS += -fsanitize=address
leak: re

# Nettoyage des fichiers objets
clean:
	@rm -rf $(OBJDIR)
	@echo "Nettoyage des fichiers objets."

# Nettoyage complet (objets + exécutable + bibliothèques)
fclean: clean
	@rm -f $(NAME)
	@echo "Nettoyage complet."

# Recompile tout depuis zéro
re: fclean all

# Vérification de la norme avec norminette
norm:
	@echo "Norminette de $(NAME) dans $(SRCDIR) et $(INCDIR)..."
	@if norminette $(SRCDIR)/*.c $(INCDIR)/*.h | grep -v "OK!" | grep -q "Error!"; then \
		norminette $(SRCDIR)/*.c $(INCDIR)/*.h | grep -v "OK!" | \
		while read line; do \
			if echo $$line | grep -q "Error!"; then \
				echo "\033[0;31m$$line\033[0m"; \
			else \
				echo "$$line"; \
			fi; \
		done; \
	else \
		echo "\033[0;32mAll files are norminette friendly !\033[0m"; \
	fi

# Ignorer les erreurs sur ces règles
.PHONY: all clean fclean re norm debug leak
