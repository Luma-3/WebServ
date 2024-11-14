CXX = g++

# std 14 is required for gtest. And simplify the test code
CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -g -I$(GTEST_INCLUDE) -I$(CODE_INCLUDE)

GTEST_INCLUDE = googletest/googletest/include
CODE_INCLUDE = ../../includes

LDFLAGS_DIR = googletest/build/lib
# Link with the gtest_main library
LDFLAGS = -L$(LDFLAGS_DIR)  -lgtest_main  -lgtest -lpthread

BUILD_DIR = ../../build
BIN_DIR = $(BUILD_DIR)/tests
OBJ_DIR = $(BUILD_DIR)/obj

SRC_DIR = ../../sources
SRC := $(filter-out $(SRC_DIR)/main.cpp, $(shell find $(SRC_DIR) -name '*.cpp'))

TEST_DIR = .
TEST_EXCLUDE = $(TEST_DIR)/googletest/
TEST := $(filter-out $(TEST_EXCLUDE)%, $(shell find $(TEST_DIR) -name '*.cpp'))
TEST_OBJ := $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST))

OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

DEPENDS := $(OBJ:.o=.d)

OUTS := $(patsubst $(TEST_DIR)/%.cpp, $(BIN_DIR)/%.out,  $(TEST))

all: $(OUTS)

$(OUTS) : $(BIN_DIR)/%.out: %.cpp | $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< $(OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# $(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@


-include $(DEPENDS)

echo :
	@echo $(TEST)
	@echo $(OUTS)

clean:
	rm -rf $(OBJ_DIR) $(DEPENDS)

fclean: clean
	rm -fr $(BIN_DIR)

re: fclean all

gtestClean:
	rm -rf googletest

.PHONY: all clean
