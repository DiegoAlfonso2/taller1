#include <stdio.h>

int main(int argc, char const *argv[])
{
	FILE* entrada;
	fprintf(stdout, "Abriendo archivo\n");
	entrada = fopen("prueba.txt", "r+b");
	char c;
	fseek(entrada, 10, SEEK_END);
	printf("%ld\n", ftell(entrada));
	fputc(64, entrada);
	fclose(entrada);
	return 0;
}