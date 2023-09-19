# Analyse-nums
The problem statement is as follows:
Create a program that :
1. Is able to store a very large number of integers (of which there may be duplicates).
2. Allows very quick access for find, add, delete, predecessor, successor, max and min operations.
3. During a normal run of the program, after the data is initialised, 10% of numbers will be randomly deleted and another 10% randomly added. Each of the other operations will occur the same number of times as the adds and deletes and the sequence of operations will be random.

The goal of this assignment is to develop a well structured, memory and computationally efficient C program to simulate the system and investigate possible techniques for achieving the requirements listed above. The program will take 3 integer parameters (n, r1, r2) from the command line with r1>0 and r2>r2. It will then generaten positive random numbers (duplicates allowed) in the range r1,.., r2 inclusive and store them in array data structure. Your program will implement all the functionality listed above. Each operation type (e.g. add) will be timed individually and the number of operations, average and total time taken for each operation type displayed.

Note:
Implement two test function that is funcitonality and performance test
1. performance test shows the performance of the program shown in sample 1 and 2
2. functionality test is the special test which runs when our n is negative . It includes performing random operation sequence(decide your own sequence) for every distinct integers and at the end ,restore the array.check the sample 3

