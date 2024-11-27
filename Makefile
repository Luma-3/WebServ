
#----------------------------------#
#			VARIABLES              #
#----------------------------------#

CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -I$(INCLUDES) -pedantic #-O3

INCLUDES = $(PWD)/includes

BUILD_DIR := build
NAME := webserv

SRC_DIR := sources
SRC_LIST =	main.cpp 					\
		Logger.cpp					\
		finder.cpp					\
		server/Handler.cpp			\
		server/ServerException.cpp	\
		server/ServerHost.cpp		\
		server/Signal.cpp			\
		server/VirtualServer.cpp	\
		parser/Action.cpp			\
		parser/Location.cpp			\
		parser/Param.cpp			\
		parser/Parser.cpp			\
		parser/ParserException.cpp	\
		parser/ReduceRule.cpp		\
		lexer/Lexer.cpp				\
		lexer/LexerException.cpp	\
		lexer/regex.cpp				\
		lexer/Token.cpp				\
		client/autoindex.cpp		\
		client/Builder.cpp			\
		client/Client_Tables.cpp		\
		client/Client.cpp			\
		client/ClientCGI.cpp		\
		client/Delete_request.cpp	\
		client/Parser_uri.cpp		\
		client/Parser.cpp			\
		cgi/CGIHandler.cpp			\
		cgi/executeCGI.cpp			\
		cgi/init_cgi.cpp
SRC = $(addprefix $(SRC_DIR)/, $(SRC_LIST))
		


# $(shell find $(SRC_DIR) -name "*.cpp")

OBJ_DIR := $(BUILD_DIR)/obj
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


DEPS := $(OBJ:.o=.d)

COMPILE_COMMANDS := $(BUILD_DIR)/compile_commands.json

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
	@mkdir -p $(BUILD_DIR)
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


check_depend: $(COMPILE_COMMANDS)
	@echo "Checking compile_commands.json"
	@command -v clang-check > /dev/null || (echo "clang-check not found, please install clang-tools" && exit 1)
.PHONY: check_depend

stats: $(COMPILE_COMMANDS)
	@echo "Numbre of cpp files: $(shell echo $(SRC) | wc -w)"
	@echo "Numbre of cpp lines: $(shell cat $(SRC) | wc -l)"
	@echo "Numbre of hpp files: $(shell echo $(shell find $(INCLUDES) -name "*.hpp") | wc -w)"
	@echo "Numbre of hpp lines: $(shell cat $(shell find $(INCLUDES) -name "*.hpp") | wc -l)"
	@echo "Total files: $(shell echo $(SRC) $(shell find $(INCLUDES) -name "*.hpp") | wc -w)"
	@echo "Total lines: $(shell cat $(SRC) $(shell find $(INCLUDES) -name "*.hpp") | wc -l)"
.PHONY: stats

#----------------------------------#
#			COMPILE RULES          #
#----------------------------------#


$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "$(COLOR_GREEN)$(BOLD)$(NAME)$(COLOR_RESET)$(COLOR_GREEN) compiled successfully$(COLOR_RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MMD -c $< -o $@
	@./progress_bar.sh $(TOTAL_SRC) $(CURRENT_SRC) $< "$(CXX) $(CXXFLAGS) -MMD -c $< -o $@"
	@$(eval CURRENT_SRC := $(shell expr $(CURRENT_SRC) + 1))

-include $(DEPS)

#----------------------------------#
#			CLEAN RULES            #
#----------------------------------#

clean:
	rm -rf $(OBJ_DIR) $(DEPS)
.PHONY: clean

fclean: clean
	rm -f $(NAME)
.PHONY: fclean

perf: CXXFLAGS += -pg
perf: re

re: fclean all
.PHONY: re


#----------------------------------#
#			DEBUG RULES            #
#----------------------------------#


debug: CXXFLAGS += -g3
debug: re
.PHONY: debug

tidy: $(COMPILE_COMMANDS)
	clang-tidy -p=$(BUILD_DIR) $(SRC) -- $(CXXFLAGS)
.PHONY: tidy

##----------------------------------#
#			UNITS TEST              #
#-----------------------------------#

test: $(NAME)
	./tests/gtest_installer.sh
	@make -C tests/gtest

clean_test:
	@make -sC tests/gtest clean

fclean_test:
	@make -sC tests/gtest fclean

re_test:
	@make -C tests/gtest re

gtest_clean:
	@make -sC tests/gtest gtestClean


.PHONY: test clean_test fclean_test gtest_clean


#----------------------------------#
#			 CLEAN ALL			   #
#----------------------------------#

fullclean: fclean fclean_test gtest_clean
.PHONY: fullclean