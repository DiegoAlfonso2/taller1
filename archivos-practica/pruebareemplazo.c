#include <stdio.h>
#include <stdlib.h>

typedef struct lista {
	struct lista *anterior;
	long pos;
} lista_t;

void liberar(lista_t* lista) {
	while (lista) {
		lista_t* tmp = lista->anterior;
		free(lista);
		lista = tmp;
	}
}

int main(int argc, char* argv[]) {
	FILE* arch = fopen("prueba.txt", "r+b");
	if (!arch) return 1;

	char c;
	long delta = 0;
	lista_t* ultimo = NULL;
	while ((c = fgetc(arch)) != EOF) {
		if (c == 'A') {
			lista_t* actual = malloc(sizeof(lista_t));
			if (!actual) {
				liberar(ultimo);
				fclose(arch);
				return 1;
			}
			actual->anterior = ultimo;
			actual->pos = ftell(arch);
			ultimo = actual;
			delta += 2;
		}
	}

	fseek(arch, 0, SEEK_END);
	long r_pos = ftell(arch) - 1;
	long w_pos = r_pos + delta;
	while (r_pos >= 0) {
		fseek(arch, r_pos, SEEK_SET);
		char leido = fgetc(arch);
		if (leido == 'A') {
			fseek(arch, w_pos - 2, SEEK_SET);
			fputs("ORO", arch);
			w_pos -= 3;
		} else {
			fseek(arch, w_pos, SEEK_SET);
			fputc(leido, arch);
			w_pos--;
		}
		r_pos--;
	}

	liberar(ultimo);
	fclose(arch);
}