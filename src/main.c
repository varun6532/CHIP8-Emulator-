#include <stdio.h>
#include <stdlib.h>
int main()
{
	unsigned char memory[4096];

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

	for (int i = 0x200; i < size + 0x200; i=i+2)
	{
		int w = i + 1;
		int opcode = memory[i] << 8 | memory[w];
		printf("Values: %04x\n", opcode);
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