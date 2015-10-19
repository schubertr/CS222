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

void stoppingtime()
{

	int operations = 0;	//Number of operations performed on the number.
	int i;			//Holds the values from 1-50.
	int num;		//Holds the number being performed on.
	
	printf("Collatz Stopping Times:\n");
	//Loops from 1-50 computing the stopping times for each integer.
	for(i=1; i<=50; ++i)
	{
		num = i;
		//Loops and performs the different actions on num until it is 1.
		while(num != 1)
		{
			//If num is an even number, it will divide by 2.
			//If num is an odd number, it will multiply by 3 and add 1.
			if((num%2) == 0)
				num = num / 2;
			else
				num = (num * 3) + 1;
			operations++;
		}
		printf("%d ", operations);
		operations = 0;
	}
	printf("\n\n");

}
