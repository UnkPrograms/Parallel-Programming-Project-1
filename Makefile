all: sortSeq sortThread

sortSeq: sortSeq.c 
	gcc -g -Wall -std=gnu99 -o sortSeq sortSeq.c

sortThread: sortThread.c 
	gcc -g -Wall -std=gnu99 -o sortThread sortThread.c -lpthread

clean:
	rm -f sortSeq sortThread
