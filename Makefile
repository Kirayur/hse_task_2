CXX = g++
CXXFLAGS = -std=c++17 -I. -O3

MAIN_SRC = MainExpression.cpp
TEST_SRC = MyExpressionTest.cpp
SRCS = $(MAIN_SRC) $(TEST_SRC)

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)
OBJS = $(MAIN_OBJ) $(TEST_OBJ)

MAIN_TARGET = differentiator
TEST_TARGET = test_program

all: $(MAIN_TARGET) $(TEST_TARGET)

$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN_TARGET) $(MAIN_OBJ)

$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJ)

%.o: %.cpp expression.hpp expression1.tpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(MAIN_TARGET) $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean test
