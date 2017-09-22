#!/bin/bash

#Diff output from QuickSort and Dr. Homer's sorted text
echo "Running diff with sortSeq output and Solution in text directory"

diff output/sortSeqBiggerOutput.txt text/biggerSorted.txt
echo "diff done for bigger.txt"

diff output/sortSeqBigOutput.txt text/bigSorted.txt
echo "diff done for big.txt"

diff output/sortSeqGettysburg.txt text/gettysburgSorted.txt
echo "diff done for gettysburg.txt"

diff output/sortSeqMobyDick.txt text/mobydick.txt
echo "diff done for mobydick.txt"

diff output/sortSeqRaven.txt text/ravenSorted.txt
echo "diff done for raven.txt"

diff output/sortSeqSawyer.txt text/sawyerSorted.txt
echo "diff done for sawyer.txt"


#Diff output from sortThread and Dr. Homer's sorted text
echo "Running diff with sortThread output and Solution in text directory"

diff output/sortThreadBiggerOutput.txt text/biggerSorted.txt
echo "diff done for bigger.txt"

diff output/sortThreadBigOutput.txt text/bigSorted.txt
echo "diff done for big.txt"

diff output/sortThreadGettysburg.txt text/gettysburgSorted.txt
echo "diff done for gettysburg.txt"

diff output/sortThreadMobyDick.txt text/mobydick.txt
echo "diff done for mobydick.txt"

diff output/sortThreadRaven.txt text/ravenSorted.txt
echo "diff done for raven.txt"

diff output/sortThreadSawyer.txt text/sawyerSorted.txt
echo "diff done for sawyer.txt"