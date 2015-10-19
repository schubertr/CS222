 /*******************************************************************************************************************************************/
 /*
 /*  Project Name: Trees Worse than Whomping Willows
 /*  Description: Have a roster that a user can add, search, and kill students.  Thats also allows points to be added to invidual users and 
 /*  	display the current scores of each house.  Print all the studets in a preorder, postorder, and inorder traversals.
 /*  File names: rosterRunner.c, Student.h, makefile
 /*  Date: 10 April 2015
 /*  Authors: Bradley Levine Ryan Schubert
 /*
 /*******************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include "Student.h"

//function prototypes
void printStudent(Student*);
void inorder(Student*);
void preorder(Student*);
void postorder(Student*);
void clear(Student*);
void score(Student* root, int* sum);
void fpreorder(FILE* file, Student* root);
void points(Student* root, char first[], char last[], int num, char house[]);
int check(char house[], char first[], char last[], int year, Student* root, char command[], int prt);
Student* search(Student*, char[], char[]);
Student* insert(Student*, char[], char[], int, int, int);
Student* kill(Student* root, char first[], char last[]);
Student* parentOf(Student* root, char first[], char last[]);
Student* leftMostRight(Student* root);
	
int main()
{
	Student* houses[5] = {NULL, NULL, NULL, NULL, NULL};	//array of 4 pointers to the root of the house threes

	char command[10];	//holds the command the user enters
	
	printf("Enter command: ");
	scanf("%s", command);
	
	//loops until the user enters the quit command
	while(!(strncmp(command, "quit", 4) == 0))
	{
		//prints out a help menu
		if(strncmp(command, "help", 4) == 0)
		{
		    printf("help\nDisplays a help screen explaining the 13 commands.\n\n");
		    printf("load <filename>\nAdds all the student records in <filename> to the roster. The file is arranged one student per line where each record has the following format.\n\n");
		    printf("save <filename>\nSave the records of all living students in the roster to <filename>. The format of the file should match what is read in by the load command. To match sample output, please print out the students in each house starting with Gryffindor, then Ravenclaw, then Hufflepuff, and finally Slytherin. Each house should be printed in a preorder traversal. That way, the file will match the structure of the trees you have generated.\n\n");
		    printf("clear\nClear the rosters for the four houses and for the list of deceased students. Delete all allocated memory.\n\n");
		    printf("inorder\nPrint out each house in an inorder traversal.\n\n");
		    printf("preorder\nPrint out each house in a preorder traversal.\n\n");
		    printf("postorder\nPrint out each house in a postorder traversal.\n\n");
		    printf("add <firstname> <lastname> <points> <year> <house>\nAdd a student with the given attributes to the roster.\n\n");
		    printf("kill <firstname> <lastname> <house>\nFind the given student in the given house's roster, remove the student from the house's roster, and add the student to the list of deceased students.\n\n");
		    printf("find <firstname> <lastname> <house>\nFind the given student in the given house's roster and print out his or her data.\n\n");
		    printf("points <firstname> <lastname> <house> <number>\nFind the given student in the given house's roster and add <number> points to their point total. Note that <number> can be negative.\n\n");
		    printf("score\nPrint out the current point scores for all houses. Points for deceased students are not factored into their house totals.\n\n");
		    printf("quit\nQuit the program. This command should automatically call the clear command, freeing all allocated memory.\n\n");
		    
		}
		else if(strncmp(command, "load", 4) == 0)
		{
			char filepath[75];
			scanf("%s", filepath);
			
			FILE* filePointer;	//pointer to file
			filePointer = fopen(filepath, "r");//opens file

			if(filePointer != NULL)
			{
			    int read;
			    char house[15];
			    char first[20];
			    char last[20];
			    int year;
		    	    int points;
		            int added = 0;
			    
			    //reads the file until it does not read five entries   
			    while ((read = fscanf(filePointer,"%s%s%d%d%s",first, last, &points, &year, house)) == 5)
			    { 
			            int i = 0;
				    
 				    //loops through trees looking for the corrcet tree to add the student too
				    while(i < 4)
				    {
			    	    	if(strcmp(house, HOUSE_NAMES[i]) == 0)
						if((check(house, first, last, year, houses[i], command, 0)))
						{
						  	houses[i] = insert(houses[i], first, last, i, year, points);
							added = 1;
						}
				    		i++;
				    }
			            if(added == 0)
					printf("add failed on student %s %s. Invalid house: %s\n", first, last, house);
				
			    }
			    
			    fclose(filePointer);	//closes the file
			}
			else
				fprintf(stderr, "Load failed. Invalid file: <filename>\n");
		}
		else if(strncmp(command, "save", 4) == 0)
		{
			char filepath[75];
			scanf("%s", filepath);
			
			FILE* filePointer;	//file pointer
			filePointer = fopen(filepath, "w");	//opens a file for writting
			
			//if the file is created
			if(filePointer)
			{
				int i = 0;
				while(i < 4)
				{
					fpreorder(filePointer, houses[i]);	//prints the houses tree to a file in a preorder traversal
					i++;
				}
				
			}
				
			fclose(filePointer);	//closes the file
				
		}
		else if(strncmp(command, "inorder", 7) == 0)	//prints the houses in an inorder traversal
		{
			int i = 0;
			while(i < 5)
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
				inorder(houses[i]);
				i++;
			}
			putchar('\n');
						
		}
		else if(strncmp(command, "preorder", 8) == 0)	//prints the houses in an preorder traversal
		{
			int i = 0;
			while(i < 5)
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
		       		preorder(houses[i]);
				i++;
			}
			putchar('\n');
			
		}
		else if(strncmp(command, "postorder", 9) == 0)	//prints the houses in an postorder traversal
		{
			int i = 0;
			while(i < 5)
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
		        	postorder(houses[i]);
				i++;
			}
			putchar('\n');
		}
		else if(strncmp(command, "add", 3) == 0)	//adds the students specified by the user to the appropriate tree
		{
			int numberRead = 0;
			char house[15];
			char first[20];
			char last[20];
			int year;
			int points, boolean;
			
			numberRead = scanf("%s%s%d%d%s",first, last, &points, &year, house);

			if(strcmp(house, HOUSE_NAMES[0]) == 0)
			{
					if(check(house, first, last, year, houses[0], command, 1))
					{
						houses[0] = insert(houses[0], first, last, 0, year, points);
						printf("Added %s %s to roster.", first, last);
					}
			}
			else if(strcmp(house, HOUSE_NAMES[1]) == 0)
		{
				if(check(house, first, last, year, houses[1], command, 1))
					{
						houses[1] = insert(houses[1], first, last, 1, year, points);
						printf("Added %s %s to roster.", first, last);
					}
		}
			else if(strcmp(house, HOUSE_NAMES[2]) == 0)
		{
				if(check(house, first, last, year, houses[2], command, 1))
					{
						houses[2] = insert(houses[2], first, last, 2, year, points);
						printf("Added %s %s to roster.", first, last);
					}
		}		
			else if(strcmp(house, HOUSE_NAMES[3]) == 0)
			{
				if(check(house, first, last, year, houses[3], command, 1))
					{
						houses[3] = insert(houses[3], first, last, 3, year, points);	
						printf("Added %s %s to roster.", first, last);
					}
			}
			else
				printf("add failed. Invalid house: %s\n", house);
				
		}
		else if(strncmp(command, "kill", 4) == 0)	//removes student from house tree and to deceased tree
		{
			char house[15];
			char first[20];
			char last[20];
			Student* temp;
			scanf("%s%s%s", first, last, house);

			if(strcmp(house, HOUSE_NAMES[0]) == 0)
				{
					temp = search(houses[0], first, last);
					kill(houses[0], first, last);
					insert(houses[4], temp->first, temp->last, 0, temp->year, temp->points);
				}
			else if(strcmp(house, HOUSE_NAMES[1]) == 0)
				{
					temp = search(houses[1], first, last);
					kill(houses[1], first, last);
					insert(houses[4], temp->first, temp->last, 1, temp->year, temp->points);
				}
			else if(strcmp(house, HOUSE_NAMES[2]) == 0)
				{
					temp = search(houses[2], first, last);
					kill(houses[2], first, last);
					insert(houses[4], temp->first, temp->last, 2, temp->year, temp->points);
				}		
			else if(strcmp(house, HOUSE_NAMES[3]) == 0)
				{
					temp = search(houses[3], first, last);
					kill(houses[3], first, last);
					insert(houses[4], temp->first, temp->last, 3, temp->year, temp->points);
				}
			
		}
		else if(strncmp(command, "find", 4) == 0)	//searches for student
		{
      char house[15];
			char first[20];
			char last[20];
			Student* temp;
			
			scanf("%s%s%s", first, last, house);
			
		    if(strcmp(house, HOUSE_NAMES[0]) == 0)
		    {
		        if(temp = search(houses[0], first, last))
			        printStudent(temp);
			    else
			        fprintf(stderr,"Find failed. %s %s was not found in %s House\n", first, last, house);
		    }
		    else if(strcmp(house, HOUSE_NAMES[1]) == 0)
			{
			    if(temp = search(houses[1], first, last))
			    	printStudent(temp);
			    else
			        fprintf(stderr,"Find failed. %s %s was not found in %s House\n", first, last, house);
			}
			else if(strcmp(house, HOUSE_NAMES[2]) == 0)
			{
			    if(temp = search(houses[2], first, last))
			    	printStudent(temp);
			    else
			        fprintf(stderr,"Find failed. %s %s was not found in %s House\n", first, last, house);
			}
			else if(strcmp(house, HOUSE_NAMES[3]) == 0)
			{
			   
			    if(temp = search(houses[3], first, last))
			    	printStudent(temp);
			    else
			        fprintf(stderr,"Find failed. %s %s was not found in %s House\n", first, last, house);
			}
			else
			{
			    fprintf(stderr, "Find failed. Invalid house: <house>\n");
			}
		}

		else if(strncmp(command, "points", 4) == 0)	//adds specified amount of points to student
		{
			char first[20]; 
			char last[20];
			char house[20];
			int num;

			scanf("%s%s%s%d", first, last, house, &num);
				if(strcmp(house, HOUSE_NAMES[0]) == 0)
		   		points(houses[0], first, last, num, house);
		    else if(strcmp(house, HOUSE_NAMES[1]) == 0)
					points(houses[1], first, last, num, house);
				else if(strcmp(house, HOUSE_NAMES[2]) == 0)
					points(houses[2], first, last, num, house);
				else if(strcmp(house, HOUSE_NAMES[3]) == 0)
					points(houses[3], first, last, num, house);
				else
			    printf("Point change failed. Invalid house: %s\n", house);
			

			
		}
		else if(strncmp(command, "score", 4) == 0)	//displays the current scores of the houses
		{
			int i, sum = 0;
			int* sumPoint;
			sumPoint = &sum;
			char* houseName[4] = {"Gryffindor", "Ravenclaw", "Hufflepuff", "Slytherin"};
			printf("Point totals:\n\n");
			for(i = 0 ; i < 4 ; i++)
				{
					score(houses[i], sumPoint);
					printf("%s House:\t%d\n",houseName[i], sum);
					sum = 0;
				}
		}
		else
			fprintf(stderr, "Unknown command: <command>\n");
		
		printf("Enter command: ");
		scanf("%s", command);
	}
	
	//clears out each of the house trees
	clear(houses[0]);
	clear(houses[1]);
	clear(houses[2]);
	clear(houses[3]);

	return 0;
}

/* 
       Description: inserts the student to the specifed house tree
					
       Parameters:  root	- the root of the tree
		    first	- character array holding the first name
		    last 	- character array holding the last name
		    house	- integer value representing the house the student is in
		    year	- integer value representing the year the student is in
		    points	- integer value representing the amount of points the student has
					
       Returns:	    a student pointer to the the root
*/
Student* insert(Student* root, char first[], char last[], int house, int year, int points)
{
    if(root == NULL)
	{
		root = (Student*)malloc(sizeof(Student));
		root->first = strdup(first);
		root->last = strdup(last);
		root->house = house;
		root->year = year;
		root->points = points;
		
		root->left = NULL;
		root->right = NULL;
	}
	else if(strcmp(last, root->last) < 0)
		root->left = insert(root->left, first, last, house, year, points);
	else if(strcmp(last, root->last) > 0)
		root->right = insert(root->right, first, last, house, year, points);
	else if(strcmp(first, root->first) < 0)
		root->left = insert(root->left, first, last, house, year, points);
	else if(strcmp(first, root->first) > 0)
		root->right = insert(root->right, first, last, house, year, points);
	
		
	return root;
}

/* 
       Description: prints the students inorder
					
       Parameters:  root	- the root of the tree
					
*/
void inorder(Student* root)
{
	if(root != NULL)
	{
		inorder(root->left);
		printStudent(root);
		inorder(root->right);
	}
}

/* 
       Description: prints the students in a tree in a preorder traversal to a file
					
       Parameters:  root	- the root of the tree
					
*/
void fpreorder(FILE* file, Student* root)
{
	if(root != NULL)
	{
		fprintf(file, "%s %s %d %d %s\n", root->first, root->last, root->points, root->year, HOUSE_NAMES[root->house]);
		fpreorder(file, root->left);
		fpreorder(file, root->right);
	}
}

/* 
       Description: prints the students in a preorder traversal
					
       Parameters:  root	- the root of the tree
					
*/
void preorder(Student* root)
{
    if(root != NULL)
    {
        printStudent(root);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(Student* root) //post order traversal
{
    if(root != NULL)
    {
        preorder(root->left);
        preorder(root->right);
        printStudent(root);
    }
}


void printStudent(Student* student //prints out the student info
{
	if(student != NULL)
	{
	    char name[25];
	    strcpy(name, student->first);
	    strcat(name, " ");
	    strcat(name, student->last);
		
		printf("%-25s Points: %-d\t Year: %-d House: %-s\n", name, student->points, student->year, HOUSE_NAMES[student->house]);
		
	}
}

void clear(Student* root) //clears the student
{
	if(root != NULL)
	{
		clear(root->left);
		clear(root->right);
		free(root->first);
		free(root->last);
		free(root);
	}	
}

int check(char house[], char first[], char last[], int year, Student* root, char command[], int prt) //checks the basic error cases
{
	if(search(root, first, last) != NULL)
	{
		if(prt == 1)
			printf("%s failed. Student named %s %s already present in roster.\n", command, first, last);
		return 0;
	}

	if(year > 7 || year < 1)
	{
		if(prt == 1)
			printf("%s failed. Invalid year: %d\n", command, year);
		return 0;
	}

	if(strcmp(house, "Gryffindor") != 0 && strcmp(house, "Ravenclaw") != 0 && strcmp(house, "Hufflepuff") != 0 && strcmp(house, "Slytherin") != 0)
	{
		if(prt == 1)
			printf("%s failed. Invalid house: %s\n", command, house);
		return 0;
	}

	return 1;
}

Student* search (Student* root, char first[], char last[]) //searches and returns the location of the student you searched for
{
	
    if(root != NULL)
	{
		if(strcmp(last, root->last) == 0 && strcmp(first, root->first) == 0)
		{						
			return root;
		}
		else if(strcmp(last, root->last) < 0)
			return search(root->left, first, last);
		else if(strcmp(last, root->last) > 0)
			return search(root->right, first, last);
		else if(strcmp(first, root->first) < 0)
			return search(root->left, first, last);
		else if(strcmp(first, root->first) > 0)
			return search(root->right, first, last);
	}

	return NULL;
}

void points(Student* root, char first[], char last[], int num, char house[]) //finds the student entered and adjusts their point total by the amount entered
{
	if(search(root, first, last) != NULL)
		search(root, first, last)->points += num;
	else
		printf("Point change failed. %s %s was not found in %s House", first, last, house);
}

void score(Student* root, int* sum) //finds the total score of each house
{
	if(root != NULL)
	{
		score(root->left, sum);
		*sum += root->points;
		score(root->right, sum);
	}
}

Student* kill(Student* root, char first[], char last[])
{
		Student* dead = search(root, first, last);
		Student* parent = parentOf(root, first, last);

		if(dead->left == NULL && dead->right == NULL)
		{
			printf("no child\n");
			if(parent->left == dead) //if the node to be deleted is the left child of parent
			{
					free(dead->first);
					free(dead->last);
					free(dead);
					
					parent->left = NULL;
			}
			else if(parent->right == dead) //if the node to be deleted is the right child of parent
			{
					free(dead->first);
					free(dead->last);
					free(dead);
					
					parent->right = NULL;
			}
		}
		if(dead->left != NULL && dead->right == NULL) //if the node to be deleted only has a left child
			if(parent->left == dead)
			{
					parent->left = dead->left;
					free(dead->first);
					free(dead->last);
					free(dead);
					
					parent->left = NULL;
			}
			else if(parent->right == dead) //if the node to be deleted is the right is the child
			{
					parent->right = dead->left;
					free(dead->first);
					free(dead->last);
					free(dead);
					
					parent->right = NULL;
			}
		if(dead->left == NULL && dead->right != NULL) //if the node to be deleted has a right child
			if(parent->left == dead)
			{
					parent->left = dead->right;
					free(dead->first);
					free(dead->last);
					free(dead);
					
					parent->left = NULL;
			}
			else if(parent->right == dead) //if the right node of parent is the one to be deleted
			{
					parent->right = dead->right;
					free(dead->first);
					free(dead->last);
					free(dead);
					
					parent->right = NULL;
			}
		if(dead->left != NULL && dead->right != NULL //if the node to be deleted has a left child
			if(parent->left == dead)
			{
					parent->left = leftMostRight(dead);
					parent->left->right = dead->right;
					parent->left->left = dead->left;
					printf("%s\n",parent->left->left->last);
					dead = NULL;
					printf("%s\n", first); //delete
					printf("%s\n", last); //delete
					
					free(dead);
					
					parent->left = NULL;
			}
			else if(parent->right == dead) //if the right node of parent is the one to be deleted
			{
					parent->right = leftMostRight(dead);
					parent->right->right = dead->right;
					parent->right->left = dead->left;
					dead->left = NULL;
					dead->right = NULL;
					dead = NULL;
					
					free(dead);
					
					parent->right = NULL;
			}


			return root; //return the new root
				
}

Student* parentOf(Student* root, char first[], char last[]) //find the parent of the node entered
{
	
    if(root != NULL)
			{
				if(root->left != NULL)
					if(strcmp(last, root->left->last) == 0 && strcmp(first, root->left->first) == 0)				
					{
						return root; //return the parent
					}
				if(root->right != NULL)
					if(strcmp(last, root->right->last) == 0 && strcmp(first, root->right->first) == 0)
					{
						return root //return the parent
					}

						else if(strcmp(last, root->last) < 0)
							return parentOf(root->left, first, last);
						else if(strcmp(last, root->last) > 0)
							return parentOf(root->right, first, last);
						else if(strcmp(first, root->first) < 0)
							return parentOf(root->left, first, last);
						else if(strcmp(first, root->first) > 0)
							return parentOf(root->right, first, last);
			}
		return NULL;
}


Student* leftMostRight(Student* root) //return the left most node in the right subtree
{
	if(root->right != NULL)
		return leftMostRight(root->right);
	if(root->left != NULL)
		return leftMostRight(root->left);
	return root;
}
