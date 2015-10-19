  /*******************************/
   /*
   /*  Project Name: Project 2: One Time, One Time
   /*  Description: Code up a One Time Pad with tiling and chaining.
   /*  File names: project2.c
   /*  Date: 2/13/15
   /*  Authors: Ryan Schubert & Jared Fellenbaum
   /*
   /*******************************/

#include <stdio.h>

int bits(unsigned char temp);
unsigned char rotate(unsigned char temp, int count);

int main()
{

	unsigned char message[2048];
	unsigned char key[2048];
	unsigned char cipher[2048];

	int i = 0;
	int mCount = 0;
	int	kCount = 0;
	int temp;
	temp = getchar();
	
//input message
/* we go through the message and store the first 2048 characters (with legal values) until we hit 2048 characters, character value 255, or the end of the file. */
	for(i = 0 ; i < 2048 && temp != 255 && temp != EOF ; i++)
	{	
		if(temp > 0 && temp < 128)
		{
			message[i] = temp;	//store the characters in the array
			mCount++;
		}
			temp = getchar();
			
	}
	/*if the message is more than 2048 characters, we throw out the rest before
	we begin the input of the key.*/
if(i == 2048)
	while(temp != 255 && temp != EOF)
		temp = getchar();
	
	temp = getchar(); //clear the delimiter

//input key
/*allocating storage for messages and keys up to 2048 chars 
 we ignore chars past 2048 and chacters wihout legal values until we get EOF.
*/

	for(i = 0 ; i < 2048 && temp != EOF ; i++)
	{
		if(temp > 0 && temp < 128)
		{
			key[i] = temp; //store the characters in the array
			kCount++;
		}
			temp = getchar();
	}

//tiling
/*If key is too short repeat process until equal with message length 
*If key is longer than message length use characters from the key 
*We compute a form of the Vigenère cipher through this process
*short key phrase is repeated over and over to encrypt the message.
*/

	if(mCount != kCount)
	{
		int diff = mCount - kCount;
			
			if(diff > 0)
				{
					int i = 0;
					while(diff != 0)
					{
						key[kCount] = key[i];		//if the key is too short, repeat it until it is the right length
						i++;
						diff--;
						kCount++;
					}
				}
			
			else
					kCount = mCount; //otherwise ignore the extra characters in the key
		}				

//chaining
/* Takes  the ith character of the key keyi. Iterates through the whole key in order doing the following: keyi = rotate( keyi XOR keysum , bits( keyi - 1 )) */

	int sum = key[kCount - 1] % kCount;

	key[0] = rotate((key[0] ^ key[sum]), bits(key[kCount - 1])); //do the algorithim for the first one
	
	sum += key[0];
	sum %= kCount;

	for(i = 1 ; i < kCount ; i++)
		{
			key[i] = rotate((key[i] ^ key[sum]), bits(key[i - 1])); //loop throuh and chain the rest
			sum += key[i];
			sum %= kCount;
		}

//output
/*run through the two arrays
*XOR each pair of characters together 
*output the result
*/
		for(i = 0 ; i < mCount ; i++)
			putchar(message[i] ^ key[i]); //output the encrypted message

}
//rotate
/*with an unsigned char character we rotate the 7 least significant digits
 *it is a right rotation by int count
 */
unsigned char rotate(unsigned char character, int count)
{		
		int temp = 1;
		int i;
		for(i = 0 ; i < count ; i++) 
			{
				if(character & temp == 1)
				{
					character = character >> 1; //loop through and rotate 'count' number of times
					character = character | 64;
				}
				else
					character = character >> 1;
			}
		return character;

}
//bits
/*counts the number of 1 bits in binary of the character
 */
int bits(unsigned char temp)
{
	int barryWittmanHatesWyclefJean = 0;
	while(temp != 0) //loops through and counts the number of set bits in the character
	{
		if(temp & 1 == 1)
			barryWittmanHatesWyclefJean++;
		temp >>= 1;
	}
return barryWittmanHatesWyclefJean; //I noticed the filename of the picture on the project page
}



