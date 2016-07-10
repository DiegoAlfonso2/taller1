#include <stdio.h>

int main(int argc, char const *argv[])
{
	FILE* entrada;
	fprintf(stdout, "Abriendo archivo\n");
	entrada = fopen("prueba.txt", "r+b");
	char c;
	long posiciones[300];
	int i = 0;
	printf("Posicion %d: %ld\n", -1, ftell(entrada));
	while ((c = fgetc(entrada)) != EOF) {
		printf("%c", c);
		posiciones[i] = ftell(entrada);
		++i;
	}
	for (int j = 0; j < i; ++j) {
		printf("Posicion %d: %ld\n", j, posiciones[j]);
	}
	fclose(entrada);
	return 0;
}