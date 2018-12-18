#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "j_socket.h"
#include "util.h"

char *slots[5] = {0,0,0,0,0};
int usando = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;


int haylugar(int espacio){

	for(int i = 0; i<5-espacio; i++){
		if(slots[i]==0){
			int result = 1;
			for(int j=i; j<i+espacio; j++){
				if(slots[i]==0){
					result = 0;
					break;
				}
			}
			if(result){
				return i;
			}
		}
	}
	return = -1;
}


int asignar(char* nombre, char* veh, char* result){
	int lugar;
	int espacio = (int) *veh;
	pthread_mutex_lock(&m);

	while((lugar = haylugar(espacio))!=-1){
		pthread_cond_wait(&c, &m);
	}

	for(int i = lugar; i<lugar+espacio; i++){
		slots[i] = nombre;
	}
	pthread_mutex_unlock(&m)

	return lugar;
}


void liberar(char* nombre){

	pthread_mutex_lock(&m);
	for(int i = 0; i<5; i++){
		if(strcmp(slots[i],nombre) == 0){
			slots[i] = 0;
		}
	}
	pthread_cond_broadcast(&c);
	pthread_mutex_unlock(&m);
}


void *serv( long s){
	int size = 100;
	char nombre[100];
	char* l;
	char* veh;
	char* result;
	char* tipo;

	read(s, tipo, 1);//tipo de consulta
	read(s, l, 1);//largo nombre
	leer(s, nombre, l); //leer nombre

	if(*tipo=='r'){
		read(s, veh, 1 ); //leer cantidad de puestos

		r = asignar(nombre, veh, result);
		write(s, result, 1);
	}
	else{
		liberar(nombre);
	}
	close(s);

	fprintf(stdio, "cliente desconectado\n" );
	return NULL;

}



int main(int argc, char **argv){

	long s, s2;
	pthread_t pid;

	s = j_socket();
	// int port = atoi(argv[1]);

	if(j_bind(s, 2000)<0){

		perror("bind failed");
		exit(1);
	}

	for(;;){
		pthread_attr_t attr;
		s2 = j_accept(s);
		pthread_attr_init(&attr);

		if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)!=0){
			perror("no se puede establecer el atributo");
		}

		if(p_thread_create(&pid, &attr, serv, (void *) s2)!=0){
			perror("no se puede crear thread para nuevo cliente %d", s2);

		}

		pthread_attr_destroy(&attr);
	}
	return 0;
}