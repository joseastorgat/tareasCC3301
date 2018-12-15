#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

typedef struct nodo {
  int x;
  struct nodo *prox;
} Nodo;

jmp_buf env;

/* agregue otras funciones como una rutina de atencion para SIGSEGV */

/* esta es la funcion pedida */

void capturador(){
	siglongjmp(env,1);
}

int largo(Nodo *nodo) {
  /* ... completar ... */
	int n =0;
	void (* hdlr)();
	hdlr =	signal(SIGSEGV, capturador);
	
	if(sigsetjmp(env,1)==0){
		while(nodo!=NULL){
			n++;
			nodo=nodo->prox;
		}
		//exito
		signal(SIGSEGV, hdlr);
		return n;
	}
	else{
		//segfault
		signal(SIGSEGV, hdlr);
		return -1;
	}
}



/* Ahora viene el test de prueba */

Nodo nodo3= { 3, NULL };
Nodo nodo2= { 2, &nodo3 };
Nodo nodo1= { 1, &nodo2 };

int main() {
  char dummy;
  printf("Largo= %d\n", largo(&nodo1));
  nodo3.prox= (Nodo*)(&dummy - 100000000);
  printf("Largo= %d\n", largo(&nodo1));
  return 0;
}
