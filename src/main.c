#include <stdio.h>
#include <stdlib.h>
int main()
{
	unsigned char memory[4096];
	unsigned char V[16];
	unsigned int I = 0;
	unsigned char display[64 * 32]; 
	V[0xF] = 0; // Set the VF register to 0
	FILE* file = fopen("C:/Users/varun/OneDrive/Documents/chip8/roms/IBM Logo.ch8", "rb");
	if (file == NULL)
	{
		printf("Error opening file\n");
		return 1;
	}
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
	fread(&memory[0x200], 1, size, file);
	fclose(file);
	printf("Loaded %ld bytes into memory\n", size);
	int lastpc = -1;
	int pc = 0x200; // Program counter starts at 0x200
	while (pc < 0x200 + size)
	{
		int opcode = (memory[pc] << 8) | memory[pc + 1];
		int firstDigit = (opcode & 0xF000) >> 12;
		if (opcode == 0x00E0)
		{
			printf("Clear the display\n");
			pc += 2;
		}
		else if (firstDigit == 0x1)
		{
			printf("Opcode : %04X\n", opcode);
			printf("Jump to address %03X\n", opcode & 0x0FFF);
			if (pc == lastpc)
			{
				printf("Halting at address %03X\n", pc);
				break;

			}
			else
			{
				lastpc = pc;
			}

			pc = opcode & 0x0FFF;
		}
		else if (firstDigit == 0x6)
		{
			int x = opcode & 0x0F00 >> 8;
			V[x] = opcode & 0x00FF;
			printf("Set V[%d] to %02X\n", x, opcode & 0x00FF);
			pc += 2;
		}
		else if (firstDigit == 0x7)
		{
			int x = (opcode & 0x0F00) >> 8;
			V[x] = (opcode & 0x00FF) + V[x];           //V[x] = V[x] + NN basically 
			printf("Add %02x to V[%d],result: %02x\n", opcode & 0x00FF, x, V[x]);
			pc = pc + 2;
		}
		else if (firstDigit == 0xA)
		{
			int addr = (opcode & 0x0FFF);
			I = I + addr;
			printf("Set I to %03X\n", I);
			pc = pc + 2;


		}
		else if (firstDigit == 0xD)
		{
			printf("Draw sprite at V[%d], V[%d] with height %d\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4, opcode & 0x000F);	
			int x = (opcode & 0x0F00) >> 8;
			int y = (opcode & 0x00F0) >> 4;
			int n = opcode & 0x000F;
			for (int row = 0; row < n; row++)
			{
				unsigned char spriteByte = memory[I + row];
				for (int col = 0; col < 8; col++)
				{
					int bit = (spriteByte >> (7 - col)) & 1;
					if (bit)
					{
						int screenX = (V[x] + col);
						int screenY = (V[y] + row);
						int index = screenY * 64 + screenX;

						if (screenX < 64 && screenY < 32)
						{
							if (display[index] == 1)
							{
								V[0xF] = 1;
							}
							display[index] ^= 1;
						}
					}
				}
			}
			pc += 2;
		}
		else
		{
			pc += 2;
		}


	}

	int opcode = (memory[0x200] << 8) | memory[0x201];
	printf("Opcode: %d\n", opcode);

	for (int i = 0x200; i < 0x200 + 10; i++)
	{
		printf("%02x ", memory[i]);
	}
	printf("\n");
	return 0;
}