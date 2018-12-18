#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jsocket.h"
#include "util.h"

int main(int argc, char **argv){
	char tipo   =  *argv[2];
	char *nombre =  argv[1];
	char l, *conn, *server;
	long s;

	if(  !((tipo=='r' && argc ==4 ) || (tipo=='l' && argc==3))){
		fprintf(stderr, "Argumentos Invalidos \n" );
		exit(1);
	}

	if( (conn = getenv("PARKING") ) == 0 ){
		fprintf(stderr, "Variable PARKING con parametros de conexion NO definida\n");
		exit(1);
	}

	//parsing servidor:port de variable PARKING
	server = conn;
	while((*conn) != ':'){
		conn++;
		if(*conn == 0){
			fprintf(stderr, "Error en la definicion variable PARKING \n");
			exit(1);
		}
	}

	*conn = 0;
	conn++;

	int port = atoi(conn);
	
	//conexion	
	s = j_socket(); 
	if(j_connect(s, server, port)<0){
		fprintf(stderr, "connection refused");
		exit(1);
	}

	write(s, &tipo, 1);  // enviar tipo de accion
	l = strlen(nombre) + 1;
	write(s, &l, 1); // enviar largo del nombre 
	write(s, nombre, l); // enviar nombre

	if(tipo == 'r'){
		int veh	=  atoi(argv[3]);	
		if(veh<1 || veh>5){
			fprintf(stderr, "Tipo de vehiculo invalido!\n");
			exit(1);
		}
		write(s, &veh, 1); //enviar tipo vehiculo
		read(s , &l, 1);
		printf("%i \n", l );
	}

	close(s); //cierre conexion
	return 0;
}