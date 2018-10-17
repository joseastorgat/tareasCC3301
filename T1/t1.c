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
	int largo=0; 
	int largo_pal = 0;
	int apariciones =0;

	while(*pal!=0){
		pal++;
		largo_pal +=1;
	}

	pal-=largo_pal;
	while(*s!=0){
		if(*s==c){apariciones++;}
		largo +=1;
		s++;
	}

	s-=largo;
	char *res = malloc(sizeof(char)*(largo+apariciones*(largo_pal-1)));
	// printf("%s\n", s);
	// printf("%s\n", pal);
	// printf("%d\n", apariciones);
	while(*s!=0){
		if(*s==c){
			int j;
			// printf("%s\n", "hi");
			for(j=0; j<largo_pal; j++){
				*res = *pal;
				res++;
				pal++;
			}
			pal-=largo_pal;
			res-=1;
			;}
		else{
			*res = *s;
		}
		s++;
		res++;
	}
	res-=(largo+apariciones*(largo_pal-1));
	s-=largo;
	printf("%s\n",res );
	return res;
}

void reemplazar(char *s, char c, char *pal){
	return;
}
// void main(){
// 	char *src= "hola que tal";
//     char *res= reemplazo(src, 'a', "xyz");
//     printf("%s\n",res );
// }