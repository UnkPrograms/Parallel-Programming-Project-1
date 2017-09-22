/* Name: Ryunki Song
        Class: CS 452
        Instructor: Homer
		File: wf.c
		Description: This program reads files and counts the number of words that
		*			 occur in each file. The method used to track the occurences
		* 			 is through an array of linked lists where it's index is calculated 
		*            by a hash function
		*			
		*			gcc -Wall -std=gnu99 -o unixfork1 unixfork1.c 
		*
        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
// #include "quickSort.c"


#define MAX_LINE_LENGTH 200
#define MAX_FILE_LENGTH 1000000         // Max of 1 million lines in a file


void quickSort(char *array[], int len);




int main(int argc, char *argv[]){

/*---------------------------------------------SETUP----------------------------------------*/
    // Check for correct number of arguments    
	if(argc != 2){
		fprintf(stderr, "The 2 arguments should be sortSeq and the textfile.\n");
		return -1;
    }
	
    char *lineArray[MAX_FILE_LENGTH];       // Array consisting of all lines in the text    
    // *lineArray = malloc(MAX_LINE_LENGTH * sizeof(char*));

    FILE *fp; 
    fp = fopen(argv[1], "r");               // Open the text file passed in as the second argument
    
    if(fp == NULL){                         //Error checking
		perror("Error opening file");
		return -1;
	}

    char line[MAX_LINE_LENGTH];                             // Max length of a word can only be 200 chars
    int i=0;                                                // Increment for the wordArray

    // Store each line in the text in the array
    while(fgets(line, MAX_LINE_LENGTH, fp)) {
        char *str = strdup(line);
        lineArray[i] = str;                                 // Store first letter of each line in the array
        i++;                                                // Go to next index of wordArray
	}
        
    lineArray[i] = '\0';                                    // Place null ending character
    int lineCount = i;                                      // Set the number of lines in the text
    // printf("Number of lines in the file: %d\n", lineCount); // Print number of lines
/*-----------------------------------------END SETUP------------------------------------------*/
    // printf("Last element in the array %s", lineArray[lineCount-1]);
	
/*------------------------------START RUNNING AND TIMING QUICK SORT----------------------------*/
    // Set up variables and call the function to save the time when quickSort is called
    struct timeval startTime, endTime;
    int seconds, micros;
    gettimeofday(&startTime, NULL);

    // Sort the array
    char **arrayPtr = lineArray;                            // This ptr points to the elements in the array
    quickSort(arrayPtr, lineCount); 

    // Call the function and see how long it took to sort the array
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


    // DEBUG Prints below
    i=0;                                                    // Reset the increment
    while(lineArray[i] != '\0'){
        printf("%s", lineArray[i]);
        i++;
    }
 
    fprintf(stderr, "runtime: %d seconds, %d microseconds\n", seconds, micros);

    
    return 0;
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
