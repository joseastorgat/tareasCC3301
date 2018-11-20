#include <stdio.h>
#include "integral.h"

/* Programe aca la funcion integral */

double integral(Funcion f, void *p, double xi, double xf, int n){
	double h = (xf-xi)/((double)n);
	double sum = 0.0;

	for(int i = 1; i<n ; i++){
		sum+= (*f)(p, xi + (i)*h);
	}
	return h * ( ((*f)(p, xi) + (*f)(p, xf))/2.0  + sum);
}