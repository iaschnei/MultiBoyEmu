# ---------------------------------------------------------------------------- #

SRCS		=	main.cpp EmuSelect/EmuSelector.cpp

# ---------------------------------------------------------------------------- #

OBJS		=	$(SRCS:.cpp=.o)

DEPS		=	$(SRCS:.cpp=.d)

C++			=	c++

INCL    	=	includes

FLAGS		=	-Wall -Wextra -Werror -MMD -MP  $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf

NAME		=	MultiBoy

COLOR_GREEN	=	\033[1;32m
COLOR_RED	=	\033[1;31m
COLOR_BLUE	=	\033[3;36m
COLOR_PURP	=	\033[1;35m
COLOR_END	=	\033[0m

%.o: %.cpp
	@$(C++) $(FLAGS) -I $(INCL) -g -c $< -o $@
	@echo "$(COLOR_BLUE) || Compiling ($<)...$(COLOR_END)"

all: $(NAME)

$(NAME): $(OBJS)
	@$(C++) $(OBJS) $(FLAGS) -o $(NAME)
	@echo "$(COLOR_GREEN) || Done !$(COLOR_END)"

clean:
	@rm -rf $(OBJS)
	@echo "$(COLOR_RED) || Cleaning files...$(COLOR_END)"

fclean: clean
	@rm -rf $(NAME) $(DEPS)
	@echo "$(COLOR_RED) || Cleaning library...$(COLOR_END)"

re: fclean all

.PHONY: re fclean all clean

-include $(DEPS)
