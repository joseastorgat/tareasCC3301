#include <stdio.h>
#include <stdlib.h>
#include "colapri.h"
#include "colapri-lista.h"

/*******************************************
 * Implementacion una Lista Enlazada
 *******************************************/
typedef struct node {
    void* x;
    struct node *next;
  } Node;


/* Cola de prioridades implementada con una lista enlazada */

/* Entrega una nueva cola de prioridades:
 * comp es el comparador de prioridades
 */
ColaPri nuevaColaPriLista(CompPri comp);

typedef struct {
  ColaPriOps *ops;
  CompPri comp;
  int nmax, n;
  Node *elems;
} ColaPriLista;

static void agregar(ColaPri colapri, void *a);
static void* extraer(ColaPri colapri);
static void* mejor(ColaPri colapri);
static int tamano(ColaPri colapri);
static void destruir(ColaPri colapri); /* debe estar vacia */
static IterColaP iterador(ColaPri colapri);

/*******************************************
 * Implementacion de la cola de prioridades
 *******************************************/
static ColaPriOps colapri_arr_ops= {
  agregar, extraer, mejor, tamano, destruir, iterador
};

ColaPri nuevaColaPriLista(CompPri comp) {
  ColaPriLista *cp= malloc(sizeof(ColaPriLista));
  cp->ops= &colapri_arr_ops;
  cp->comp= comp;
  cp->n= 0;
  cp->elems= NULL;
  return (ColaPri)cp;
}

static void agregar(ColaPri colapri, void *a) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
	Node *new_node = malloc(sizeof(Node));
	new_node->x = a;
	new_node->next=NULL;
	if(cp->n== 0){
		cp->elems = new_node;
		cp->n++;
		return;
	}
	cp->n++;
	Node *ultimo_elemento = cp->elems;
	Node *elementos_cola = cp->elems;
	
	//En el primer caso debemos arreglar la referencia :( a "mano"
	if((*cp->comp)(a, elementos_cola->x)<0){
		cp->elems = new_node;
		new_node->next = elementos_cola;
		return;
	}

	while(elementos_cola != NULL ){
		if((*cp->comp)(a, elementos_cola->x)<0){
			break;
		}
		ultimo_elemento = elementos_cola;
		elementos_cola = elementos_cola->next;
	}

	new_node->next = elementos_cola;
	ultimo_elemento->next = new_node;
}

static void* mejor(ColaPri colapri) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
	return cp->elems->x;
}

static int tamano(ColaPri colapri) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
    return cp->n;
}

static void* extraer(ColaPri colapri) {
    ColaPriLista *cp= (ColaPriLista*)colapri;
    Node *primero = cp->elems;
    cp->elems = primero->next;
    cp->n--;
	return primero->x;

}
static void destruir(ColaPri colapri) {
}


/*******************************************
 * Implementacion de Iterador
 *******************************************/

typedef struct {
  IterColaPOps *ops;
  ColaPriLista *cp;
  Node *nodo_actual;
  int k;
} IterColaLista;

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

static int continua(IterColaP itercp) {
	IterColaLista *iter= (IterColaLista*) itercp;
	return iter->k < iter->cp->n;
}

static void *proximo(IterColaP itercp) {
	IterColaLista *iter= (IterColaLista*) itercp;
	iter->k++;
	void *x = iter->nodo_actual->x;
	iter->nodo_actual = iter->nodo_actual->next;
	return x;
}

static void destruirIter(IterColaP iter) {
	free(iter);
}
