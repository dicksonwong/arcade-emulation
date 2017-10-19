/* DisassemblerPrinter.c
 * Modified By: Dickson Wong
 * Last Updated: October 18, 2017
 * Prints the instructions in a readable format given a hex code file of 8080
 * instructions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#define INSTRUCTION_LENGTH 20
#define NUMBER_OF_INSTRUCTIONS (0xff - 0x00 + 1)
#define HEX_SIZE 16
#define INSTRUCTION_SIZE_IN_FILE 3
#define MAX_INSTRUCTION_SIZE 3

/* gets the next byte represented in two hex characters following the pointer
 * fp and writes it to next_byte; if two characters do not exist, return -1; 
 * otherwise return 0 */
int get_byte(FILE *fp, char *next_byte)
{
	char ch;
	int num_chars = 0;
	
	*next_byte = 0;
	
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

/* gets the next instruction in file fp, given an instruction buffer 
 * next_instr and 3-byte character buffer next_byte used to read bytes
 * in fp; returns the number of data arguments the instruction uses (which
 * will be stored in next_instr[1] and possibly next_instr[2] as well; returns
 * -1 otherwise */
int get_instruction(FILE *fp, char *next_byte, unsigned char *next_instr) 
{
	/* gets the next instruction and writes it to next_instr[0]; depending
	 * on the instruction size (8080), also gets data arguments and writes it
	 * to next_instr[1] and posible next_instr[2] */
	if (get_byte(fp, next_byte) != -1)
	{
		next_instr[0] = (unsigned char)strtol(next_byte, NULL, HEX_SIZE);
		
		/* Check if read data is necessary */
		switch (next_instr[0])
		{
			/* cases where instruction requires one more argument */
			case 0x06:
			case 0x0E:
			case 0x16:
			case 0x1E:
			case 0x26:
			case 0x2E:
			case 0x36:
			case 0x3E:
			case 0xC6:
			case 0xCE:
			case 0xD3:
			case 0xD6:
			case 0xDB:
			case 0xDE:
			case 0xE6:
			case 0xEE:
			case 0xF6:
			case 0xFE:
				get_byte(fp, next_byte);
				next_instr[1] = (unsigned char)strtol(next_byte, NULL, HEX_SIZE);
				return 1;
				
			/* cases where instruction requires two more arguments */
			case 0x01:
			case 0x11:
			case 0x21:
			case 0x22:
			case 0x2A:
			case 0x31:
			case 0x32:
			case 0x3A:
			case 0xC2:
			case 0xC3:
			case 0xC4:
			case 0xCA:
			case 0xCC:
			case 0xCD:
			case 0xD2:
			case 0xD4:
			case 0xDA:
			case 0xDC:
			case 0xE2:
			case 0xE4:
			case 0xEA:
			case 0xEC:
			case 0xF2:
			case 0xF4:
			case 0xFA:
			case 0xFC:
				for (int i = 1; i < 3; i++) 
				{
					get_byte(fp, next_byte);
					next_instr[i] = (unsigned char)strtol(next_byte, NULL, HEX_SIZE);
				}
				return 2;
		}
		return 0;
	}
	return -1;
}
			
	
/* prints the instructions in a readable format in the file fp */
void print_instructions(FILE *fp)
{	
	/* decode the instructions while there are still bytes left to read; 
	 * assumes the last byte in file has no data to read (i.e. the size is 1) 
	 */
	char *next_byte = malloc(INSTRUCTION_SIZE_IN_FILE);
	unsigned char *next_instr = malloc(MAX_INSTRUCTION_SIZE);
	int num_data;
	int num_ops;
	char arg;
	
	if (next_byte == NULL) 
	{
		fprintf(stderr, "print_instructions: malloc failed\n");
		return;
	}
	next_byte[INSTRUCTION_SIZE_IN_FILE - 1] = '\0';
	
	while ((num_data = get_instruction(fp, next_byte, next_instr)) != -1)
	{
		/* print the instruction number */
		printf("INSTRUCTION: %x ", next_instr[0]);
		
		/* prints the arguments to instruction if there are any */
		for (arg = 1; arg < num_data + 1; arg++)
		{
			printf("ARG%d: %x ", arg, next_instr[arg]);
		}
		
		/* print a newline */
		printf("\n");
		
		switch(next_instr[0])
		{
			case 0x00:
				printf("NOP\n");
				break;
			case 0xC3:
				printf("JMP ADR\n");
				break;
			default:
				printf("(NOT YET IMPLEMENTED)\n");
				break;
		}
		num_ops++;
	}
	free(next_instr);
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
			
