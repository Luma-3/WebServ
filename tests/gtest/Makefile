CXX = g++

# std 14 is required for gtest. And simplify the test code
CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -g -I$(GTEST_INCLUDE) -I$(CODE_INCLUDE) -MMD -MP

GTEST_INCLUDE = googletest/googletest/include
CODE_INCLUDE = ../../includes

# Link with the gtest_main library
LDFLAGS = -lgtest_main -lgtest -lpthread

BUILD_DIR = ../../build
BIN_DIR = $(BUILD_DIR)/tests

OBJ_DIR = $(BUILD_DIR)/obj

SRC_DIR = ../../sources
SRC = $($(filter-out $(SRC_CODE_DIR/main.cpp), $(shell find $(SRC_DIR) -name '*.cpp')))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/obj/%.o, $(SRC))

TEST_DIR = .
SRC_TEST_EXCLUDE = $(TEST_DIR)/googletest/
TEST := $(filter-out $(SRC_TEST_EXCLUDE)%, $(shell find $(TEST_DIR) -name '*.cpp'))
OBJS_TEST = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/obj/%.o, $(TEST))

DEPENDS := $(OBJS:.o=.d) $(OBJS_TEST:.o=.d)

OUTS := $(patsubst $(TEST_DIR)/%.cpp, $(BIN_DIR)/%.out, $(notdir $(TEST)))

all: $(OUTS)

$(OUTS): %.out : $(OBJS) $(OBJS_TEST)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPENDS)




# SRC_CODE_DIR = ../../sources
# SRC_CODE := $(shell find $(SRC_CODE_DIR) -name '*.cpp')
# SRC_CODE := $(filter-out $(SRC_CODE_DIR)/main.cpp, $(SRC_CODE))

# SRC_TEST_DIR = .
# SRC_TEST_EXCLUDE = $(SRC_TEST_DIR)/googletest/
# SRC_TEST := $(shell find $(SRC_TEST_DIR) -name '*.cpp')
# SRC_TEST := $(filter-out $(SRC_TEST_EXCLUDE)%, $(SRC_TEST))

# OBJ_DIR = $(BUILD_DIR)/obj
# OBJ := $(patsubst $(SRC_CODE_DIR)%.cpp, $(OBJ_DIR)/%.o, $(SRC_CODE))

# OUTS := $(patsubst %.cpp, $(TEST_DIR)/%.out, $(notdir $(SRC_TEST)))

# all_test: $(OUTS)

# $(OUTS): %.out: $(SRC_TEST) $(OBJ)
# 	@mkdir -p $(TEST_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) $< -o $@

# $(OBJ_DIR)/%.o: $(SRC_CODE_DIR)%.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean_test:
# 	rm -rf $(OBJ_DIR) $(OUTS)

# .PHONY: all clean