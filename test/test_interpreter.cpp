#include <iostream>
#include "gtest/gtest.h"
#include "../src/Interpreter.h"

using namespace std;

using namespace ::testing;

TEST_F(Test, testZeroFilledMemory) {
    auto interpreter = Interpreter();
    ASSERT_EQ(0, interpreter.memoryAt(0));
    ASSERT_EQ(0, interpreter.memoryAt(16));
    ASSERT_EQ(0, interpreter.memoryAt(5000));
}

TEST_F(Test, testIncrementMemory) {
    auto interpreter = Interpreter();
    interpreter.loadProgram("+++");
    interpreter.interpret();
    ASSERT_EQ(3, interpreter.memoryAt(0));
}

TEST_F(Test, testDecrementMemory) {
    auto interpreter = Interpreter();
    interpreter.loadProgram(">+++-");
    interpreter.interpret();
    ASSERT_EQ(2, interpreter.memoryAt(1));
}
