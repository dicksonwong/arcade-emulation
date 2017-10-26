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
			case 0x31:
				printf("LXI    SP, #$%02x%02x",next_instr[2],next_instr[1]);
				break;
			case 0x32:
				printf("STA    $%02x%02x",next_instr[2],next_instr[1]);
				break;
			case 0x33:
				printf("INX    SP");
				break;
			case 0x34:
				printf("INR    M");
				break;
			case 0x35:
				printf("DCR    M");
				break;
			case 0x36:
				printf("MVI    M,#$%02x", next_instr[1]);
				break;
			case 0x37:
				printf("STC");
				break;
			case 0x38:
				printf("NOP");
				break;
			case 0x39:
				printf("DAD   SP");
				break;
			case 0x3A:
				printf("LDA    $%02x%02x",next_instr[2],next_instr[1]);
			case 0x3B:
				printf("DCX    SP");
				break;
			case 0x3C:
				printf("INR    A");
				break;
			case 0x3D:
				printf("DCR    A");
				break;
			case 0x3E:
				printf("MVI    A,#$%02x", next_instr[1]);
				break;
			case 0x3F:
				printf("CMC");
				break;
			case 0x40:
				printf("MOV    B,B");
				break;
			case 0x41:
				printf("MOV    B,C");
				break;
			case 0x42:
				printf("MOV    B,D");
				break;
			case 0x43:
				printf("MOV    B,E");
				break;
			case 0x44:
				printf("MOV    B,H");
				break;
			case 0x45:
				printf("MOV    B,L");
				break;
			case 0x46:
				printf("MOV    B,M");
				break;
			case 0x47:
				printf("MOV    B,A");
				break;
			case 0x48:
				printf("MOV    C,B");
				break;
			case 0x49:
				printf("MOV    C,C");
				break;
			case 0x4A:
				printf("MOV    C,D");
				break;
			case 0x4B:
				printf("MOV    C,E");
				break;
			case 0x4C:
				printf("MOV    C,H");
				break;
			case 0x4D:
				printf("MOV    C,L");
				break;
			case 0x4E:
				printf("MOV    C,M");
				break;
			case 0x4F:
				printf("MOV    C,A");
				break;
			case 0x50:
				printf("MOV    D,B");
				break;
			case 0x51:
				printf("MOV    D,C");
				break;
			case 0x52:
				printf("MOV    D,D");
				break;
			case 0x53:
				printf("MOV    D,E");
				break;
			case 0x54:
				printf("MOV    D,H");
				break;
			case 0x55:
				printf("MOV    D,L");
				break;
			case 0x56:
				printf("MOV    D,M");
				break;
			case 0x57:
				printf("MOV    D,A");
				break;
			case 0x58:
				printf("MOV    E,B");
				break;
			case 0x59:
				printf("MOV    E,C");
				break;
			case 0x5A:
				printf("MOV    E,D");
				break;
			case 0x5B:
				printf("MOV    E,E");
				break;
			case 0x5C:
				printf("MOV    E,H");
				break;
			case 0x5D:
				printf("MOV    E,L");
				break;
			case 0x5E:
				printf("MOV    E,M");
				break;
			case 0x5F:
				printf("MOV    E,A");
				break;
			case 0x60:
				printf("MOV    H,B");
				break;
			case 0x61:
				printf("MOV    H,C");
				break;
			case 0x62:
				printf("MOV    H,D");
				break;
			case 0x63:
				printf("MOV    H,E");
				break;
			case 0x64:
				printf("MOV    H,H");
				break;
			case 0x65:
				printf("MOV    H,L");
				break;
			case 0x66:
				printf("MOV    H,M");
				break;
			case 0x67:
				printf("MOV    H,A");
				break;
			case 0x68:
				printf("MOV    L,B");
				break;
			case 0x69:
				printf("MOV    L,C");
				break;
			case 0x6A:
				printf("MOV    L,D");
				break;
			case 0x6B:
				printf("MOV    L,E");
				break;
			case 0x6C:
				printf("MOV    L,H");
				break;
			case 0x6D:
				printf("MOV    L,L");
				break;
			case 0x6E:
				printf("MOV    L,M");
				break;
			case 0x6F:
				printf("MOV    L,A");
				break;
			case 0x70:
				printf("MOV    M,B");
				break;
			case 0x71:
				printf("MOV    M,C");
				break;
			case 0x72:
				printf("MOV    M,D");
				break;
			case 0x73:
				printf("MOV    M,E");
				break;
			case 0x74:
				printf("MOV    M,H");
				break;
			case 0x75:
				printf("MOV    M,L");
				break;
			case 0x76:
				printf("HLT");
				break;
			case 0x77:
				printf("MOV    M,A");
				break;
			case 0x78:
				printf("MOV    A,B");
				break;
			case 0x79:
				printf("MOV    A,C");
				break;
			case 0x7A:
				printf("MOV    A,D");
				break;
			case 0x7B:
				printf("MOV    A,E");
				break;
			case 0x7C:
				printf("MOV    A,H");
				break;
			case 0x7D:
				printf("MOV    A,L");
				break;
			case 0x7E:
				printf("MOV    A,M");
				break;
			case 0x7F:
				printf("MOV    A,A");
				break;
			case 0x80:
				printf("ADD    B");
				break;
			case 0x81:
				printf("ADD    C");
				break;
			case 0x82:
				printf("ADD    D");
				break;
			case 0x83:
				printf("ADD    E");
				break;
			case 0x84:
				printf("ADD    H");
				break;
			case 0x85:
				printf("ADD    L");
				break;
			case 0x86:
				printf("ADD    M");
				break;
			case 0x87:
				printf("ADD    A");
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
			
