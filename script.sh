#!/bin/bash

#Create output from QuickSort
echo "running sortSeq on bigger.txt"
./sortSeq text/bigger.txt > output/sortSeqBiggerOutput.txt

echo "running sortSeq on big.txt"
./sortSeq text/big.txt > output/sortSeqBigOutput.txt

echo "running sortSeq on gettysburg.txt"
./sortSeq text/gettysburg.txt > output/sortSeqGettysburg.txt

echo "running sortSeq on mobydick.txt"
./sortSeq text/mobydick.txt > output/sortSeqMobyDick.txt

echo "running sortSeq on raven.txt"
./sortSeq text/raven.txt > output/sortSeqRaven.txt

echo "running sortSeq on sawyer.txt"
./sortSeq text/sawyer.txt > output/sortSeqSawyer.txt


#Create output from Threads
echo "running sortThread on bigger.txt"
./sortThread 4 text/bigger.txt > output/sortThreadBiggerOutput.txt

echo "running sortThread on big.txt"
./sortThread 4 text/big.txt > output/sortThreadBigOutput.txt

echo "running sortThread on gettysburg.txt"
./sortThread 4 text/gettysburg.txt > output/sortThreadGettysburg.txt

echo "running sortThread on mobydick.txt"
./sortThread 4 text/mobydick.txt > output/sortThreadMobyDick.txt

echo "running sortThread on raven.txt"
./sortThread 4 text/raven.txt > output/sortThreadRaven.txt

echo "running sortThread on sawyer.txt"
./sortThread 4 text/sawyer.txt > output/sortThreadSawyer.txt
