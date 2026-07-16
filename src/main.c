#include <stdio.h>
#include <stdlib.h>
int main()
{
	unsigned char memory[4096] = { 0 };
	unsigned char V[16] = { 0 };
	unsigned int I = { 0 };
	unsigned char display[64 * 32] = { 0 };
	unsigned int stack[16] = { 0 };
	int stackPointer = 0;
	unsigned char delayTimer = 0;
	unsigned char soundTimer = 0;
	V[0xF] = 0; 
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
	int lastpc = -1;
	int pc = 0x200; 
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
			int x = (opcode & 0x0F00) >> 8;
			V[x] = opcode & 0x00FF;
			printf("Set V[%d] to %02X\n", x, opcode & 0x00FF);
			pc += 2;
		}
		else if (firstDigit == 0x7)
		{
			int x = (opcode & 0x0F00) >> 8;
			V[x] = (opcode & 0x00FF) + V[x];         
			printf("Add %02x to V[%d],result: %02x\n", opcode & 0x00FF, x, V[x]);
			pc = pc + 2;
		}
		else if (firstDigit == 0xA)
		{
			int addr = (opcode & 0x0FFF);
			I = addr;
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
		else if (firstDigit == 0x3)
		{
			int x = (opcode & 0x0F00) >> 8;
			int addr = (opcode & 0x00FF);
			if (V[x] == addr)
			{
				pc += 4;        
				printf("Skip next instruction because V[%d] == %02X\n", x, addr);
			}
			else
			{
				pc += 2;
			}
		}
		else if (firstDigit == 0x4)
		{
			int x = (opcode & 0x0F00) >> 8;
			int addr = (opcode & 0x00FF);
			if (V[x] != addr)
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
		}
		else if (firstDigit == 0x8 && (opcode & 0x00F) == 0)
		{
			int x = (opcode & 0x0F00) >> 8;
			int y = (opcode & 0x00F0) >> 4;
			V[x] = V[y];
			pc += 2;
			printf("Set V[%d] to V[%d],result: %02x\n", x, y, V[x]);
		}
		else if (firstDigit == 0xC)
		{
			int x = (opcode & 0x0F00) >> 8;
			int addr = (opcode & 0x00FF);
			int randomByte = rand() % 256;
			V[x] = randomByte & addr;
			printf("V[%d] populated with randomByte!! AND with %02x(NN)", x, addr);
			pc += 2;
		}
		else if (firstDigit == 0x2)
		{
			int addr = (opcode & 0x0FFF);
			printf("CALL: current pc=%03X, target addr=%03X, saving return=%03X\n", pc, addr, pc + 2);

			stack[stackPointer] = pc + 2;
			printf("Stack Pointer in call value: %d\n", stackPointer);
			++stackPointer;
			pc = addr;
		}

		else if (opcode == 0x00EE)
		{
			stackPointer = stackPointer - 1;
			printf("RETURN: current pc=%03X, return addr=%03X\n", pc, pc + 2);
			printf("Stack Pointer in RETURN value: %d\n", stackPointer);
			pc = stack[stackPointer];
			printf("Return from subroutine to address %03X\n", pc);
		}
		else if (firstDigit == 0xF && (opcode & 0x000F) == 0x07)
		{
			int x = (opcode & 0x0F00) >> 8;
			V[x] = delayTimer;
			pc = pc + 2;
			printf("Set V[%d] to delay timer value: %02x\n", x, V[x]);
		}
		else if (firstDigit == 0xF && (opcode & 0x00FF) == 0x15)
		{
			int x = (opcode & 0x0F00) >> 8;
			delayTimer = V[x];
			pc += 2;
			printf("Set delay timer to V[%d], value: %02x\n", x, delayTimer);
		}
		else if (firstDigit == 0xF && (opcode & 0x00FF) == 0x18)
		{
			int x = (opcode & 0x0F00) >> 8;
			soundTimer = V[x];
			pc += 2;
			printf("Set sound timer to V[%d], value: %02x\n", x, soundTimer);
		}
		else
		{
			pc += 2;
		}
	}

	for (int row = 0; row < 32;row++)
	{
		for (int col = 0; col < 64; col++)
		{
			int index = row * 64 + col;
			if (display[index] == 1)
			{
				printf("X");
			}
			else
			{
				printf(".");
			}


		}
		printf("\n");
	}

	return 0;
}