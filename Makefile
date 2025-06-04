# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilmahjou <ilmahjou@student.42firenze.it>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/24 02:29:03 by ilmahjou          #+#    #+#              #
#    Updated: 2025/03/05 12:01:44 by ilmahjou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
NAME = push_swap
NAME_BONUS = checker
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I/usr/include -Ilibft
LIBFTDIR = libft/
LIBFT = $(LIBFTDIR)libft.a

SRCDIR = source
OPSDIR = operations
BONUSDIR = bonus
OBJ_DIR = obj
OBJ_SRC_DIR = $(OBJ_DIR)/source
OBJ_OPS_DIR = $(OBJ_DIR)/operations
OBJ_BONUS_DIR = $(OBJ_DIR)/bonus

SRC =   $(SRCDIR)/finded.c \
		$(SRCDIR)/free.c \
		$(SRCDIR)/initiate.c \
		$(SRCDIR)/prepare_stack_a.c \
		$(SRCDIR)/prepare_stack_b.c \
		$(SRCDIR)/parsing.c \
		$(SRCDIR)/push_swap.c \
		$(OPSDIR)/push.c \
		$(OPSDIR)/reverse_rotate.c \
		$(OPSDIR)/rotate.c \
		$(OPSDIR)/turk_algo.c \
		$(OPSDIR)/turk_algo_utils.c \
		$(OPSDIR)/sort_three.c \
		$(OPSDIR)/swap.c

BONUS_SRC = $(BONUSDIR)/checker.c \
			$(SRCDIR)/finded.c \
			$(SRCDIR)/free.c \
			$(SRCDIR)/initiate.c \
			$(SRCDIR)/prepare_stack_a.c \
			$(SRCDIR)/prepare_stack_b.c \
			$(SRCDIR)/parsing.c \
			$(OPSDIR)/push.c \
			$(OPSDIR)/reverse_rotate.c \
			$(OPSDIR)/rotate.c \
			$(OPSDIR)/turk_algo.c \
			$(OPSDIR)/turk_algo_utils.c \
			$(OPSDIR)/sort_three.c \
			$(OPSDIR)/swap.c

OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJ_SRC_DIR)/%.o,$(filter $(SRCDIR)/%, $(SRC))) \
			$(patsubst $(OPSDIR)/%.c,$(OBJ_OPS_DIR)/%.o,$(filter $(OPSDIR)/%, $(SRC)))

BONUS_OBJECTS = $(patsubst $(BONUSDIR)/%.c,$(OBJ_BONUS_DIR)/%.o,$(filter $(BONUSDIR)/%, $(BONUS_SRC))) \
				$(patsubst $(SRCDIR)/%.c,$(OBJ_SRC_DIR)/%.o,$(filter $(SRCDIR)/%, $(BONUS_SRC))) \
				$(patsubst $(OPSDIR)/%.c,$(OBJ_OPS_DIR)/%.o,$(filter $(OPSDIR)/%, $(BONUS_SRC)))

GREEN = \033[0;32m
YELLOW = \033[0;33m
PINK = \033[4;35m
RESET = \033[0m

define print_progress
	@echo "$(PINK)$1...$(RESET)"
endef

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(call print_progress,"Linking $(NAME)")
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBFT)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

$(NAME_BONUS): $(LIBFT) $(BONUS_OBJECTS)
	$(call print_progress,"Linking $(NAME_BONUS)")
	@$(CC) $(CFLAGS) $(BONUS_OBJECTS) -o $(NAME_BONUS) $(LIBFT)
	@echo "$(GREEN)$(NAME_BONUS) created successfully!$(RESET)"

bonus: $(NAME_BONUS)

$(LIBFT):
	$(call print_progress,"Compiling libft")
	@make -sC $(LIBFTDIR)

$(OBJ_SRC_DIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJ_SRC_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
	@echo "$(YELLOW)Compiling: $(notdir $<)$(RESET)"

$(OBJ_OPS_DIR)/%.o: $(OPSDIR)/%.c
	@mkdir -p $(OBJ_OPS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
	@echo "$(YELLOW)Compiling: $(notdir $<)$(RESET)"

$(OBJ_BONUS_DIR)/%.o: $(BONUSDIR)/%.c
	@mkdir -p $(OBJ_BONUS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
	@echo "$(YELLOW)Compiling: $(notdir $<)$(RESET)"

clean:
	$(call print_progress,"Cleaning object files")
	@make -sC $(LIBFTDIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)Clean completed!$(RESET)"

fclean: clean
	$(call print_progress,"Removing executables")
	@make -sC $(LIBFTDIR) fclean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "$(GREEN)Full clean completed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus