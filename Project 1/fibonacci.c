/*************************************************/
/*
/* Project Name: Project 1: Check the Mathematics
/* Description: Computes the first 50 integers in 5 different number sequences.
/* File names: main.c, lazy.c, primes.c, fibonacci.c, stoppingtime.c, happynumbers.c
/* Date: January 30, 2015
/* Authors: Jared Latham and Ryan Schubert
/*
/*************************************************/

#include <stdio.h>

void fibonacci()
{
	
	long long current;	//Current number in the sequence.
	long long prev = 0;	//Previous number.
	long long pprev = 0;	//Number that precedes the previous number.
	int i;			//Iterator for the for() loop.	
	
	printf("Fibonacci Sequence:\n");
	//Loops 50 times allowing the sequence to run 50 times.
	for(i=0;i<50;++i)
	{
		current = prev + pprev;
		// This is to prevent the sequence from entering an infinite 
		// loop while all the numbers are 0 at the start.
		if(current == 0)
			current = 1;
		printf("%lld ", current);
		pprev = prev;
		prev = current;
	}
	printf("\n\n");
	
}
