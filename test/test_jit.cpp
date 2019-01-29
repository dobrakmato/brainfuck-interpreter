#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "../src/Jit.h"

using namespace std;

using namespace ::testing;

TEST_F(Test, testZeroFilledMemoryJit) {
    auto jit = Jit();
    ASSERT_EQ(0, jit.memoryAt(0));
    ASSERT_EQ(0, jit.memoryAt(16));
    ASSERT_EQ(0, jit.memoryAt(5000));
}

TEST_F(Test, testIncrementMemoryJit) {
    auto jit = Jit();
    jit.loadProgram("+++");
    jit.compile();
    jit.run();
    ASSERT_EQ(3, jit.memoryAt(0));
}

TEST_F(Test, testDecrementMemoryJit) {
    auto jit = Jit();
    jit.loadProgram(">+++-");
    jit.compile();
    jit.run();
    ASSERT_EQ(2, jit.memoryAt(1));
}

TEST_F(Test, testMultiplicationJit) {
    auto jit = Jit();
    jit.loadProgram("++++++++[->+++++++<]>.");
    jit.compile();
    jit.run();
    ASSERT_EQ(0, jit.memoryAt(0));
    ASSERT_EQ(56, jit.memoryAt(1));
}

// todo: implement input/output to buffers to add these tests
/*

TEST_F(Test, testOutputBuffer) {
    auto jit = Jit();
    jit.setPrintToStdout(false);
    jit.loadProgram("+++++++++++++++++++++++++++++++++++.");
    ASSERT_EQ("#", jit.interpret());
}

TEST_F(Test, testHelloWorld) {
    auto jit = Jit();
    jit.setPrintToStdout(false);
    jit.loadProgram(
            "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.");
    ASSERT_EQ("Hello World!\n", jit.interpret());
}

// This is a slightly more complex variant that often triggers jit bugs.
TEST_F(Test, testHelloWorld2) {
    auto jit = Jit();
    jit.setPrintToStdout(false);
    jit.loadProgram(">++++++++[-<+++++++++>]<.>>+>-[+]++>++>+++[>[->+++<<+++>]<<]>-----.>->\n"
                            "+++..+++.>-.<<+[>[+>+]>>]<--------------.>>.+++.------.--------.>+.>+.");
    ASSERT_EQ("Hello World!\n", jit.interpret());
}

TEST_F(Test, testPrimes) {
    ifstream t("./sample-programs/primes.bf");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto jit = Jit();
    jit.setPrintToStdout(false);
    jit.loadProgram(str);
    jit.setInput("10\n");
    ASSERT_EQ("Primes up to: 2 3 5 7 \n", jit.interpret());
}

TEST_F(Test, testPrimes2) {
    ifstream t("./sample-programs/primes.bf");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto jit = Jit();
    jit.setPrintToStdout(false);
    jit.loadProgram(str);
    jit.setInput("30\n");
    ASSERT_EQ("Primes up to: 2 3 5 7 11 13 17 19 23 29 \n", jit.interpret());
}

TEST_F(Test, testSierpinski) {
    ifstream t("./sample-programs/sierpinski.bf");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto jit = Jit();
    jit.setPrintToStdout(false);
    jit.loadProgram(str);
    auto output = jit.interpret();
    ASSERT_EQ(
            "                                *    \n\r                               * *    \n\r                              *   *    \n\r                             * * * *    \n\r                            *       *    \n\r                           * *     * *    \n\r                          *   *   *   *    \n\r                         * * * * * * * *    \n\r                        *               *    \n\r                       * *             * *    \n\r                      *   *           *   *    \n\r                     * * * *         * * * *    \n\r                    *       *       *       *    \n\r                   * *     * *     * *     * *    \n\r                  *   *   *   *   *   *   *   *    \n\r                 * * * * * * * * * * * * * * * *    \n\r                *                               *    \n\r               * *                             * *    \n\r              *   *                           *   *    \n\r             * * * *                         * * * *    \n\r            *       *                       *       *    \n\r           * *     * *                     * *     * *    \n\r          *   *   *   *                   *   *   *   *    \n\r         * * * * * * * *                 * * * * * * * *    \n\r        *               *               *               *    \n\r       * *             * *             * *             * *    \n\r      *   *           *   *           *   *           *   *    \n\r     * * * *         * * * *         * * * *         * * * *    \n\r    *       *       *       *       *       *       *       *    \n\r   * *     * *     * *     * *     * *     * *     * *     * *    \n\r  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *    \n\r * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *    \n\r",
            output);
}
*/