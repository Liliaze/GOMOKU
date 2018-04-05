NAME= Gomoku

CC= g++
CFLAGS= -pthread -std=c++11 -Wall -Wextra -Ofast -g3  #-Werror -fsanitize=address
SFMLFLAG= -L ~/.brew/opt/sfml/lib -L ./lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
SRC_NAME= main.cpp \
          utils.cpp \
          gomoku.cpp \
          rules.cpp \
          defaultRules.cpp \
          player.cpp \
          randomPlayer.cpp\
	  humanPlayer.cpp\
	  noobIA.cpp\
	  interface.cpp\
	  boardIterator.cpp\
	  minMaxDynamicPlayer.cpp\
	  heuristicBoard.cpp\
	  heuristicBoard_h.cpp\
	  heuristicBoard_v.cpp\
	  heuristicBoard_dl.cpp\
	  heuristicBoard_dr.cpp\
	  menu.cpp
SRC_PATH= ./sources/
INC_PATH= ./includes/
OBJ_NAME= $(SRC_NAME:.cpp=.o)
OBJ_PATH= ./obj/
SRC= $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ= $(addprefix $(OBJ_PATH), $(OBJ_NAME))

.PHONY: all
all: $(NAME) end export
	
end:
	@printf "$(NAME) successfully created\n"

export:
	@export LD_LIBRARY_PATH=~/.brew/opt/sfml/lib

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) -I$(INC_PATH) $(OBJ) $(SFMLFLAG)
	@printf "\033[2K[ \033[31mcompiling\033[0m ] $< \r"

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@$(CC) -I$(INC_PATH) $(CFLAGS) -o $@ -c $<
	@printf " \033[2K[ \033[31mcompiling\033[0m ] $< \r"

.PHONY: clean
clean:
	@rm -f $(OBJ)
	@printf "[ \033[36mdelete\033[0m ] objects from $(NAME)\n"
	@rm -rf $(OBJ_PATH)

.PHONY: fclean
fclean: clean
	@printf "[ \033[36mdelete\033[0m ] $(NAME)\n"
	@rm -f $(NAME)

.PHONY: re
re: fclean all
