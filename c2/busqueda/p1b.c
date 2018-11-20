#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "p1.h"

int cmp_dic( void *a, int k, void *x){
  char *p = a;
  FILE *f = x;
  char buff[strlen(p)+1];
  fseek(f,(long int) k * 80, SEEK_SET);
  fread(buff, 1, strlen(p)+1, f);
  char *aux = buff;
  buff[strlen(p)+1] = 0;
  printf("%s\n",buff );

  while( *p  ){
    if(*aux < *p )
      return -1;
    else if(*aux > *p )
      return 1;
    p++;
    aux++;
    }

  printf("%s\n", aux);
  if(*aux==':'){
    return 0;
  }

  return 1;
}



int consultar(char *nom_dic, char *pal, int n_lin){
  FILE *f;
  f = fopen (nom_dic, "r");
  int i = buscar((void *) pal, n_lin, (void *) f, cmp_dic);
  fclose(f);
  return i;
}
