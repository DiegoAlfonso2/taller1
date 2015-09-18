#include "common_Socket.h"
#include <unistd.h>
#include <stdlib.h>

#define MAX_CONEXIONES 20
#define MAX_BUFFER 2

Socket::Socket (std::string ip, std::string puerto, int flags) {
	struct addrinfo hints;
	struct addrinfo *posibilidades, *iterador;
	this->cerrado = false;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; /* IP v4*/
	hints.ai_socktype = SOCK_STREAM; /* Protocolo TCP */
	hints.ai_flags = flags;
	int resultado = getaddrinfo(ip.c_str(), puerto.c_str(), &hints, &posibilidades);
	if (resultado != 0){
		std::cerr << "ERROR EN ADDRINFO: " << gai_strerror(resultado) << std::endl;
		return;
	}
	iterador = posibilidades;
	bool direccionValida = false;
	while (iterador != NULL && !direccionValida){
		int skt = socket(iterador->ai_family, iterador->ai_socktype, iterador->ai_protocol);
		if (skt != -1){
			// Anduvo, nos podemos conectar a esta direccion
			this->socketFD = skt;
			memcpy(& this->ai_addr, iterador->ai_addr, sizeof(struct sockaddr));
			this->ai_addrlen = iterador->ai_addrlen;
			direccionValida = true;
		}
		iterador = iterador->ai_next;
	}
	if (!direccionValida){
		std::cerr << "NO SE OBTUVIERON DIRECCIONES VALIDAS" << std::endl;
	}
	freeaddrinfo(posibilidades);
}

Socket::Socket(int nuevoSocketFD){
	this->socketFD = nuevoSocketFD;
	memset(&this->ai_addrlen, 0, sizeof(socklen_t));
	this->cerrado = false;
}

bool Socket::bindSocket(){
	int resultado = bind(this->socketFD, &this->ai_addr, this->ai_addrlen);
	if (resultado != 0){
		std::cerr << "ERROR AL BINDEAR SOCKET: " << gai_strerror(resultado) << std::endl;
		return false;
	}
	return true;
}

bool Socket::conectar(){
	int resultado = connect(this->socketFD, &this->ai_addr, this->ai_addrlen);
	if (resultado != 0){
		std::cerr << "ERROR AL CONECTAR: " << gai_strerror(resultado) << std::endl;
		close(this->socketFD);
		return false;
	}
	return true;
}

bool Socket::listenSocket(){
	int resultado = listen(this->socketFD, MAX_CONEXIONES);
	if (resultado != 0){
		std::cerr << "ERROR AL ESCUCHAR SOCKET: " << gai_strerror(resultado) << std::endl;
		return false;
	}
	return true;
}

Socket* Socket::aceptar(){
	int nuevoSocketFD = accept(this->socketFD, &this->ai_addr, &this->ai_addrlen);
	if (nuevoSocketFD == -1){
		if (!cerrado){
			std::cerr << "ERROR AL ACEPTAR CONEXION. " << gai_strerror(nuevoSocketFD) << std::endl;
		}
		return NULL;
	}
	return new Socket(nuevoSocketFD);
}

bool Socket::enviar(const char *buffer, ssize_t tamanio){
	ssize_t bytesEnviados = 0;
	bool error = false, socketCerrado = false;
	while (bytesEnviados < tamanio && !error && !socketCerrado){
		ssize_t envioParcial = send(this->socketFD, buffer + bytesEnviados, tamanio - bytesEnviados, MSG_NOSIGNAL);
		if (envioParcial < 0){
			std::cerr << "Error al enviar mensaje: " << std::string(buffer) << std::endl;
			std::cerr << "Error: " << gai_strerror((int) envioParcial) << std::endl;
			error = true;
		} else if (envioParcial == 0){
			std::cerr << "SOCKET CERRADO DESDE EL OTRO PUNTO. " << std::endl;
			socketCerrado = true;
		} else {
			bytesEnviados += envioParcial;
		}
	}
	if (error || socketCerrado){
		this->cerrar();
		close(this->socketFD);
		return false;
	}
	return true;
}

bool Socket::enviar(std::string mensaje){
	return this->enviar(mensaje.c_str(), mensaje.length());
}

bool Socket::recibir(std::string &mensaje){
	bool error = false, socketCerrado = false;
	char buffer[MAX_BUFFER];
	ssize_t recibidoParcial = recv(this->socketFD, buffer, MAX_BUFFER, MSG_NOSIGNAL);
	if (recibidoParcial < 0 && !cerrado){
		std::cerr << "Error al recibir mensaje: " << gai_strerror((int)recibidoParcial) << std::endl;
		error = true;
	} else if (recibidoParcial == 0){
		socketCerrado = true;
	} else {
		buffer[recibidoParcial] = '\0';
		mensaje += std::string(buffer);
	}
	if (error || socketCerrado){
		this->cerrar();
		close(this->socketFD);
		// Si me cierran el socket, no lo considero error.
		return socketCerrado;
	}
	return true;
}

bool Socket::cerrar(){
	this->cerrado = true;
	shutdown(this->socketFD, SHUT_RDWR);
	return true;
}


Socket::~Socket() {
	close(this->socketFD);
}

