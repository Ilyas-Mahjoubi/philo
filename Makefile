CC = cc
NAME = philo
CFLAGS = -Wall -Wextra -Werror -g -pthread
INCLUDES = -I.

SRCDIR = source
OBJ_DIR = obj

SRC =   $(SRCDIR)/main.c \
		$(SRCDIR)/init.c \
		$(SRCDIR)/parssing.c \
		$(SRCDIR)/routine.c \
		$(SRCDIR)/utlis.c

OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

GREEN = \033[0;32m
YELLOW = \033[0;33m
PINK = \033[4;35m
RESET = \033[0m

define print_progress
	@echo "$(PINK)$1...$(RESET)"
endef

all: $(NAME)

$(NAME): $(OBJECTS)
	$(call print_progress,"Linking $(NAME)")
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
	@echo "$(YELLOW)Compiling: $(notdir $<)$(RESET)"

clean:
	$(call print_progress,"Cleaning object files")
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)Clean completed!$(RESET)"

fclean: clean
	$(call print_progress,"Removing executable")
	@rm -f $(NAME)
	@echo "$(GREEN)Full clean completed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re