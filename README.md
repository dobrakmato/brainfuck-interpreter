brainfuck-jit
----------------------
[![Build Status](https://travis-ci.org/dobrakmato/brainfuck-interpreter.svg?branch=master)](https://travis-ci.org/dobrakmato/brainfuck-interpreter)
[![Build Status](https://ci.appveyor.com/api/projects/status/wn8wu8e4lk9tnjsa)](https://ci.appveyor.com/project/dobrakmato/brainfuck-interpreter)
[![Coverage Status](https://coveralls.io/repos/github/dobrakmato/brainfuck-interpreter/badge.svg?branch=master)](https://coveralls.io/github/dobrakmato/brainfuck-interpreter?branch=master)
[![Codacy](https://api.codacy.com/project/badge/Grade/eac772ecb4b141b5a0a6a345f8d9f8c0)](https://app.codacy.com/project/dobrakmato/brainfuck-interpreter/dashboard)

This is simple brainfuck VM with JIT compiler with interpreter as a backup. 

Memory size is 30 000 cells. It is constant and does not increase dynamically. Neither the negative memory 
cells are supported (memory does not extend to left).

If the architecture of system that is running the VM is supported as JIT compilation target the Brainfuck program is 
first compiled and then run, otherwise it is run only with interpreter.

Memory access is bounds-checked and interpreter ends with exit code 3 if
illegal memory access is detected.

If the program has unmatched brackets, the exit code 4 is returned.

Interpreter performs some optimizations. To see what are they check the **Optimizations** part.

JIT compiled code does **no memory bound checking** so wrong program can crash the VM. This simplifies generated
machine code and speeds up execution. If brackets are unbalanced, code cannot be compiled - so the compilation
fails.

Currently the JIT compilation is possible for following targets:
  - Windows x86-64
  - Linux x86-64

You can force interpreter mode by using `--interpret` flag.

## Building

You can build the project using CMake to generate build files and then using your favorite build tool.

```
$ cmake .
$ make
```

## Running

To run a brainfuck program, you must pass the path to program as first argument to the 
compiler application. Sending the program via standard input is not possible becuase
it would collide with interactively entered data by user while the program is running.

```
$ brainfuck-vm hello_world.bf
Hello World!
```

## Testing

To run included unit tests, use `ctest` command or run tests manually `./bin/unit_tests`.

```
$ ctest
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

## Optimizations

### Interpreter - Caching jumps to jump table

Brainfuck program use jumps very often, so caching them seems to be good direction in
optimizing the interpreter.

By using a cache for remembering the addresses of jumps we can easily improve the interpreter
performance to about two times faster (depending on the running application).

The jump cache is accomplished by using an hash map. Usage of this jump table can be enabled by 
defining c macro `#define JUMP_TABLE`. The code for using jump table is then added
to interpreter source code.

After the script execution finishes some statistics are printed to console. These number can
help us to get better understanding what is happening in our program. We can measure
the performance without jump table and with.

```
$ time echo "80" | ./brainfuck_interpreter ../sample-programs/primes.bf 
Primes up to: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 
STAT_JT_MISS=11991978
STAT_JT_HIT=0

real    0m2.086s
user    0m2.071s
sys     0m0.012s
```
```
$ time echo "80" | ./brainfuck_interpreter ../sample-programs/primes.bf 
Primes up to: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 
STAT_JT_MISS=119
STAT_JT_HIT=11991859

real    0m1.101s
user    0m1.092s
sys     0m0.007s
```

In this example we see almost 2x performance improvement. Printing of statistics
can be disabled by passing `--no-stats` as second parameter of the application.

### Compiler optimizations

To improve the performance of generated assembly code compiler does several optimizations.

#### Consecutive INC/DEC operations optimization
Compiler replaces consecutive increment / decrement instructions with one addition / subtraction.

```asm
add ebx, 1
add ebx, 1
add ebx, 1
```

is replaced by 

```asm
add ebx, 3
```

#### Consecutive subroutine calls

#### Clear loops
Replace `[-]` with `mov [ebx], 0`.

Replace `[+]` with `mov [ebx], 0`.

http://calmerthanyouare.org/2015/01/07/optimizing-brainfuck.html

## Performance 

Below you can find table that compares performance of different execution profiles (interpreted code, JIT compiled, JIT compiled & optimized)

|File|Interpreted|Interpreted + optimizations|JIT|JIT + optimizations|
|----|-----------|---------------------------|---|-------------------|
|mandelbrot.bf|0s|0s|0s|0s|
|primes.bf (80)|0s|0s|0s|0s|
|primes.bf (140)|0s|0s|0s|0s|
|sierpinski.bf|0s|0s|0s|0s|

## Sample programs

In the sample-programs directory are some programs that can be run with this interpreter.

```
cat.bf - Copies stdin to stdout (does not terminate)
factorial.bf - Computes factorials indefinitely (does not terminate)
hw.bf - Prints Hello, World! and exits.
mandelbrot.bf - Draws fractal from mandelbrot set.
primes.bf - Generates primes up to specified number.
sierpinski.bf - Draws sierpinski triangle.
```

For example running `mandelbrot.bf` produces the following output:

```text
$ ./brainfuck_interpreter ../sample-programs/mandelbrot.bf 
AAAAAAAAAAAAAAAABBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDEGFFEEEEDDDDDDCCCCCCCCCBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAABBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDEEEFGIIGFFEEEDDDDDDDDCCCCCCCCCBBBBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAABBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDEEEEFFFI KHGGGHGEDDDDDDDDDCCCCCCCCCBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAABBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDEEEEEFFGHIMTKLZOGFEEDDDDDDDDDCCCCCCCCCBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAABBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDEEEEEEFGGHHIKPPKIHGFFEEEDDDDDDDDDCCCCCCCCCCBBBBBBBBBBBBBBBBBB
AAAAAAAAAABBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDEEEEEEFFGHIJKS  X KHHGFEEEEEDDDDDDDDDCCCCCCCCCCBBBBBBBBBBBBBBBB
AAAAAAAAABBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDEEEEEEFFGQPUVOTY   ZQL[MHFEEEEEEEDDDDDDDCCCCCCCCCCCBBBBBBBBBBBBBB
AAAAAAAABBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDEEEEEFFFFFGGHJLZ         UKHGFFEEEEEEEEDDDDDCCCCCCCCCCCCBBBBBBBBBBBB
AAAAAAABBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDEEEEFFFFFFGGGGHIKP           KHHGGFFFFEEEEEEDDDDDCCCCCCCCCCCBBBBBBBBBBB
AAAAAAABBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDEEEEEFGGHIIHHHHHIIIJKMR        VMKJIHHHGFFFFFFGSGEDDDDCCCCCCCCCCCCBBBBBBBBB
AAAAAABBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDEEEEEEFFGHK   MKJIJO  N R  X      YUSR PLV LHHHGGHIOJGFEDDDCCCCCCCCCCCCBBBBBBBB
AAAAABBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDEEEEEEEEEFFFFGH O    TN S                       NKJKR LLQMNHEEDDDCCCCCCCCCCCCBBBBBBB
AAAAABBCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDEEEEEEEEEEEEFFFFFGHHIN                                 Q     UMWGEEEDDDCCCCCCCCCCCCBBBBBB
AAAABBCCCCCCCCCCCCCCCCCCCCCCCCCDDDDEEEEEEEEEEEEEEEFFFFFFGHIJKLOT                                     [JGFFEEEDDCCCCCCCCCCCCCBBBBB
AAAABCCCCCCCCCCCCCCCCCCCCCCDDDDEEEEEEEEEEEEEEEEFFFFFFGGHYV RQU                                     QMJHGGFEEEDDDCCCCCCCCCCCCCBBBB
AAABCCCCCCCCCCCCCCCCCDDDDDDDEEFJIHFFFFFFFFFFFFFFGGGGGGHIJN                                            JHHGFEEDDDDCCCCCCCCCCCCCBBB
AAABCCCCCCCCCCCDDDDDDDDDDEEEEFFHLKHHGGGGHHMJHGGGGGGHHHIKRR                                           UQ L HFEDDDDCCCCCCCCCCCCCCBB
AABCCCCCCCCDDDDDDDDDDDEEEEEEFFFHKQMRKNJIJLVS JJKIIIIIIJLR                                               YNHFEDDDDDCCCCCCCCCCCCCBB
AABCCCCCDDDDDDDDDDDDEEEEEEEFFGGHIJKOU  O O   PR LLJJJKL                                                OIHFFEDDDDDCCCCCCCCCCCCCCB
AACCCDDDDDDDDDDDDDEEEEEEEEEFGGGHIJMR              RMLMN                                                 NTFEEDDDDDDCCCCCCCCCCCCCB
AACCDDDDDDDDDDDDEEEEEEEEEFGGGHHKONSZ                QPR                                                NJGFEEDDDDDDCCCCCCCCCCCCCC
ABCDDDDDDDDDDDEEEEEFFFFFGIPJIIJKMQ                   VX                                                 HFFEEDDDDDDCCCCCCCCCCCCCC
ACDDDDDDDDDDEFFFFFFFGGGGHIKZOOPPS                                                                      HGFEEEDDDDDDCCCCCCCCCCCCCC
ADEEEEFFFGHIGGGGGGHHHHIJJLNY                                                                        TJHGFFEEEDDDDDDDCCCCCCCCCCCCC
A                                                                                                 PLJHGGFFEEEDDDDDDDCCCCCCCCCCCCC
ADEEEEFFFGHIGGGGGGHHHHIJJLNY                                                                        TJHGFFEEEDDDDDDDCCCCCCCCCCCCC
ACDDDDDDDDDDEFFFFFFFGGGGHIKZOOPPS                                                                      HGFEEEDDDDDDCCCCCCCCCCCCCC
ABCDDDDDDDDDDDEEEEEFFFFFGIPJIIJKMQ                   VX                                                 HFFEEDDDDDDCCCCCCCCCCCCCC
AACCDDDDDDDDDDDDEEEEEEEEEFGGGHHKONSZ                QPR                                                NJGFEEDDDDDDCCCCCCCCCCCCCC
AACCCDDDDDDDDDDDDDEEEEEEEEEFGGGHIJMR              RMLMN                                                 NTFEEDDDDDDCCCCCCCCCCCCCB
AABCCCCCDDDDDDDDDDDDEEEEEEEFFGGHIJKOU  O O   PR LLJJJKL                                                OIHFFEDDDDDCCCCCCCCCCCCCCB
AABCCCCCCCCDDDDDDDDDDDEEEEEEFFFHKQMRKNJIJLVS JJKIIIIIIJLR                                               YNHFEDDDDDCCCCCCCCCCCCCBB
AAABCCCCCCCCCCCDDDDDDDDDDEEEEFFHLKHHGGGGHHMJHGGGGGGHHHIKRR                                           UQ L HFEDDDDCCCCCCCCCCCCCCBB
AAABCCCCCCCCCCCCCCCCCDDDDDDDEEFJIHFFFFFFFFFFFFFFGGGGGGHIJN                                            JHHGFEEDDDDCCCCCCCCCCCCCBBB
AAAABCCCCCCCCCCCCCCCCCCCCCCDDDDEEEEEEEEEEEEEEEEFFFFFFGGHYV RQU                                     QMJHGGFEEEDDDCCCCCCCCCCCCCBBBB
AAAABBCCCCCCCCCCCCCCCCCCCCCCCCCDDDDEEEEEEEEEEEEEEEFFFFFFGHIJKLOT                                     [JGFFEEEDDCCCCCCCCCCCCCBBBBB
AAAAABBCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDEEEEEEEEEEEEFFFFFGHHIN                                 Q     UMWGEEEDDDCCCCCCCCCCCCBBBBBB
AAAAABBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDEEEEEEEEEFFFFGH O    TN S                       NKJKR LLQMNHEEDDDCCCCCCCCCCCCBBBBBBB
AAAAAABBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDEEEEEEFFGHK   MKJIJO  N R  X      YUSR PLV LHHHGGHIOJGFEDDDCCCCCCCCCCCCBBBBBBBB
AAAAAAABBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDEEEEEFGGHIIHHHHHIIIJKMR        VMKJIHHHGFFFFFFGSGEDDDDCCCCCCCCCCCCBBBBBBBBB
AAAAAAABBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDEEEEFFFFFFGGGGHIKP           KHHGGFFFFEEEEEEDDDDDCCCCCCCCCCCBBBBBBBBBBB
AAAAAAAABBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDEEEEEFFFFFGGHJLZ         UKHGFFEEEEEEEEDDDDDCCCCCCCCCCCCBBBBBBBBBBBB
AAAAAAAAABBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDEEEEEEFFGQPUVOTY   ZQL[MHFEEEEEEEDDDDDDDCCCCCCCCCCCBBBBBBBBBBBBBB
AAAAAAAAAABBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDEEEEEEFFGHIJKS  X KHHGFEEEEEDDDDDDDDDCCCCCCCCCCBBBBBBBBBBBBBBBB
AAAAAAAAAAABBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDEEEEEEFGGHHIKPPKIHGFFEEEDDDDDDDDDCCCCCCCCCCBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAABBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDEEEEEFFGHIMTKLZOGFEEDDDDDDDDDCCCCCCCCCBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAABBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDEEEEFFFI KHGGGHGEDDDDDDDDDCCCCCCCCCBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAABBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDEEEFGIIGFFEEEDDDDDDDDCCCCCCCCCBBBBBBBBBBBBBBBBBBBBBBBBBB

```