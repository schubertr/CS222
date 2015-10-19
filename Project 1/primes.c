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

int primes()
{
	int i = 2;
	int j = 2;		//declaration of variables
	int prime;
	int count = 50;

	printf("Prime Numbers:\n"); //prints the label

	for(i = 2 ; count > 0 ; i++)
	{	
		prime = 1;		//1 if prime, 0 if not prime
		
		for(j = 2 ; j <= i ; j++)
			if( i % j == 0  && j != i )
				prime = 0;	// if the number is evenly divisible by any
						// number less than it, not prime
		if(prime == 1)
		{
			printf("%d ", i); //otherwise it is prime and we print it out.
			count--;
		}
	}
	printf("\n\n");
	return 0;
}
