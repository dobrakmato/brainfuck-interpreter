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
    interpreter.setPrintToStdout(false);
    interpreter.loadProgram("+++");
    interpreter.interpret();
    ASSERT_EQ(3, interpreter.memoryAt(0));
}

TEST_F(Test, testDecrementMemory) {
    auto interpreter = Interpreter();
    interpreter.setPrintToStdout(false);
    interpreter.loadProgram(">+++-");
    interpreter.interpret();
    ASSERT_EQ(2, interpreter.memoryAt(1));
}

TEST_F(Test, testOutputBuffer) {
    auto interpreter = Interpreter();
    interpreter.setPrintToStdout(false);
    interpreter.loadProgram("+++++++++++++++++++++++++++++++++++.");
    ASSERT_EQ("#", interpreter.interpret());
}

TEST_F(Test, testMultiplication) {
    auto interpreter = Interpreter();
    interpreter.setPrintToStdout(false);
    interpreter.loadProgram("++++++++[->+++++++<]>.");
    interpreter.interpret();
    ASSERT_EQ(0, interpreter.memoryAt(0));
    ASSERT_EQ(56, interpreter.memoryAt(1));
}

TEST_F(Test, testHelloWorld) {
    auto interpreter = Interpreter();
    interpreter.setPrintToStdout(false);
    interpreter.loadProgram("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.");
    ASSERT_EQ("Hello World!\n", interpreter.interpret());
}

// This is a slightly more complex variant that often triggers interpreter bugs.
TEST_F(Test, testHelloWorld2) {
    auto interpreter = Interpreter();
    interpreter.setPrintToStdout(false);
    interpreter.loadProgram(">++++++++[-<+++++++++>]<.>>+>-[+]++>++>+++[>[->+++<<+++>]<<]>-----.>->\n"
                            "+++..+++.>-.<<+[>[+>+]>>]<--------------.>>.+++.------.--------.>+.>+.");
    ASSERT_EQ("Hello World!\n", interpreter.interpret());
}
