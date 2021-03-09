NAME		= npuzzle

CC			= g++-9
FLAGS		= -g -Wall -Wextra -Werror -O3
LIB			= lib/lib.a

INCLUDES	= ./includes/npuzzle.hpp

SRC			= main.cpp

OBJ			= State/State.cpp \
			  Puzzle/Puzzle.cpp \
			  Puzzle/Puzzle_parseOptions.cpp \
			  Puzzle/Puzzle_init.cpp \
			  Puzzle/Puzzle_generate_start_array.cpp \
			  Puzzle/Puzzle_isSolvable.cpp \
			  Puzzle/Puzzle_solve.cpp \
			  Visualizer/Visualizer.cpp
OBJ_DIR		= objects/
OBJ_SUBDIR	= objects/State \
			  objects/Puzzle \
			  objects/Visualizer

SRC_PATH	= ./srcs/
SRCS		= $(addprefix $(SRC_PATH), $(SRC))
SRCS		+= $(addprefix $(SRC_PATH), $(OBJ_DIR), $(OBJ))

BIN			= $(SRC:%.cpp=%.o)
BIN			+= $(addprefix $(OBJ_DIR), $(OBJ:%.cpp=%.o))
BIN_PATH	= ./bins/
BINS		= $(addprefix $(BIN_PATH), $(BIN))
BIN_SUBDIR  = $(addprefix $(BIN_PATH), $(OBJ_DIR))
BIN_SUBDIR  += $(addprefix $(BIN_PATH), $(OBJ_SUBDIR))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(LIB) $(BIN_PATH) $(BIN_SUBDIR) $(BINS)
	$(CC) $(FLAGS) $(BINS) $(LIB) -lncurses -o $@

$(BIN_PATH):
	@ mkdir $@

$(BIN_SUBDIR):
	 mkdir $@

$(LIB):
	@ make -C lib

$(BIN_PATH)%.o: $(SRC_PATH)%.cpp
	$(CC) -I includes -o $@ -c $< $(FLAGS)

clean:
	@ make -C lib clean
	@ rm -rf $(BIN_PATH)

fclean: clean
	@ rm -rf $(LIB)
	@ rm -f $(NAME)

re: fclean all
