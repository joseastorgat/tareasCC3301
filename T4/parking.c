#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jsocket.h"
#include "util.h"

char *slots[5] = {0,0,0,0,0};
int usando = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

typedef void *(*Thread_fun)( void *);


void print_disponibles(){
	pthread_mutex_lock(&m);
	for(int i = 0; i<5; i++){

	if(slots[i]==0){
		printf("%d",i);
	}
	else{
		printf("*");
		}
	}


	printf("\n");
	pthread_mutex_unlock(&m);
}

int haylugar(int espacio){
	for(int i = 0; i<5-espacio+1; i++){
		if(slots[i]==0){
			int result = 1;
			
			for(int j=i; j<i+espacio; j++){
				if(slots[i]!=0){
					result = 0;
					break;
				}
			}
			if(result==1){
				return i;
			}
		}
	}
	return -1;
}


int asignar(char* nombre, char* veh){
	int lugar;
	int espacio = (int) *veh;
	pthread_mutex_lock(&m);

	while((lugar = haylugar(espacio))==-1){
		pthread_cond_wait(&c, &m);
	}
	char *escrito = malloc(sizeof(char)*(strlen(nombre)+1));
	strcpy(escrito,nombre);
	for(int i = lugar; i<lugar+espacio; i++){
		slots[i] = escrito;
	}
	pthread_mutex_unlock(&m);

	return lugar;
}


void liberar(char* nombre){
	char* rmnombre;
	pthread_mutex_lock(&m);
	for(int i = 0; i<5; i++){
		if(slots[i]!=0){
			printf("%s\n",slots[i] );
			if(strcmp(slots[i],nombre) == 0){
				rmnombre = slots[i];
				slots[i] = 0;
		}
		}
	}
	free(rmnombre);
	pthread_cond_broadcast(&c);
	pthread_mutex_unlock(&m);
}


void *serv( long s){
	char nombre[100];
	char l;
	char veh;
	// char* result;
	char tipo;
	
	fprintf(stderr, "cliente conectado\n");

	read(s, &tipo, 1);//tipo de consulta
	read(s, &l, 1);//largo nombre
	leer(s, nombre, l); //leer nombre

	printf("%s\n",&tipo);
	if(tipo == 'r'){
		read(s, &veh, 1); //leer cantidad de puestos
		char result = asignar(nombre, &veh);
		write(s, &result, 1);
	}
	else{
		liberar(nombre);
	}
	print_disponibles();
	close(s);

	fprintf(stderr, "cliente desconectado\n" );
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

	print_disponibles();

	for(;;){
		pthread_attr_t attr;
		s2 = j_accept(s);
		pthread_attr_init(&attr);
				
		if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)!=0){
			perror("no se puede establecer el atributo");
		}

		if(pthread_create(&pid, &attr, (Thread_fun) serv, (void *) s2)!=0){
			perror("no se puede crear thread para nuevo cliente %d");

		}

		pthread_attr_destroy(&attr);
	}
	return 0;
}