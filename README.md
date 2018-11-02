brainfuck-interpreter
----------------------

This is simple brainfuck interpreter with memory size of 30 000 cells. Memory size is constant and
does not increase dynamically. Neither the negative memory 
cells are supported (memory does not extend to left).

Memory access is protected (bounds checking is implemented) and interpreter ends with exit code 3 if
illegal memory access is detected.

If the program has unmatched brackets, the exit code 4 is returned.

## Building

You can build the project using CMake to generate build files and then using your favorite build tool.

```
$ cmake .
$ make
```

## Running

To run a brainfuck program, you must pass the path to program as first argument to the 
interpreter application. Sending the program via standard input is not possible becuase
it would collide with interactively entered data by user while the program is running.

```
$ brainfuck-interpreter hello_world.bf
Hello World!
```

## Testings

To run tests, pass the `--tests` as the first argument.

```
$ ./brainfuck_interpreter --tests
[==========] Running 7 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 7 tests from Test
[ RUN      ] Test.testZeroFilledMemory
[       OK ] Test.testZeroFilledMemory (0 ms)
...
[ RUN      ] Test.testHelloWorld2
[       OK ] Test.testHelloWorld2 (1 ms)
[----------] 7 tests from Test (1 ms total)

[----------] Global test environment tear-down
[==========] 7 tests from 1 test case ran. (1 ms total)
[  PASSED  ] 7 tests.
```

### Sample programs

In the sample-programs directory are some programs that can be run with this interpreter.

```
cat.bf - Copies stdin to stdout (does not terminate)
factorial.bf - Computes factorials indefinitely (does not terminate)
hw.bf - Prints Hello, World! and exits.
```