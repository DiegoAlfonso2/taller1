#include <stdio.h>
#include <stdlib.h>

typedef struct lista {
	struct lista *anterior;
	long pos;
} lista_t;

void liberar(lista_t* lista) {
	lista_t* liberando = lista;
	while (liberando != NULL) {
		lista_t* otro = liberando->anterior;
		free(liberando);
		liberando = otro;
	}
}

int main(int argc, char* argv[]) {
	FILE* archivo = fopen("prueba.txt", "r+b");
	if (!archivo) {
		return 1;
	}
	char c;
	lista_t* ultimo = NULL;
	while ((c = fgetc(archivo)) != EOF) {
		if (c == 'A') {
			lista_t* actual = malloc(sizeof(lista_t));
			if (!actual) {
				fclose(archivo);
				liberar(ultimo);
				return 1;
			}
			actual->anterior = ultimo;
			actual->pos = ftell(archivo) - 1;
			ultimo = actual;
		}
	}
	for (lista_t* lista = ultimo; lista; lista = lista->anterior) {
		printf("Localizado en posicion %ld\n", lista->pos);
	}
	liberar(ultimo);
	fclose(archivo);
	return 0;
}