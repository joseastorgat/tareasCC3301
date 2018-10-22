#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t1.h"

/* borrar bits*/
uint borrar_bits(uint x, uint pat, int len){
    uint mascara; 
    uint res = x; // respuesta
    int i;  // iterador
    
    if(len == 32) mascara = -1; // mascara con len 1's ->  00....01..11
    else mascara = (1<<len)-1;  
    
    for(i=0; i<32-len; i++){            // iteramos moviendo la mascara para encontrar coincidencias de pat en x
        if((x & mascara)>> i == pat){   // corrobora si seccion de x coinicide con el patron pat
            res = res & ( ~ mascara );  // Para eliminar pat de x negamos la mascara y operamos and con res ( x )
        }
        mascara<<=1;  // movemos la mascara 1 a la izquierda <-  000..011...110...0 
    }
    return res;
}


/* reemplazo */
char *reemplazo(char *s, char c, char *pal){
    int largo=0;          // variable para contar el largo de string original ( *s )
    int largo_pal = 0;    // variable para contar el largo de la string  ( *pal ) por la cual se reemplazara 
    int apariciones =0;   // variable para contar las aparicicones de c en *s

    while(*pal!=0){    // largo de string *pal
        pal++;
        largo_pal +=1;
    }

    pal-=largo_pal; //regresamos pal al inicio del string

    while(*s!=0){ // contar apariciones de c en *s y largo de *s
        if(*s==c){apariciones++;}
        largo +=1;
        s++;
    }

    s-=largo;
    char *res = malloc(sizeof(char)*(largo+apariciones*(largo_pal-1)));  // solicitamos memoria para entregar el resultado ( calculando el largo final del resultado)

    while(*s!=0){   // iteracion sobre *s
        if(*s==c){  // aparición de c 
            int j;
            for(j=0; j<largo_pal; j++){ // se inserta *pal en *res 
                *res = *pal;
                res++;
                pal++;
            }
            pal-=largo_pal;
            res-=1;
        }
        else{
            *res = *s; // se copia caracter de *s a *res
        }
        s++;
        res++;
    }
    res-=(largo+apariciones*(largo_pal-1)); //  regresa res al inicio del string
    s-=largo;                               // regresa al inicio del string
    return res;
}

/* reemplazar */
void reemplazar(char *s, char c, char *pal){
    int largo=0;         // variable para contar el largo de string original ( *s )
    int largo_pal = 0;   // variable para contar el largo de la string  ( *pal ) por la cual se reemplazara 
    int apariciones =0;  // variable para contar las aparicicones de c en *s

    while(*pal!=0){      // contar largo de *pal
        pal++;
        largo_pal +=1;
    }
    pal-=largo_pal;
    
    while(*s!=0){       //contar largo de *s y apariciones de c en *s
        if(*s==c){apariciones++;}
        largo +=1;
        s++;
    }

    if(largo_pal > 1){                               // si el largo_pal>1 se itera desde el final del string
        char *aux = s + (largo_pal-1)*apariciones;   // se necesita un puntero auxiliar para iterar
                                                     // s apunta al final de la string y aux al final de la nueva string (con reemplazos)
        
        int i; //iterador
        for(i = 0; i < largo; ++i)   //iterar sobre el largo original de *s 
        {
            if(*s==c){           //si aparece c insertamos *pal
                int j;      
                pal+=largo_pal-1; //itera pal desde el final

                for(j=0; j < largo_pal; j++){ //se inserta *pal desde el final
                    *aux = *pal;
                    aux--;
                    pal--;
                }
                aux++;    
                pal++;  
            }
            else{ *aux= *s;} // si no aparece s movemos char de pos s a aux
            s--;      
            aux--; 
        }
    }

    else{               // si largo es 0 o 1
        s-=largo;       // s apunta al principio de s*
        int i;
        for (i = 0; i < largo; i++)
        {
            if(*s == c){    // aparición de s
                if (largo_pal>0)  //si pal no es caracter vacio
                {
                    *s = *pal;    //se cambia  *s por *pal
                }
                else{           //si es caracter vacio
                    int j;    
                    char *aux = s+1;  //puntero auxiliar para reubicar los caracteres
                    for(j=i; j<largo; j++){ //adelantamos en un espacio todos los caracteres desde la posicion que estamos ubicados
                        *s = *aux; 
                        s++;
                        aux++;
                    }
                    s-=largo-i;
                }
            }
            s++;
        }   
    }


    return;
}
