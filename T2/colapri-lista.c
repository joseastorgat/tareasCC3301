#include <stdio.h>
#include <stdlib.h>
#include "colapri.h"
#include "colapri-lista.h"

/*******************************************
 * Implementacion una Lista Enlazada
 *******************************************/
typedef struct node {
    void* valor;
    struct node *sig;
  } Nodo;


/****************************************************************
 * Implementacion de la cola de prioridades usando Lista Enlazada
 ****************************************************************/
typedef struct {
  ColaPriOps *ops;
  CompPri comp;
  int nmax, n;
  Nodo *elems;
} ColaPriLista;

/* Operaciones de la cola prioridad */
static void agregar(ColaPri colapri, void *a);
static void* extraer(ColaPri colapri);
static void* mejor(ColaPri colapri);
static int tamano(ColaPri colapri);
static void destruir(ColaPri colapri); /* debe estar vacia */
static IterColaP iterador(ColaPri colapri);

static ColaPriOps colapri_list_ops= {
  agregar, extraer, mejor, tamano, destruir, iterador
};

/* Entrega una nueva cola de prioridades:
 * comp es el comparador es la funciona que compara la prioridad de los elementos*/
ColaPri nuevaColaPriLista(CompPri comp) {
  ColaPriLista *cp= malloc(sizeof(ColaPriLista));
  cp->ops= &colapri_list_ops;
  cp->comp= comp;
  cp->n= 0;
  cp->elems= NULL;
  return (ColaPri)cp;
}

/*agregar un elemento a la cola, la posicion del elemento queda determinado por su prioridad
  respecto a la funcion de comparacion  */
static void agregar(ColaPri colapri, void *a) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
	//Nuevo nodo conteniendo el elemento a insertar
	Nodo *nuevo_nodo = malloc(sizeof(Nodo));
	nuevo_nodo->valor = a;
	nuevo_nodo->sig=NULL;

	//si la cola de prioridad se encuentra vacía, se inserta primer nodo
	if(cp->n== 0){
		cp->elems = nuevo_nodo;
		cp->n++;
		return;
	}
	cp->n++;
	Nodo *ultimo_elemento = cp->elems;
	Nodo *elementos_cola = cp->elems;

	// si el nodo a ingresar ocupa el tope de la cola
	if((*cp->comp)(a, elementos_cola->valor)<0){
		cp->elems = nuevo_nodo;
		nuevo_nodo->sig = elementos_cola;
		return;
	}

	// otros Casos: se utiliza un ciclo while recorriendo la cola para determinar la posición del nuevo elemento
	while(elementos_cola != NULL ){
		if((*cp->comp)(a, elementos_cola->valor)<0){
			break;
		}
		ultimo_elemento = elementos_cola;
		elementos_cola = elementos_cola->sig;
	}

	nuevo_nodo->sig = elementos_cola;
	ultimo_elemento->sig = nuevo_nodo;
}

/* entregar elemento con mejor prioridad, sin extraerlo de la cola */
static void* mejor(ColaPri colapri) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
	return cp->elems->valor;
}

/* retoran cantidad de elementos actuales de la cola de prioridad */
static int tamano(ColaPri colapri) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
    return cp->n;
}

/* extrae y retorna el elemento con mejor prioridad */
static void* extraer(ColaPri colapri) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
    Nodo *primero = cp->elems;
    cp->elems = primero->sig;
    cp->n--;
	return primero->valor;

}

/*destrucción de cola de prioridad*/
static void destruir(ColaPri colapri) {
	ColaPriLista *cp= (ColaPriLista*) colapri;
	// si es que la lista no está vacía, liberamos los nodos restantes
	if (cp->n>0){
		Nodo *elemento; 
		while (cp->elems!=NULL){
			elemento = cp->elems;
			cp->elems = cp->elems->sig;
			free(elemento);
		}
	}
	// liberamos la cola
	free(cp);
}


/*******************************************
 * Implementacion de Iterador pa ColaPriLista
 *******************************************/

typedef struct {
  IterColaPOps *ops;
  ColaPriLista *cp;
  Nodo *nodo_actual;
  int k;
} IterColaLista;

/* Operaciones del Iterador*/
static int continua(IterColaP iter);
static void* proximo(IterColaP iter);
static void destruirIter(IterColaP iter);

static IterColaPOps iterOps= { continua, proximo, destruirIter };

IterColaP iterador(ColaPri colapri) {
  IterColaLista *iter= malloc(sizeof(IterColaLista));
  iter->ops= &iterOps;
  iter->cp = (ColaPriLista*)colapri;
  iter->nodo_actual = iter->cp->elems;
  iter->k =0;
  return (IterColaP)iter;
}

/* True si quedan elementos en el iterador 
   False si no */
static int continua(IterColaP itercp) {
	IterColaLista *iter= (IterColaLista*) itercp;
	return iter->k < iter->cp->n;
}

/*
  Retorna el próximo elemento en el iterador, y mueve el iterador en un elemento
*/
static void *proximo(IterColaP itercp) {
	IterColaLista *iter= (IterColaLista*) itercp;
	iter->k++;
	void *x = iter->nodo_actual->valor;
	iter->nodo_actual = iter->nodo_actual->sig;
	return x;
}

/* destrucción del iterador 
( solo el iterador no los nodos ni la cola ) */
static void destruirIter(IterColaP iter) {
	free(iter);
}
