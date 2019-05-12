# CSC 415 - Project 4 - Thread Racing

## Student Name: Joe Binalinbing

## Student ID   : 918501338

## Build Instructions
We create a new thread. Both the main thread, sub and adder thread then increment or decrement a global variable MAX_ITERATIONS times.

## Run Instructions

1. cd to directory
2. make
3. ./pthread_race

## Explain why your program produces the wrong output
The add and sub function are accessing the global value and both function try to change it at the same time. Because the thread scheduling algorithm can swap between threads at any time, you don't know the order in which the threads will attempt to access the shared data. Therefore, the result of the change in data is dependent on the thread scheduling algorithm, i.e. both threads are "racing" to access/change the data.
