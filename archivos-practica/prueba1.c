#include <stdio.h>

int main(int argc, char const *argv[])
{
	FILE* entrada;
	fprintf(stdout, "Abriendo archivo\n");
	entrada = fopen("prueba.txt", "r+t");
	fclose(entrada);
	return 0;
}