#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "j_socket.h"
#include "util.h"



int main(int argc, char **argv){


	char *tipo  =  argv[2];
	char *nombre =  argv[1];
	


	char l;

	long s;
	s = j_socket();

	if(j_connect(s, "localhost", 2000)<0){
		fprintf(stderr, "connection refused");
		exit(1);
	}

	write(s, tipo, 1);  //l o r
	l = strlen(nombre) + 1;
	write(s, &l, 1); // largo nombre 
	write(s, nombre, l); // nombre

	if(*tipo == 'r'){
		char *veh =  atoi(argv[3]);		
		write(s, veh, 1); //tipo vehiculo
	}

	read(s , &l, 1);

	puts(l);

	close(s);

	return 0;

}