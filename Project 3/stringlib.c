/*******************************/
/*
/*  Project Name: Project 3: String 'Em Up
/*  Description: "The C standard library has a string library that supplies many of these functions, and for this project you must implement your own version of the some of it."
/*  File names: stringlib.c new_string.h makefile
/*  Date: Friday, March 13, 2015 at 11:59pm
/*  Authors: Ryan Schubert & Josh Hughes
/*
/*******************************/

#include <stddef.h>
#include "new_string.h"
#include <limits.h>

/*
*	var: char* & const char*
* return: char*
*	Copies the characters from source into destination.
*/
char* new_strcpy(char* destination, const char* source)
{
	int length = (int) new_strlen(source);
	int i;
	for(i = 0 ; i < length + 1 ; i++) //adds each character from source to the destination
		destination[i] = source[i];

	return destination; //returns the new copy
}

/*
*	var: char* & const char*
* return: char*
*	Copies the first n characters from source into destination. If the function hits a null character in source before it has copied n characters, the remaining characters are filled with null characters.
*/
char* new_strncpy(char* destination, const char* source, size_t n)
{
	int i;
	
	for(i = 0; i < n && source[i] != '\0'; i++) //goes n number of times or until null character is reached
		destination[i] = source[i]; //copies each character over from source to destination

  while(i < n)
	{
		destination[i] = '\0'; //fills the remaining characters as null characters
		i++;
	}

return destination;

}

/*
*	var: const char* & const char*
* return: int
*	Compares two strings. The return value is positive if string1 comes after string2 alphabetically, negative if string1 comes before string2 alphabetically, and 0 if the two strings are equal.
*/
int new_strcmp(const char* string1, const char* string2)
{
	int i = 0;

	while(string1[i] != '\0') //compares them until it hits a null character in string1
		{
			if(string2[i] == '\0') //returns 1 if string2 is empty but string1 is not
				return 1;
			if(string2[i] > string1[i]) //returns -1 if string1 comes before string2
				return -1;
			if(string1[i] > string2[i]) //returns 1 if string2 comes before string1
				return 1;
				
			i++;
		}

	if(string2[i] != '\0') //returns -1 if string1 is empty but string2 is not
		return -1;

	return 0; //returns 0 if they are the same
}

/*
*	var: const char* & const char* & size_t
* return: int
*	Compares two strings exactly like new_strcmp(), except comparing at most the first n characters.
*/
int new_strncmp(const char* string1, const char* string2, size_t n)
{
	size_t i = 0;

	while(string1[i] != '\0' && i < n) //compares them until it hits a null character in string1 or n number of times
		{
			if(string2[i] == '\0') //returns 1 if string2 is empty but string1 is not
				return 1;
			if(string2[i] > string1[i]) //returns -1 if string1 comes before string2
				return -1;
			if(string1[i] > string2[i]) //returns 1 if string2 comes before string1
				return 1;
				
			i++;
		}

	if(string2[i] != '\0')  //returns -1 if string1 is empty but string2 is not
		return -1;

	return 0; //returns 0 if they are the same
}

/*
*	var: char* & const char*
* return: char*
*	Adds the string contained in source to the end of the string contained in destination. The values in destination are modified, but a pointer to destination is also returned.
*/
char* new_strcat(char* destination, const char* source)
{
	new_strncat(destination, source, INT_MAX); //calls new_strncat with the n parameter as INT_MAX so it goes until a NULL character
}

/*
*	var: char* & const char* & size_t
* return: char*
*	Adds the string contained in source to the end of the string contained in destination, but adding a maximum of n characters. 
*/
char* new_strncat(char* destination, const char* source, size_t n)
{
	size_t length = new_strlen(destination);
	size_t i;

	for (i = 0 ; i < n && source[i] != '\0' ; i++) //goes through the source until null character or n number of times
		destination[length + i] = source[i]; //adds the character in source to the end of destination
	destination[length + i] = '\0'; //adds a null character at the very end

	return destination;
}

/*
*	var: const char*
* return: size_t
*	Returns the number of characters in string before the null character.
*/
size_t new_strlen(const char* string)
{
	size_t length = 0; 

	while(string[length] != '\0') //goes through the string until null character
		length++; //adds 1 to length for each non null character there is in the string
	
	return length; //returns the length
}

/*
*	var: const char* & int
* return: char*
*	Returns a pointer to the first occurrence of character (converted to a char) in string or a NULL pointer if character cannot be found. 
*/
char* new_strchr(const char* string, int character)
{
	int i;
	int length = new_strlen(string);

	for(i = 0 ; i < length ; i++) //goes through the length of the string
		if(string[i] == (char) character)
			return (char*) &string[i];	//if it reaches a character that matches the character provided, it returns the first instance of the character.
	
		return NULL; //otherwise returns null
}

/*
*	var: const char* & const char*
* return: char*
*	Returns a pointer to the first occurrence of the string contained in needle in haystack or a NULL pointer if needle cannot be found.
*/
char* new_strstr(const char* haystack, const char* needle)
{
	int i;
	for(i = 0 ; i < new_strlen(haystack) ; i++) //goes through the length of the haystack
		{
			int j = 0;
			if(haystack[i] == needle[j]) //if it finds the first character in needle
			{
				while(haystack[i+j] == needle[j]) //loop through the and check to see if the next characters match.
					j++;
				if(needle[j] == '\0') //if the null character in needle is reached, that means we have a match
					return (char*) &haystack[i]; //return the location in haystack where the needle is
			}
		}
	return NULL; //otherwise return null
}
