   /*******************************/
   /*
   /*  Project Name: Project 6: You Got Served
   /*  Description: Use socket programming to create a web server
   /*  File names: server.c, makefile
   /*  Date: Wednesday, April 29, 2015, 3:00pm
	 /*  Due by: Friday, May 1, 2015 at 11:59pm
   /*  Author: Ryan Schubert
   /*
   /*******************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char* getType(char* path);
void sendFile(char* path, char* hData, char* fBuffer, int c1, FILE* file, struct stat buf);
void fileNotFound(char* path, char* FNF);

int main( int argc, char** argv )
{
	int i,j = 0;

	if(argc != 3) //if there are not three arguments, print error and quit
		{
			printf("Usage: ./server <port> <path to root>\n");	
			return 0;
		}

	int port = atoi(argv[1]); //get the port
	const char* dir = argv[2]; //get the directory

	if(chdir(dir) == -1) //if we can't change to the directory, print error and quit
		{
			printf("Could not change to directory: %s\n", dir);
			return 0;
		}

	int s1 = 0;
	int c1 = 0;
	int optval = 1;
	char ip[15] = "127.0.0.1"; //local IP

	struct sockaddr_in address;
	struct sockaddr_storage otherAddress; //initialize socket structs
	socklen_t otherSize = sizeof(otherAddress);

	s1 = socket(AF_INET, SOCK_STREAM, 0); //get socket

	setsockopt(s1, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); //make the socket reusable
	
	memset(&address, 0, sizeof(address));					//bind the socket to the local IP and port
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	bind(s1, (struct sockaddr*)&address, sizeof(address));

	listen(s1, 1); //listen
	while(1) //contint accepting new connections
{
	c1 = accept(s1, (struct sockaddr*) &otherAddress, &otherSize); //accept

	printf("Got a client: \n");

	char req[4];
	char path[100];
	char http[10];
	char reqBuffer[200];
	char fBuffer[1024];
	struct stat buf;

	for(i = 0 ; i < 99 ; ++i) //initialize path as spaces to get length
		path[i] = ' ';
	path[100] = '\0';

	i = 0;

	recv(c1, reqBuffer, 200, 0); //recieve the request from the connection
	
	sscanf(reqBuffer, "%s %s %s", req, path, http); //read in the first three strings

	while(path[i] != ' ') //get the length of path
		++i;

	int pLen = i;
	i = 0;
	int getOrHead = 0;

	if(strncmp(req, "GET", 3) == 0)
		getOrHead = 1;
	else if(strncmp(req, "HEAD", 4) == 0)
		getOrHead = 2;

	if(getOrHead == 1 || getOrHead == 2 ) //if GET or HEAD request
		{
			if(path[pLen] == '/') //if the file path ends with a slash (/), concatenate index.html onto the path.
				{
					strcat(path, "index.html");
					pLen += 10;
				}
			if(path[0] == '/') //if the file path starts with a slash (/), skip it.
				for(i = 1 ; i < 100 ; ++i)
					{
						path[i-1] = path[i];
						--pLen;
					}

			if(getOrHead == 1) //if it is a  GET request
				{
					printf("\t GET %s\n", path);
					FILE* file = fopen(path, "rb"); //open the file specified
					if(file != NULL) //if the file exists
					{
						char hData[40];

						sprintf(hData, "HTTP/1.0 200 OK\r\n"); //send the first line of header
						send(c1, hData, strlen(hData), 0);
						
						sendFile(path, hData, fBuffer, c1, file, buf); //send the rest of the header and the file
						printf("Sent file: %s\n", path);

					}
					else //if the file is not found
						{
							char FNF[1251 + strlen(path)];
							fileNotFound(path, FNF); //get the code for the 404 page

							char hData[40];

							sprintf(hData, "HTTP/1.0 404 Not Found\r\n"); //send header data
							send(c1, hData, strlen(hData), 0);
					
							sprintf(hData, "Content-Type: %s\r\n", "html");
							send(c1, hData, strlen(hData), 0);

							sprintf(hData, "Content-Length: %d\r\n\r\n", ((int)strlen(FNF)+1));
							send(c1, hData, strlen(hData), 0);

							send(c1, FNF, strlen(FNF), 0); //send 404 page
							printf("File not found: %s\n", path);
						}
				}
			else
				{
					char hData[40];
					sprintf(hData, "HTTP/1.0 200 OK\r\n"); //send the first line of header
					send(c1, hData, strlen(hData), 0);

					stat(path, &buf);
					int size1 = buf.st_size; //gets the size of the file

					sprintf(hData, "Content-Length: %d\r\n", size1); //sends header data
					send(c1, hData, strlen(hData), 0);

					sprintf(hData, "Content-Type: %s\r\n", getType(path)); 
					send(c1, hData, strlen(hData), 0);

					printf("Sent head: %s\n", path);
				}
		}
	}
}
/*
 * var: char* path, hData, fBuffer
 * var: int c1
 * var: FILE* file
 * var: struct stat buf
 * sends the rest of the header data and file
 */
void sendFile(char* path, char* hData, char* fBuffer, int c1, FILE* file, struct stat buf)
	{
		stat(path, &buf);
		int size1 = buf.st_size; //gets the size of the file

		sprintf(hData, "Content-Type: %s\r\n", getType(path)); //sends header data
		send(c1, hData, strlen(hData), 0);

		sprintf(hData, "Content-Length: %d\r\n\r\n", size1);
		send(c1, hData, strlen(hData), 0);
						
		int readIn = 0;
						
		while((readIn = fread(fBuffer, sizeof(char), 1024, file)) != 0) //sends the rest of the file
			send(c1, fBuffer, readIn, 0);
	}
/*
 * var: char* path
 * return: char*
 * returns the file type
 */
char* getType(char* path)
	{
		char temp[20];
		int i, j, len;

		len = strlen(path); //length of path

		for(i = len ; path[i-1] != '.' ; --i); //index of the file extension

		for(j = 0 ; i < len ; j++)
		{
			temp[j] = path[i]; //gets file extension
			i++;
		}
		
		if(strcmp(temp, "html") == 0) //returns the correct file type based on the extension
			return "text/html";
		else if(strcmp(temp, "htm") == 0)
			return "text/html";
		else if(strcmp(temp, "jpg") == 0)
			return "image/jpeg";
		else if(strcmp(temp, "jpeg") == 0)
			return "image/jpeg";
		else if(strcmp(temp, "gif") == 0)
			return "image/gif";
		else if(strcmp(temp, "png") == 0)
			return "image/png";
		else if(strcmp(temp, "txt") == 0)
			return "text/plain";
		else if(strcmp(temp, "c") == 0)
			return "text/plain";
		else if(strcmp(temp, "h") == 0)
			return "text/plain";
		else if(strcmp(temp, "pdf") == 0)
			return "application/pdf";
		else
			return "application/unknown"; //returns unknown if it isn't supported
	}
/*
 * var: char* path, FNF
 * concatenates the html code for the 404 page into a string, including the file that was not found
 */
void fileNotFound(char* path, char* FNF)
	{
		sprintf(FNF, "<!--A Design by W3layouts Author: W3layout Author URL: http://w3layouts.com License: Creative Commons Attribution 3.0 Unported License URL: http://creativecommons.org/licenses/by/3.0/ --> <!DOCTYPE HTML> <html> <head> <title>Free 404 for Website Template | Home :: w3layouts</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/> <link href=\"http://fonts.googleapis.com/css?family=Fjalla+One\" rel=\"stylesheet\" type=\"text/css\"> <style type=\"text/css\"> body{ background-color: #4DB3B3 ; } .wrap{ margin:0 auto; width:1000px; } .title{ margin-bottom: 40px; } .title h1{ font-size:100px; color: #E64A45; text-align:center; margin-top:100px; text-shadow:6px 1px 6px #333; font-family: 'Fjalla One', sans-serif; } .title h2{ font-size:100px; color: #E64A45; text-align:center; margin-bottom:1px; text-shadow:6px 1px 6px #333; font-family: 'Fjalla One', sans-serif; margin-top: -20px; } .title h3{ font-size:45px; color: #E64A45; text-align:center; margin-bottom:1px; text-shadow:6px 1px 6px #333; font-family: 'Fjalla One', sans-serif; margin-top: 75px; } </style> </head> <body> <div class=\"wrap\"> <div class=\"title\"> <h1>You've Failed.</h1> <h2>404 Error!</h2> <h3>\"%s\" does not exist!</h3> </div> </div> </div> </body>", path);
	}
