#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "p1.h"

int buscar(void *a, int n, void *x, Comparador cmp){
  int left = 0;
  int right = n;
  
  while( left <= right){
      int i = (right + left)/2;
      int r = (*cmp)(a, i, x );
      printf("%i\n", i);
      if (r==0){
        return i;
      }
      else if(r<0){
        left=i+1;
      }
      else{
        right=i-1;
      }
  }
  return -1;
}