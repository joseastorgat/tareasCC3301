#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "reunion.h"

struct reunion {
	int asistentes, votos_concluir;
	pthread_mutex_t m;
	pthread_cond_t c;
};

Reunion *nuevaReunion(){
	Reunion *reunion = malloc(sizeof(Reunion));
	reunion->asistentes=reunion->votos_concluir=0;
	pthread_cond_init(&reunion->c, NULL);
	pthread_mutex_init(&reunion->m, NULL);
	return reunion;
}

void entrar(Reunion *r){
	pthread_mutex_lock(&r->m);
	r->asistentes++;
	pthread_mutex_unlock(&r->m);
}

void concluir(Reunion *r){
	pthread_mutex_lock(&r->m);
	r->votos_concluir++;
	if(r->asistentes==r->votos_concluir){
		pthread_cond_broadcast(&r->c);
	}
	else{
		while(r->votos_concluir!=r->asistentes){
			pthread_cond_wait(&r->c, &r->m);
		 }
    }
	pthread_mutex_unlock(&r->m);
}