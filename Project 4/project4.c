  /*******************************/
   /*
   /*  Project Name: Project 4: Wave Goodbye
   /*  Description: Write a program that processes audio files based on user input.
   /*  File names: project4.c, audioFunctions.h, wave.h, wave.c, makefile
   /*  Date: Due by: Friday, March 27, 2015 at 11:59pm (1 Grace Day used)
   /*  Authors: Kegan Flagg & Ryan Schubert
   /*
   /*******************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "wave.h"
#include "audioFunctions.h"

int main(int argc, char **argv)
{
	int i,j;
	double num;
	WaveHeader	wave; //creates a WaveHeader
	int success = readHeader(&wave); //reads in the wave header
	checkFileErrors(wave); //checks for errors

	int size = wave.dataChunk.size; //gets the size
	short* left = (short*) malloc(sizeof(short)*(size/4)); //creates left and right arrays
	short* right = (short*) malloc(sizeof(short)*(size/4));
	if(left == NULL || right == NULL)
		fprintf(stderr, "Program out of memory\n"); // Call to malloc() failed (returned NULL)

	input(left, right, size); //reads in the rest of the file

	if(argc > 1)
		{
			for(i = 1 ; i < argc ; i++)
				{	
						if(strcmp(argv[i], "-r") == 0) //reverses if -r
							reverse(left, right, size/4);

						if(strcmp(argv[i], "-s") == 0) //changes speed if -s
							{
								num = atof(argv[++i]);
								if(num < 0)
									fprintf(stderr, "A positive number must be supplied for the speed change\n");
								
								left = speed(left, size/4, num);
								right = speed(right, size/4, num);
								size /= num;
								wave.dataChunk.size /= num; //updates size info
							}
						if(strcmp(argv[i], "-f") == 0) //flips channels if -f
								swapC(&left, &right);

						if(strcmp(argv[i], "-o") == 0) //fades out if -0
							{
								num = atof(argv[++i]);

								if(num < 0)
									fprintf(stderr, "A positive number must be supplied for the fade in and fade out time\n");
								fadeo(right, left, size/4, num);
							}

						if(strcmp(argv[i], "-i") == 0) //fades in if -i
							{
								num = atof(argv[++i]);

								if(num < 0)
									fprintf(stderr, "A positive number must be supplied for the fade in and fade out time\n");
								fadei(right, left, size/4, num);
							}
						if(strcmp(argv[i], "-v") == 0) //changes volume if -v
							{
								num = atof(argv[++i]);

								if(num < 0)
									fprintf(stderr, "A positive number must be supplied for the volume scale\n");
								volume(left, right, size/4, num);
							}

						if(strcmp(argv[i], "-e") == 0) //adds echo if -e
							{
								num = atof(argv[++i]);
								double scale = atof(argv[++i]);
								if(num < 0 || scale < 0)
									fprintf(stderr, "Positive numbers must be supplied for the echo delay and scale parameters\n");

								left = echo(left, size/4, num, scale);
								right = echo(right, size/4, num, scale);
								size += (int) (num* 44100); //updates size info
								wave.dataChunk.size += (int) (num * 44100);
							}
					
						}
		}

	//writes the new file constants
	output(left, right, size, &wave);

}
/*
 * var: short* left, right
 * var: int size
 * reads in the .wav file
 */
void input(short* left, short* right, int size)
{
	short temp, temp2; //temp variables
	int i;
		
	for(i = 0 ; i < size/4 ; i++) //loops through a quarter of the size of the file
		{
			temp2 = getchar();
			temp = getchar();
			temp <<= 8; //parses each character value into two shorts and stores in left array
			temp |= temp2;
			left[i] = temp;

			temp2 = getchar();
			temp = getchar();
			temp <<= 8;//parses each character value into two shorts and stores in right array
			temp |= temp2;
			right[i] = temp;
		}

	if(getchar() != EOF)
		fprintf(stderr, "File size does not match size in header\n"); //End of file reached before all the data was read

}

/*
 * var: short* left, right
 * var: int size
 * var: WaveHeader* wav
 * outputs the .wav file after any and all changes have been made
 */
void output(short* left, short* right, int size, WaveHeader* wave)
{
	writeHeader(wave); //writes the header
	
	int i;

	for(i = 0 ; i < size/4 ; i++) //loops through the arrays
	{
		//puts each set of shorts back into a char and then outputs them
		putchar(left[i] & 0xFF); 
		putchar(left[i] >> 8);
		putchar(right[i] & 0xFF);
		putchar(right[i] >> 8);
	}

}



/*
 * var: short* left, right
 * var: int size
 * var: double scale
 * Scale the volume of the file by a specified amount
 */
void volume(short* left, short* right, int size, double scale)
{
	int i;
	double value;
	
	for(i = 0 ; i < size/4 ; i++)
	{
		value = left[i] * scale;
			if(value > SHRT_MAX)
				value = SHRT_MAX; //sets values to short max or min if they go out of bounds
			if(value < SHRT_MIN)
				value = SHRT_MIN;
		left[i] = value; //updates the new value in the left array

		value = right[i] * scale;
			if(value > SHRT_MAX)
				value = SHRT_MAX; //sets values to short max or min if they go out of bounds
			if(value < SHRT_MIN)
				value = SHRT_MIN;
		right[i] = value; //updates the new value in the right array
	}
}

/*
 * var: short* left, right
 * var: int size
 * Reverse the audio
 */
void reverse(short* left, short* right, int size)
{
	int i, j = size - 1;
	short templ, tempr;
	for (i = 0; i < size/2; i++) //goes through and swaps all the values
	{
		templ = left[i];
		tempr = right[i];

		left[i] = left[j];
		right[i] = right[j];				
		
		left[j] = templ;
		right[j--] = tempr;
		
	}
}

/*
 * var: short* array
 * var: int size
 * var: double factor
 * return short*
 * Reverse the audio
 */
short* speed(short* array, int size, double factor)
{
	int newSize = size/factor;
	short* newArray =  (short*) malloc(sizeof(short)*newSize); //new array that is appropriate size
	int i = 0;
	
	for( i = 0; i < newSize; i++)
		newArray[i] = array[(int) (i * factor)]; //puts the new value in the new array

	free(array); //free's the old array
	return newArray; //returns new array
}

/*
 * var: short* right, left
 * var: int size
 * var: double time
 * Fade in from zero to full volume at the beginning of the file over a specified time
 */
void fadei(short* right, short* left, int size, double time)
{
	double n = 0.0, newt = time * 44100;
	int i = 0;
	while (n <= 1)
	{
		n = ((i)/newt) * ((i)/newt); //fades in from front by quadratic factor <= 1
		left[i] *= n;
		right[i++] *= n;		
	}

}

/*
 * var: short* right, left
 * var: int size
 * var: double time
 * Fade out from full volume to zero at the end of the file over a specified time
 */
void fadeo(short* right, short* left, int size, double time)
{

	double n = 0.0 , newt = time * 44100;
	int i = 0, j = size - 1;
	while (n <= 1)
	{
		n = ((i)/newt) * ((i++)/newt); //fades in from end by quadratic factor <= 1
		left[j] *= n;
		right[j--] *= n;	
	}

}


/*
 * var: short* right, left
 * Flip the left and the right stereo channels
 */
void swapC(short** left, short** right)
{
	short* temp = *left; //flips the values in the two arrays
	*left = *right;
	*right = temp;
}

/*
 * var: short* array
 * var: int size
 * var: double delay, scale
 * Add an echo to the sound with a specified delay and a specific scale factor
 * return: short*
 */
short* echo(short* array, int size, double delay, double scale)
{
	delay *=44100;
	short* darray =  (short*)malloc(sizeof(short)*(size+(int)delay));
	int i = 0, j = 0;	//j is location of echo

	while ( i < (int) delay) //copies until delay
		darray[i] = array[i++];

	while (i < size) //adds the echo to original
	{	
		darray[i] = array[i];
		darray[i++] += (short)(array[j++] * scale);
	}
	while ( i < (int)(size + delay)) //echoes until end
		darray[i++] = (short)(array[j++] * scale);
	

	free(array); //free old array
	return darray; //return new array
}

/*
 * WaveHeader wave
 * Checks for basic errors
 */
void checkFileErrors(WaveHeader wave)
{
	if(wave.ID[0] != 'R' || wave.ID[1] != 'I' || wave.ID[2] != 'F' || wave.ID[3] != 'F')
		fprintf(stderr, "File is not a RIFF file\n"); //Header ID does not contain "RIFF"

	if(wave.formatChunk.ID[0] != 'f' || wave.formatChunk.ID[1] != 'm' ||
			wave.formatChunk.ID[2] != 't' || wave.formatChunk.ID[3] != ' ' ||
			wave.formatChunk.size != 16 || wave.formatChunk.compression != 1)
		fprintf(stderr, "Format chunk is corrupted\n"); //Format chunk ID does not contain "fmt " or format size is not 16 or compression is not 1

	if(wave.dataChunk.ID[0] != 'd' || wave.dataChunk.ID[1] != 'a' || wave.dataChunk.ID	[2] != 't' || wave.dataChunk.ID[3] != 'a')
		fprintf(stderr, "Data chunk is corrupted\n"); //Data chunk ID does not contain "data"

	if(wave.formatChunk.channels != 2)
		fprintf(stderr, "File is not stereo\n"); //File does not have two channels

	if(wave.formatChunk.sampleRate != 44100)
		fprintf(stderr, "File does not use 44,100Hz sample rate\n"); //Sample rate is not 44,100

	if(wave.formatChunk.bitsPerSample != 16)
		fprintf(stderr, "File does not have 16-bit samples\n"); //Samples are not 16 bits
}
