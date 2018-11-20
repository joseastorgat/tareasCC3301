#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "integral.h"

/* Programe aca la funcion integral_g_dx_dy_par */

typedef struct args {
	double xi, xf, yi, yf;
	double res;
	int n, m;
} Args;

void* integral_g_dx_dy_par_aux(void *ptr){
	Args *args = (Args*) ptr;
	args->res = integral_g_dx_dy(args->xi, args->xf, args->n, args->yi, args->yf, args->m);
	return NULL;
}

double integral_g_dx_dy_par(double xi, double xf, int n, double yi, double yf, int m){
	int n_threads = 8;
	double d_threads = (double) n_threads;
	pthread_t p[n_threads];
	Args args[n_threads];

	double k = (yf-yi)/d_threads; 

	for(int i=0; i<n_threads; i++){
		Args *arg = &args[i];
		arg->xi = xi;
		arg->xf = xf;
		arg->n  = n;
		arg->yi = yi + k*i;
		arg->yf = yi + k*(i+1);
		arg->m  = m/d_threads;
		arg->res = 0;
		pthread_create( &p[i], NULL, integral_g_dx_dy_par_aux, arg);
	}

	double res = 0.0;

	for(int i=0; i<n_threads; i++){
		pthread_join(p[i], NULL);
		res += (args[i].res);
	}

	return res;
}
