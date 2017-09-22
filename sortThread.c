/* Name: Ryunki Song
        Class: CS 352
        Instructor: Lee Savoie
        File: wf.c
        Description: This program reads files and counts the number of words that
        *            occur in each file. The method used to track the occurences
        *            is through an array of linked lists where it's index is calculated 
        *            by a hash function
        *           
        *           gcc -Wall -std=gnu99 -o unixfork1 unixfork1.c 
        *
        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

// #include "quickSort.c"
// #include "mergesort.c"


/*----------------------------------------Global Variables-----------------------------------*/
#define MAX_FILE_LENGTH 1000000         // Max of 1 million lines in a file
char *lineArray[MAX_FILE_LENGTH];       // Array consisting of all lines in the text    
int numProcess;
int lineCount;
char **arrayPtr;
int partitionCount, mergeCount;
int isPartitionComplete, isMergeComplete, isHalfArrayComplete, isSetArrayComplete;
int indexTracker;
int isPartition1, isPartition2, isMerge1, isMerge2;
int partitionArray[100];



void createPartitionThread();
void *myPartitionThread();
void mergeThread();
void *myThreadMerge();
void halfPartitionArray();
void setPartitionArray();

int mergeSort(char *arrayPtr[], int partition1, int partition2);
void quickSort(char *array[], int len);



int main(int argc, char *argv[]){
/*---------------------------------------------SETUP----------------------------------------*/
    // Check for correct number of arguments    
    if(argc != 3){
        fprintf(stderr, "The 3 arguments should be sortThread, number of threads, and the textfile.\n");
        exit(-1);
    }
    

    numProcess = atoi(argv[1]);         // Get number of processes from user input    
  //   if(!isdigit(numProcess)){
  //    fprintf(stderr, "The second argument must be an integer.\n");
        // return -1;
  //   }

    // Since we know that the 2nd agrument is a power of two, we can figure out x in 2^x for the given arg
    int power = 0;
    int num = numProcess;
    while(num>1){
        num/=2;
        power++;
    }

    // printf("POWER: %d\n", power);

    FILE *fp; 
    fp = fopen(argv[2], "r");               // Open the text file passed in as the third argument
    
    if(fp == NULL){                         // Error checking
        perror("Error opening file");
        exit(-1);
    }

    #define MAX_LINE_LENGTH 200
    char line[MAX_LINE_LENGTH];                             // Max length of a word can only be 200 chars
    int i=0;                                                // Increment for the wordArray

    // Store each line in the text in the array
    while(fgets(line, MAX_LINE_LENGTH, fp)) {
//        printf("%s", line);
        char *str = strdup(line);
        lineArray[i] = str;                                 // Store first letter of each line in the array
        i++;                                                // Go to next index of wordArray
    }

    lineArray[i] = '\0';                                    // Place null ending character
    lineCount = i;                                          // Set the number of lines in the text

    setPartitionArray();

    if(numProcess > lineCount){                             // Error checking
        perror("Number of processes should not exceed number of lines in the textfile.");
        exit(-1);
    }
   // printf("Number of lines in the file: %d\n", lineCount); // Print number of lines
/*-----------------------------------------END SETUP------------------------------------------*/



/*------------------------------START RUNNING AND TIMING QUICK SORT---------------------------*/
    // Now create the processes and sort

    // Set up variables and call the function to save the time when quickSort is called
    struct timeval startTime, endTime;
    int seconds, micros;
    gettimeofday(&startTime, NULL);

    // Create the process and start quicksort    
    arrayPtr = lineArray;                            // This ptr points to the first element in the array
    createPartitionThread();                         // Create the sorted partitions in the array

    while(!isPartitionComplete);                     // Do nothing until partition is complete

    // arrayPtr= lineArray;
//     //DEBUG Prints below
//     int z=0;                                                    // Reset the increment
//     while(arrayPtr[z] != '\0'){
//         printf("%s", arrayPtr[z]);
//         z++;
//     }
// printf("-------------------------------------------------------------------------\n");


    for(int j=0; j<power; j++){
        isSetArrayComplete=0;
        arrayPtr = lineArray;                        // Reset the pointer to first element
        mergeThread();   
        while(!isMergeComplete && !isHalfArrayComplete);  // Do nothing until merging is complete
        numProcess /= 2; 


    }

    gettimeofday(&endTime, NULL);
    seconds = endTime.tv_sec  - startTime.tv_sec;
    micros  = endTime.tv_usec - startTime.tv_usec;
    if ( endTime.tv_usec < startTime.tv_usec ) {
        micros += 1000000;                                  // one million
        seconds--;
    }
/*------------------------------END RUNNING AND TIMING QUICK SORT----------------------------*/


    if (fclose(fp) == EOF) {                                // Error checking
        perror("Error closing file");
        return 1;
    }

    while(!isPartitionComplete || !isMergeComplete);

    //DEBUG Prints below
    i=0;                                                    // Reset the increment
    while(lineArray[i] != '\0'){
        printf("%s", lineArray[i]);
        i++;
    }
    // printf("SEG FAULT CHECK 8\n");
    fprintf(stderr, "runtime: %d seconds, %d microseconds\n", seconds, micros);
    
    return 0;
}
/*----------------------------------_END OF MAIN -------------------------------------------*/


/*-----------------------------Partition the Array and Sort---------------------------------*/
void *myFirstPartitionThread( void *arg ){
    int partition = partitionArray[0];

    arrayPtr = lineArray;
    quickSort(arrayPtr, partition);
    isPartition1=1;
    arrayPtr += partition;

    while(partitionCount<numProcess){
        if(isPartition1==1 && isPartition2==numProcess-1){
            isPartitionComplete=1;
            pthread_exit( NULL );
            break;
        }
    }
    pthread_exit( NULL );
}


void *myPartitionThread( void *arg ){
    partitionCount++;
    int partition = partitionArray[partitionCount];          // Size of the array for each process
    int leftover = partitionArray[0] - partitionArray[1];

    arrayPtr = lineArray;
    arrayPtr += partitionCount*partition + leftover;
    quickSort(arrayPtr, partition);   
    arrayPtr += partition;

    isPartition2++;

    pthread_exit( NULL );
}

void createPartitionThread(){
  
    long numIters = numProcess;

    /* thread ids and attributes */
    pthread_t id;  
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  
    
    isPartition1=0; isPartition2=0; isPartitionComplete = 0; partitionCount=0;
    for (int i=0; i < numProcess; i++) {
        if(i==0)
            pthread_create( &id, &attr, myFirstPartitionThread, (void *) numIters );
        else
            pthread_create( &id, &attr, myPartitionThread, (void *) numIters );
    }
}
/*--------------------------End Partition the Array and Sort---------------------------------*/

/*----------------------------------Merge the Array and Sort---------------------------------*/
void *myFirstThreadMerge( void *arg ){
    int partition1 = partitionArray[0];
    int partition2 = partitionArray[1];
    

    arrayPtr = lineArray;
    indexTracker++;

    mergeSort(arrayPtr, partition1, partition2);
    isMerge1 = 1;


    while(mergeCount<(numProcess/2)){
        if(isMerge1==1 && isMerge2==numProcess/2-1){
            halfPartitionArray();
            isMergeComplete=1;

            pthread_exit( NULL );
            break;
        }
    }

    //UNSURE IF THIS CODE IS EVER REACHED
    halfPartitionArray();    
    isMergeComplete=1;
    pthread_exit( NULL );
}


void *myThreadMerge( void *arg ){


    mergeCount++;
    int index = 2*mergeCount;

    int partition1 = partitionArray[index];
    int partition2 = partitionArray[index+1];

    // int leftover = partitionArray[0] - partitionArray[1];

    arrayPtr = lineArray;
    arrayPtr += partitionArray[0] + partitionArray[1] + 2*partition1*(mergeCount-1);

    indexTracker++;
    mergeSort(arrayPtr, partition1, partition2);   



    isMerge2++;
    pthread_exit( NULL );
}

void mergeThread(){

    long numIters = numProcess;

    /* thread ids and attributes */
    pthread_t id;  
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  

    mergeCount=0; isMerge1=0; isMerge2=0; isMergeComplete=0; isHalfArrayComplete=0; indexTracker = 0;
    for (int i=0; i < numProcess/2; i++) {

        if(i==0){
            pthread_create( &id, &attr, myFirstThreadMerge, (void *) numIters );
            int indexPrev = indexTracker;
            while(indexPrev == indexTracker);                               // Do nothing until this sort finishes
        }
        else{
            pthread_create( &id, &attr, myThreadMerge, (void *) numIters );
            int indexPrev = indexTracker;
            while(indexPrev == indexTracker);                               // Do nothing until this sort finishes
        }

    }

}

/*----------------------------------Merge the Array and Sort-------------------------------*/


void halfPartitionArray(){
    for(int i=0; i<numProcess/2; i++){
        partitionArray[i] = partitionArray[2*i] + partitionArray[2*i+1];  
    }
    isHalfArrayComplete = 1;
}

void setPartitionArray(){
    int lineCountCopy = lineCount;
    int x=0;
    while(x < numProcess){                                  // Calculate partition array

        int partition = lineCountCopy/numProcess;
        int leftover = lineCountCopy - partition*numProcess;

        if(x==0)
            partitionArray[x] = partition + leftover;
        else
            partitionArray[x] = partition;

        x++;
    }
    isSetArrayComplete=1;
}











/*--------------------------------------------Merge Sort---------------------------------------------*/
/*
*   Parameters: first and last pointers point to the first and last elements in the array.
*               size holds the value of the size of the array.
*               **ptr is points to the elements of the array, so that when its incremented it goes to
*               the next element in the array. Note first and last when incremented goes to the next 
*               character of the String and not the next element in the array which is why we have **ptr
*
*/
int mergeSort(char *arrayPtr[], int partition1, int partition2){

    // printf("partition1 %d, partition2 %d\n", partition1, partition2);

    char *array1[partition1];
    char *array2[partition2];
    // char *retArray[lineCount];


    // //DEBUG Prints below
    // int z=0;                                                    // Reset the increment
    // while(z<partition1+partition2){
    //     printf("%s", arrayPtr[z]);
    //     z++;
    // }
    // printf("----------------------------------------------------------------------------\n");

    // Copy data to temp arrays 
    int i=0, j=0, k=0;
    for (i = 0; i < partition1; i++)
        array1[i] = arrayPtr[i];

    for (j = 0; j < partition2; j++)
        array2[j] = arrayPtr[partition1+j];

    // Compare and sort
    i=0, j=0, k=0;
    while(j<partition1 && k < partition2){
        int val = strcmp(array1[j], array2[k]);
         
        if(val<=0){
            arrayPtr[i] = array1[j];
            j++;

        }
        else{
            arrayPtr[i] = array2[k];
            k++;

        }
        i++;
    }   
 


    // Copy remaining elements if any
    while (j < partition1){
        arrayPtr[i] = array1[j];
        j++;
        i++;
    }

    // Copy remaining elements if any
    while (k < partition2){
        arrayPtr[i] = array2[k];
        k++;
        i++;
    }

 // z=0;                                                    // Reset the increment
 //    while(z<partition1+partition2){
 //        printf("%s", arrayPtr[z]);
 //        z++;
 //    }
 //    printf("-------WTF-------------------------------------------------------------------\n\n");

    return 1;
}


/*--------------------------------------------Quick Sort---------------------------------------------*/

//Declare some functions
void swap();
int median();

/*
*   Parameters: first and last pointers point to the first and last elements in the array.
*               size holds the value of the size of the array.
*               **ptr is points to the elements of the array, so that when its incremented it goes to
*               the next element in the array. Note first and last when incremented goes to the next 
*               character of the String and not the next element in the array which is why we have **ptr
*
*/


/*
*   Function to swap two pointers in *array[]
*/
void swap(char **arg1, char **arg2){
    char *tmp = *arg1;
    *arg1 = *arg2;
    *arg2 = tmp;
}

void quickSort(char *array[], int len){
    // printf("Len =  %d\n", len);

    if (len <= 1)                                           //No need to sort an array of length 1
        return;
        
    // Take the first, middle, and last element in the array and find the median between the 3 
    char *first = array[0];
    char *middle = array[len/2];
    char *last = array[len-1];
    int pivotIndex = median(first, middle, last, len);


    //Swap pivot with the last element in the array
    swap(array+pivotIndex, array+len-1);
    char *pivot = array[len-1];

    // Var i increments through the array, wall keeps track of the index to be swapped 
    // NOTE: wall does not necessarily increment every loop but i does.
    int i=0;
    int wall=0;


    // Perform do-while until we reach the last element in the array
    do{
        while( strcmp(array[i], pivot) > 0 && i<len-1){     //Keep incrementing if its bigger than the pivot
            i++;
            // printf("i: %i\n", i);
        }
        if(i<len-1){                                        // Otherwise perform a swap, so that the element

            swap(array+wall, array+i);                      // smaller than the pivot is now at the left-most 
            i++;                                            // index in the array at the wall.
            wall++;                                         // Make sure to increment i and the wall indexes
        }     
    }
    while(i<len-1);                                         

    swap(array+len-1, array+wall);                          // Move the pivot value into its proper place

    quickSort(array, wall);                                 // Recall quickSort and make sure not to include 
    quickSort(array+wall + 1, len - (wall + 1) );           // the pivot
    // printf("-------------------------------------------------------\n\n");
}

int median(char *first, char* middle, char *last, int len){
    // If first is the median
    if ( (strcmp(first, middle) > 0 && strcmp(first, last) < 0 ) ||
         (strcmp(first, middle) < 0 && strcmp(first, last) > 0 )
        )
        return 0;
    // If middle is the median
    else if ( (strcmp(middle, first) > 0 && strcmp(middle, last) < 0 ) ||
              (strcmp(middle, first) < 0 && strcmp(middle, last) > 0 )
            )
        return len/2;

    // Otherwise pick the last element to be the median
    else
        return len-1;
}
