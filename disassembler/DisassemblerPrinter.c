/* DisassemblerPrinter.c
 * Author: Dickson Wong
 * Last Updated: October 18, 2017
 * Prints the instructions in a readable format given a hex code file of 8080
 * instructions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#define INSTRUCTION_LENGTH 20
#define NUMBER_OF_INSTRUCTIONS (0xff - 0x00 + 1)

/* gets the next byte represented in two hex characters following the pointer
 * fp and writes it to next_byte; if two characters do not exist, return -1; 
 * otherwise return 1 */
int get_byte(FILE *fp, char *next_byte)
{
	char ch;
	int num_chars = 0;
	 
	/* gets two non-whitespace/newline characters in file fp */
	while((ch = fgetc(fp)) != EOF)
	{
		if ((ch != ' ') && (ch != '\n')) 
		{
			next_byte[num_chars] = ch;
			num_chars++;
			
			/* Return the string once we have enough characters */
			if (num_chars == 2) 
			{
				return 0;
			}
		}
	}
	return -1;
}

/* prints the instructions in a readable format in the file fp */
void print_instructions(FILE *fp)
{	
	/* decode the instructions while there are still bytes left to read; 
	 * assumes the last byte in file has no data to read (i.e. the size is 1) 
	 */
	char *next_byte = malloc(3);
	int num_rep;
	
	if (next_byte == NULL) 
	{
		fprintf(stderr, "print_instructions: malloc failed\n");
		return;
	}
	
	next_byte[2] = '\0';
	
	while (get_byte(fp, next_byte) != -1)
	{
		num_rep = (int)strtol(next_byte, NULL, 16);
		printf("%d\n", num_rep);
		/*
		switch(next_byte)
		{
			case "00":
				printf("NOP\n");
				break;
			default:
				printf("DEFAULT CASE - CASES LEFT TO IMPLEMENT\n");
				break;
		}
		*/
	}
	free(next_byte);
}


int main(int argc, char **argv)
{
	FILE *fp;
	
	// open the file provided in the current folder
	if ((fp = fopen(argv[1], "r")) != NULL) 
	{
		print_instructions(fp);
		fclose(fp);
		return 0;
	}
	return -1;
}
			
