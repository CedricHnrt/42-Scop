
#COLOR
_GREEN = \033[92m
_YELLOW = \033[33m
_RED = \033[31m

#POLICE
_END = \033[0m
_BOLD = \033[1m

NAME = scop
CC = @c++
INCLUDES =	-Iinclude/

C++FLAGS = -Wall -Wextra -Werror $(INCLUDES) -std=c++11 -MMD -MP
RM = @rm -rf
MKDIR = @mkdir -p
PRINT = @echo
FILES =	main		\
		ObjectData
OBJ_DIR = obj/
BIN_DIR = bin/

OBJ = $(addsuffix .o, $(addprefix $(OBJ_DIR), $(FILES)))
DEP = $(addsuffix .d, $(addprefix $(OBJ_DIR), $(FILES)))

$(NAME): $(OBJ)
	$(MKDIR) $(BIN_DIR)
	$(PRINT) "\n${_YELLOW}Making $(NAME)...${_END}"
	$(CC) $(OBJ) -o $(BIN_DIR)$(NAME)
	$(PRINT) "${_BOLD}${_GREEN}$(NAME) done.\a${_END}"

obj/%.o: src/%.cpp Makefile
	$(MKDIR) $(OBJ_DIR)
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) -c $(C++FLAGS) $< -o $@

all: $(NAME)

clean:
ifneq ($(strip $(wildcard $(OBJ))),)
	$(PRINT) "\n${_RED}Cleaning Objects...${_END}"
	$(RM) $(OBJ_DIR)
	$(PRINT) "${_GREEN}Objects cleaned.${_END}"
endif

fclean: clean
ifeq ($(wildcard $(BIN_DIR)$(NAME)), $(BIN_DIR)$(NAME))
	$(PRINT) "${_RED}Deleting $(NAME)...${_END}"
	$(RM) $(BIN_DIR)
	$(PRINT) "${_GREEN}$(NAME) deleted.\a${_END}"
endif

re: fclean all

.PHONY: all clean fclean re

-include $(DEP)