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
	int pc = 0;
	int num_instructions = 0;
	//char arg;
	
	if (next_byte == NULL) 
	{
		fprintf(stderr, "print_instructions: malloc failed\n");
		return;
	}
	next_byte[INSTRUCTION_SIZE_IN_FILE - 1] = '\0';
	
	while ((num_data = get_instruction(fp, next_byte, next_instr)) != -1)
	{
		/* print the instruction number */
		/*
		printf("INSTRUCTION: %x ", next_instr[0]);
		*/
		
		/*
		/* prints the arguments to instruction if there are any */
		/*
		for (arg = 1; arg < num_data + 1; arg++)
		{
			printf("ARG%d: %x ", arg, next_instr[arg]);
		}
		printf("\n");
		*/
		
		/* print program counter */
		printf ("%04x ", pc);
		
		switch(next_instr[0])
		{
			/* prints the next instruction */
			case 0x00:
			case 0x08:
				printf("NOP");
				break;
			case 0x01:
				printf("LXI    B,#$%02x%02x", next_instr[2], next_instr[1]);
				break;
			case 0x02:
				printf("STAX   B");
				break;
			case 0x03:
				printf("INX    B");
				break;
			case 0x04:
				printf("INR    B");
				break;
			case 0x05:
				printf("DCR    B");
				break;
			case 0x06:
				printf("MVI    B,#$%02x", next_instr[1]);
				break;
			case 0x07:
				printf("RLC");
				break;
			case 0x09:
				printf("DAD    B");
				break;
			case 0x0A:
				printf("LDAX   B");
				break;
			case 0x0B:
				printf("DCX    B");
				break;
			case 0x0C:
				printf("INR    C");
				break;
			case 0x0D:
				printf("DCR    C");
				break;
			case 0x0E:
				printf("MVI    C,#$%02x", next_instr[1]);
				break;
			case 0x0F:
				printf("RRC");
				break;
			case 0x10:
				printf("NOP");
				break;
			case 0x11:
				printf("LXI    D,#$%02x%02x", next_instr[2], next_instr[1]);
				break;
			case 0x12:
				printf("STAX   D");
				break;
			case 0x13:
				printf("INX    D");
				break;
			case 0x14:
				printf("INR    D");
				break;
			case 0x15:
				printf("DCR    D");
				break;
			case 0x16:
				printf("MVI    D,#$%02x",next_instr[1]);
				break;
			case 0x17:
				printf("RAL");
				break;
			case 0x18:
				printf("NOP");
				break;
			case 0x19:
				printf("DAD    D");
				break;
			case 0x1A:
				printf("LDAX   D");
				break;
			case 0x1B:
				printf("DCX    D");
				break;
			case 0x1C:
				printf("INR    E");
				break;
			case 0x1D:
				printf("DCR    E");
				break;
			case 0x1E:
				printf("MVI    E,#$%02x",next_instr[1]);
				break;
			case 0x1F:
				printf("RAR");
				break;
			case 0x20:
				printf("RIM");
				break;
			case 0x21:
				printf("LXI    H,#$%02x%02x", next_instr[2], next_instr[1]);
				break;
			case 0x22:
				printf("SHLD   $%02x%02x",next_instr[2],next_instr[1]);
				break;
			case 0x23:
				printf("INX    H");
				break;
			case 0x24:
				printf("INR    H");
				break;
			case 0x25:
				printf("DCR    H");
				break;
			case 0x26:
				printf("MVI    H,#$%02x",next_instr[1]);
				break;
			case 0x27:
				printf("DAA");
				break;
			case 0x28:
				printf("NOP");
				break;
			case 0x29:
				printf("DAD    H");
				break;
			case 0x2A:
				printf("LHLD   $%02x%02x",next_instr[2],next_instr[1]);
				break;
			case 0x2B:
				printf("DCX    H");
				break;
			case 0x2C:
				printf("INR    L");
				break;
			case 0x2D:
				printf("DCR    L");
				break;
			case 0x2E:
				printf("MVI    L,#$%02x", next_instr[1]);
				break;
			case 0x2F:
				printf("CMA");
				break;
			case 0x30:
				printf("SIM");
				break;
			case 0xC3:
				printf("JMP ADR");
				break;
			case 0xC5:
				printf("PUSH   B");
				break;
			case 0xD5:
				printf("PUSH   D");
				break;
			case 0xE5:
				printf("PUSH   H");
				break;
			case 0xF5:
				printf("PUSH   PSW");
				break;
			default:
				printf("INSTRUCTION: %x ", next_instr[0]);
				break;
		}
		printf("\n");
		num_instructions++;
		pc += num_data + 1;
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
			
