#include "Agua.h"
#include "Constantes.h"
#include <list>

Agua::Agua(const std::list<Coordenada>& coordenadas) : Poligono(coordenadas) {
}

bool Agua::superficieEdificada(){
	return false;
}

bool Agua::superficieArbolada(){
	return false;
}

const char Agua::caracter(){
	return '-';
}

size_t Agua::nivel(){
	return CAPA_UNO;
}
