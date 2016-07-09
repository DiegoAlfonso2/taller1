#include <stdio.h>

int main(int argc, char const *argv[])
{
	FILE* entrada;
	fprintf(stdout, "Abriendo archivo\n");
	entrada = fopen("prueba.txt", "r+b");
	char c;
	while ((c = fgetc(entrada)) != EOF) {
		printf("%c", c);
	}
	fclose(entrada);
	return 0;
}