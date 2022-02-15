# Colors
# ****************************************************************************

_GREY	= \033[30m
_RED	= \033[31m
_GREEN	= \033[32m
_YELLOW	= \033[33m
_BLUE	= \033[34m
_PURPLE	= \033[35m
_CYAN	= \033[36m
_WHITE	= \033[37m
_END	= \033[0m

# ****************************************************************************

NAME = term3d
SRC_DIR = srcs/
OBJ_DIR = objs/
VPATH = $(SRC_DIR):$(SRC_DIR)utils

# Config
# ****************************************************************************

SHELL = /bin/bash
CC = gcc
INCLUDE = -I includes -I libft
RM = rm -fr

BASEFLAGS = -Wall -Werror -Wextra -MMD -MP
CFLAGS = $(BASEFLAGS) $(INCLUDE)
DEBUG_FLAGS = -g3

# Source files
# ****************************************************************************

SRCS = $(shell find $(SRC_DIR) -name '*.c' | xargs basename -a)
OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

DEPENDS = $(OBJS:.o=.d)

LIBFT_DIR	:= libft
LIBFT		:= libft.a

# Recipe
# ****************************************************************************

all: $(LIBFT) $(NAME)

# C program
$(NAME): $(OBJS) $(LIBFT)
	@printf "$(_END)\nCompiled source files\n"
	$(CC) $(CFLAGS) $(OBJS) $(LIBFLAGS) -o $@ $(LIBFT)
	@printf "$(_GREEN)Finish compiling $(NAME)!\n"
	@printf "Try \"./$(NAME)\" to use$(_END)\n"

$(OBJ_DIR)%.o: %.c
	@if [ ! -d $(OBJ_DIR) ];then mkdir $(OBJ_DIR); fi
	$(CC) $(CFLAGS) -c $< -o $@ 
	@printf "$(_GREEN)█$(_END)"

clean:
	@printf "$(_YELLOW)Removing object files ...$(_END)\n"
	@$(RM) $(OBJ_DIR)
	@$(RM) *.dSYM

fclean:
	@printf "$(_RED)Removing object files and program ...$(_END)\n"
	@$(RM) $(NAME) $(OBJ_DIR)
	@$(RM) *.dSYM

re: fclean all

$(LIBFT):
	make -C $(LIBFT_DIR)
	cp $(LIBFT_DIR)/$(LIBFT) .

debug: CFLAGS += -fsanitize=address $(DEBUG_FLAGS)
debug: re
	@printf "$(_BLUE)Debug build done$(_END)\n"

leak: CFLAGS += $(DEBUG_FLAGS)
leak: re
	@printf "$(_BLUE)Leak check build done$(_END)\n"

test: all
	./$(NAME)

-include $(DEPENDS)

PHONY: all clean fclean re debug leak test

