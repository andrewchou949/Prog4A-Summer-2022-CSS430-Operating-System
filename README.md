# Prog4A-Summer-2022-CSS430-Operating-System

This assignment is focusing on heap space management concept. Typically, it would consist of malloc function and free function to request for memory space to allocate and space to deallocate back. This malloc.cpp file implements a non-default malloc functions and free function using "first-fit" and best-fit" concept by searching through the free or available space in Thread Control Block (TCB) and compare accordingly to determine whether it's the best-fit or not.

Note: the driver.cpp file is a source-code provided file to test out the malloc.cpp functions., All implementation are done on the malloc.cpp file.

# To run

In console or IDE terminal,

1. Locate the file directory
2. Compile driver.cpp file to test out the malloc.cpp functions
3. then "strace ./a.out l 2>&1 | grep brk" to use default malloc function, or "strace ./a.out b 2>&1 | grep brk" to use the best-fit malloc function, or "strace ./a.out f 2>&1 | grep brk" to use the first-fit malloc function.
