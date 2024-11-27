Write a set of functions to implement a heap-based memory manager. 
Your mission is to write a module implementing malloc, free, and realloc. Acquire a block of memory as described below (the address and size of this data are variables in your module). Create all structures in this block so that subsequent calls to these functions work correctly. Use a minheap for free blocks in your final version, but you might choose to use an array to get things working initially. Don’t forget that malloc ensures that allocated blocks have been reset to 0.

Implementing and demonstrating (via tests) thread safety is worth EXTRA CREDIT.

Module
This code should be in a .h and .c that are linked in with your test program. The test program(s) should work with NO changes between the system version and your version – the only difference is if your module is present.

Incremental Development
It is STRONGLY recommended that you pursue this incrementally – add pieces one at a time, testing between parts. For example, the first part is to build the test program. The second is implementing the module (with names like “malloc”) that uses the real malloc/free. Then, save your allocated/free blocks in a simple array. Then, save the free blocks in a heap. Then, attach the block size to the allocated memory (in a void* immediately BEFORE the chunk).

Tests
Testing is critical for this project. Consider allocation patterns (small, then large, same size) and frees (allocs and interspersed frees). Once allocated, the test program should fill the allocated memory with a test pattern.

Write these first as a test cases document, then implement a test program that includes all the tests. The test program should execute all tests unless a “-t num” option is present, in which case just that single test is executed. Obviously, output the name of the test and a pass/fail. It’s strongly suggested that each test is a function.

It’s strongly suggested that you write this with “normal” malloc before you start work on your version. Extend this as you find new things to test.

You may have several test programs that you write and use. That’s great, but do check in this big one.

Recommended: Have a Makefile with two targets: one that links your test program without your malloc (uses the system ones), and one that links with your malloc functions. The output should be equivalent (modulo using random numbers).

Acquiring a Block of Memory
To implement this, you need to have a personal chunk of memory. You will be asking the system for a block of your own. It does this by increasing the size of the data segment so it is NOT shared with the heap or anything else. It’s all yours! THIS IS NOT HOW YOU MALLOC. This is the SPACE in which you manage LOTS of mallocs. (You should probably size this block to fit your tests.) 

 

void* get_me_blocks( ssize_t how_much ) {

void* ptr = sbrk(0);

sbrk(how_much);

return ptr;

}

 

Error checking
Write messages to stderr if errors are detected. Personally, I'd have lots of logging that can be compiled in or out.

