#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jsocket.h"
#include "util.h"

/* variables globales */
char *slots[5] = {0,0,0,0,0};  //slots para estacionar
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; //mutex
pthread_cond_t c = PTHREAD_COND_INITIALIZER;   //condicion
typedef void *(*Thread_fun)( void *); 

//funcion para imprimir espacios disponibles
void imprimir_disponibles(){
	pthread_mutex_lock(&m); // lock para dataraces
	for(int i = 0; i<5; i++){ //iteramos sobre slots
		if(slots[i]==0){    // si esta disponible imprime numero slot
			printf("%d",i);
		}
		else{               // sino imprime *
			printf("*"); 
			}
	}
	printf("\n");
	pthread_mutex_unlock(&m); // unlock
}


//funcion para checkear si hay espacio suficiente para estacionar un veh especifico
// recibe: espacio que ocupa el vehiculo
// entrega: posicion del primer slot disponible donde pueda estacionar
int haylugar(int espacio){

	// iterando sobre slots.
	for(int i = 0; i<5-espacio+1; i++){
		
		if(slots[i]==0){  //slot i esta vacio, ahora comprobar que veh cae en los siguientes slots
			int result = 1;

			for(int j=i; j<i+espacio; j++){  
				if(slots[j]!=0){   //notar que no habra segfault debigo a que j siempre será menor a 4
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


// funcion para asignar un estacionamiento
// retorna int el primer slot desde donde quedo estacionado el auto
int asignar(char* nombre, char* veh){
	int lugar; //resultado
	int espacio = (int) *veh; //tipo de vehiculo

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

// funcion para liberar los lugares ocupados por nombre
void liberar(char* nombre){
	char* rmnombre; 
	
	pthread_mutex_lock(&m);// lock para evitar dataraces
	
	for(int i = 0; i<5; i++){ //buscamos a la persona
		if(slots[i]!=0){
			if(strcmp(slots[i],nombre) == 0){ //desocupamos el slot
				rmnombre = slots[i];
				slots[i] = 0;
			}
		}
	}

	free(rmnombre); //liberar memoria ocupada por el nombre
	pthread_cond_broadcast(&c);// avisamos a quien este esperando su lugar
	pthread_mutex_unlock(&m);
}


void *serv( long s){
	char nombre[100];
	char l;
	char veh;
	// char* result;
	char tipo;
	
	// fprintf(stderr, "cliente conectado\n");

	read(s, &tipo, 1);//tipo de consulta
	read(s, &l, 1);//largo nombre
	leer(s, nombre, l); //leer nombre

	if(tipo == 'r'){
		read(s, &veh, 1); //leer cantidad de puestos
		char result = asignar(nombre, &veh);
		write(s, &result, 1);
		imprimir_disponibles();

	}
	else if(tipo=='l'){
		liberar(nombre);
		imprimir_disponibles();
	}

	else{
		char result = -1;
		write(s, &result, 1);

	}
	close(s);

	// fprintf(stderr, "cliente desconectado\n" );
	return NULL;

}

int main(int argc, char **argv){
	long s, s2;
	pthread_t pid;
	s = j_socket();
	
	int port = atoi(argv[1]);//puerto a utilizar

	if(j_bind(s, port)<0){
		perror("bind failed");
		exit(1);
	}

	imprimir_disponibles();

	for(;;){
		pthread_attr_t attr;
		s2 = j_accept(s); // aceptar conexion

		// crear threads auto joinables
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