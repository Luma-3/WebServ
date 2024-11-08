CXX = g++

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -I$(INCLUDES) -pedantic  #-O3

INCLUDES = includes

SRC_DIR = sources
SRC = $(shell find $(SRC_DIR) -name "*.cpp")

OBJ_DIR = build/obj
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEPS_DIR = build/deps
DEPS = $(SRC:$(SRC_DIR)/%.cpp=$(DEPS_DIR)/%.d) 
-include $(DEPS)

NAME = webserv

all: $(NAME)
	

$(NAME): $(OBJ)
	mkdir -p ./build
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf ./build

re: fclean all

