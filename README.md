# Parallel-Programming-Project-1
Project in which we use Threads to sort lines of data from a book

NOTES: 
1. The programs were ran and tested on linux machines.
2. The goal of this project was to sort each line in the text files using a sequential sort and a concurrent sort using Threads 

SortSeq Description:
A sequential sort using the quicksort algorithm. 

How to run SortSeq
1. Use command 'make' or 'make all'
2. Then 

How to Run the program
1. cd into Phase1
2. Run ./script.sh  - This will run the program with the testcases and place the output files into the output directory
3. Run ./output.sh  - This will run a diff on the files in the output directory versus the testResults directory which were given to us
4. If scripts do not run, change permissions using chmod


NOTE: Not all tests 00-36 were run because some of the output requires that we dump and print out our entire process table and it is unlikely that the process table data would match exactly with the process table data in testResults. The script output.sh only runs diff on the outputs in which we were not required to print our process table.

Also, the code concerning how to fork, create, kill, block, zap, and join processes is in phase1.c. 
