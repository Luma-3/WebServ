CXX = g++

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -I$(INCLUDES) # -pedantic -O3

INCLUDES = includes

SRC_DIR = sources
SRC = $(shell find $(SRC_DIR) -name "*.cpp")

OBJ_DIR = build/obj
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

NAME = webserv

all: $(NAME)
	mkdir -p ./build

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f ./build

re: fclean all

