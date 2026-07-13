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
	for (int i = 0x200; i < 0x200 + 10; i++)
	{
		printf("%02X ", memory[i]);
	}
	printf("\n");
	return 0;

}