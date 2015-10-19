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

int lazy()
{
	int i = 0;

	printf("Lazy Caterer's Sequence:\n"); //prints label

	for(i = 0 ; i < 50 ; i++)
	{
		printf("%d ", ( ( (i*i) + (i) + (2) ) / 2 ));	// prints out the numbers
								// following the formula
	}
	printf("\n\n");
	return 0;
}
