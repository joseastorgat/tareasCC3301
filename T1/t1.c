#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t1.h"

uint borrar_bits(uint x, uint pat, int len){
	uint mascara;
	uint res = x;
	int i;

	// len == 32? mascara = -1 :  (1 << len) -1;
	if(len == 32){
		mascara = -1;
	}
	else{
		mascara = (1<<len)-1;
	}

	for(i=0; i<32; i++){
		uint mask = (mascara << i);  // movemos la mascara   000..011..110...0
		uint cap = (x & mask)>> i;   // extraemos la seccionde x que coincida con mas
		if(cap == pat){
			res = res & ( ~ mask );  // Para eliminar se niega mas  1111...100...001...1
		}
	}
	printf("%x\n", res);
	return res;
}


char *reemplazo(char *s, char c, char *pal){
	return 0;

}


void reemplazar(char *s, char c, char *pal){
	return;
}
