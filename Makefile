-include $(DEPS)

#----------------------------------#
#			VARIABLES              #
#----------------------------------#

CXX = g++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -I$(INCLUDES) -pedantic #-O3

INCLUDES = includes

BUILD_DIR := build
NAME := webserv

SRC_DIR := sources
SRC := $(shell find $(SRC_DIR) -name "*.cpp")

OBJ_DIR := $(BUILD_DIR)/obj
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEPS := $(BUILD_DIR)/depend.mk

COMPILE_COMMANDS := $(BUILD_DIR)/compile_commands.json

VERSION := $(BUILD_DIR)/version.txt


#----------------------------------#
#			  COLORS               #
#----------------------------------#

COLOR_RESET		:=	\033[0m
COLOR_GREEN		:=	\033[38;5;76m
COLOR_RED		:=	\033[38;5;160m
COLOR_BLUE		:=	\033[38;5;45m
COLOR_ORANGE	:=	\033[38;5;220m
BOLD			:=	\033[1m
UNDERLINE		:=	\033[4m

#----------------------------------#
#			PROGRESS BAR           #
#----------------------------------#

TOTAL_SRC := $(words $(SRC))
CURRENT_SRC := 1


#----------------------------------#
#			MAKE RULES             #
#----------------------------------#


all: $(NAME)

$(COMPILE_COMMANDS): $(SRC)
	@echo "Generating compile_commands.json"
	rm -f $(COMPILE_COMMANDS)
	@echo "[" >> $(COMPILE_COMMANDS)
	@for file in $(SRC); do \
		echo "  { \"directory\": \"$(shell pwd)\"," >> $(COMPILE_COMMANDS); \
		echo "    \"command\": \"$(CXX) $(CXXFLAGS) -c $$file -o $(OBJ_DIR)/$$(basename $$file .cpp).o\"," >> $(COMPILE_COMMANDS); \
		echo "    \"file\": \"$$file\"" >> $(COMPILE_COMMANDS); \
		echo "  }," >> $(COMPILE_COMMANDS); \
	done
	@echo "]" >> $(COMPILE_COMMANDS)


$(VERSION): $(SRC)
	@echo "Build version: $(shell date)" > $(VERSION)
	@echo "Commit hash: $(shell git rev-parse --short HEAD)" >> $(VERSION)

check_depend: $(COMPILE_COMMANDS)
	@echo "Checking compile_commands.json"
	@command -v clang-check > /dev/null || (echo "clang-check not found, please install clang-tools" && exit 1)

stats: $(COMPILE_COMMANDS)
	@echo "Numbre of files: $(shell echo $(SRC) | wc -w)"
	@echo "Numbre of lines: $(shell cat $(SRC) | wc -l)"

#----------------------------------#
#			COMPILE RULES          #
#----------------------------------#

$(DEPS) : $(SRC)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MM $(SRC) > $(DEPS)


$(NAME): $(DEPS) $(OBJ) $(VERSION)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(COLOR_GREEN)$(BOLD)$(NAME)$(COLOR_RESET)$(COLOR_GREEN) compiled successfully$(COLOR_RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@./progress_bar.sh $(TOTAL_SRC) $(CURRENT_SRC) $< "$(CXX) $(CXXFLAGS) -c $< -o $@"
	@$(eval CURRENT_SRC := $(shell expr $(CURRENT_SRC) + 1))


#----------------------------------#
#			CLEAN RULES            #
#----------------------------------#

clean:
	rm -rf $(OBJ_DIR) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all


#----------------------------------#
#			DEBUG RULES            #
#----------------------------------#


debug: CXXFLAGS += -g3
debug: re

tidy: $(COMPILE_COMMANDS)
	clang-tidy -p=$(BUILD_DIR) $(SRC) -- $(CXXFLAGS)

.PHONY: all clean fclean re