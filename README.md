# Parallel-Programming-Project-1
Project in which we use Threads to sort lines of data from a book

NOTES: 
1. The programs were ran and tested on linux machines.
2. The goal of this project was to sort each line in the textfiles using a sequential sort and a concurrent sort using Threads 
3. The strcmp function determined which line was greater or less than

SortSeq Description:
A sequential sort using the quicksort algorithm to sort lines of text in the textfiles. 

SortThread Description:
Uses n threads to partition the text into n sorted parts concurrently. This will result in n blocks of the text being sorted. 
Then for each iteration, we create n/2 threads to merge each pair of sorted blocks until we have one final sorted text. 
Example: Lets say we have 8 threads. Then the textfile will be partitioned into 8 sorted parts using quicksort. Once all threads have finished running ie. we have 8 partitioned sorted parts, then we create 4 more threads so that we merge each pair of sorted parts which will results in 4 sorted partitions. Then we create 2 more threads to merge each pair so that we have 2 sorted paritions. Finally we create one last thread to merge the final pair so that we end up with a sorted textfile. 
NOTE: n is always a power of 2.

How to run:
1. Use command 'make' or 'make all'
2. Then run the script using './script.sh'  - This will run both sortSeq and sortThread to create the output files in the output directory
3. Then run './output.sh' to compare the differences to the output files versus the already given sorted files.

NOTE: For sortThread, script.sh inputs 4 as the number of threads to use to sort the each textfile. If less/more threads are desired to sort the textfiles you can manually run the program as follows
1. Use the command 'sortThread n text/sawyer.txt' where n represents the amount of threads desired (make sure it is a power of 2).
2. So if 8 threads are desired then use 'sortThread 8 text/sawyer.txt' with whatever textfile you want to sort.
3. This will print out the sorted textfile

Similarly, sortSeq runs the same but there is no integer input, only the textfile you want to sort.
Example: 'sortSeq text/sawyer.txt'
This will purely sort the textfile using a quicksort algorithm.

NOTE: You can see the differences in how long the sorting process takes between sortSeq and sortThread, as well as how long it takes sortThread to sort with different number of threads.

