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

void happynumbers()
{

	int i = 1;		//Holds the number that is being checked.
	int num;		//Holds the value in i, but has operations done to it.
	int sum = 0;		//The sum of all the digits in a number.
	int digit;		//Holds individual digits in the number.
	int printCount = 0;	//Counts how many numbers have been printed.
	
	printf("Happy Numbers:\n");
	//Loops until 50 happy numbers are printed.
	while(printCount < 50)
	{
		num = i;
		sum = 0;
		//Loops until the sum becomes 1 or 4.
		while(sum!=1&&sum!=4)
		{
			sum = 0;
			//Adds up the digits in num.			
			while(num > 0)
			{
				digit = num%10;
				sum += digit*digit;
				num = num/10;
			}
			//The sum becomes the new number that is checked.
			num = sum;
		}
		//Prints out the number if the sum is 1.
		if(sum == 1)
		{
			printf("%d ", i);
			printCount++;
		}
		
		i++;
	}
	printf("\n");

}
