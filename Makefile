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
INCLUDE = -I includes
RM = rm -fr

BASEFLAGS = -Wall -Werror -Wextra -MMD -MP
CFLAGS = $(BASEFLAGS) $(INCLUDE)
DEBUG_FLAGS = -g3

# Source files
# ****************************************************************************

FILES = ft_raw_split.c \
        ft_split.c \
        main.c \
        rd_file.c \
        t3_affine.c \
        t3_affine_compose.c \
        t3_double.c \
        t3_glyph.c \
        t3_glyph_allocate.c \
        t3_loop.c \
        t3_math.c \
        t3_read_3d.c \
        t3_render.c \
        t3_reset_params.c \
        t3_scan_message.c \
        t3_setup.c \
        t3_timing.c \
        t3_ui.c \
        t3_update_animate.c \
        t3_update_axis.c \
        t3_update_flame.c \
        t3_update_offset.c \
        t3_update_omega.c \
        t3_update_scale.c

SRCS = $(addprefix $(SRC_DIR), $(FILES))
OBJS = $(addprefix $(OBJ_DIR), $(FILES:.c=.o))

DEPENDS = $(OBJS:.o=.d)

# Recipe
# ****************************************************************************

all: $(NAME)

# C program
$(NAME): $(OBJS)
	@printf "$(_END)\nCompiled source files\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFLAGS) -o $@ $(LIBFT)
	@printf "$(_GREEN)Finish compiling $(NAME)!\n"
	@printf "Try \"./$(NAME)\" to use$(_END)\n"

$(OBJ_DIR)%.o: %.c
	@if [ ! -d $(OBJ_DIR) ];then mkdir $(OBJ_DIR); fi
	@$(CC) $(CFLAGS) -c $< -o $@
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

debug: CFLAGS += -fsanitize=address $(DEBUG_FLAGS)
debug: re
	@printf "$(_BLUE)Debug build done$(_END)\n"

leak: CFLAGS += $(DEBUG_FLAGS)
leak: re
	@printf "$(_BLUE)Leak check build done$(_END)\n"

norm:
	@python3 -m norminette srcs includes

test: all
	./$(NAME)

-include $(DEPENDS)

PHONY: all clean fclean re debug leak test norm
