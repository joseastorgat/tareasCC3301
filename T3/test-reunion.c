#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "reunion.h"

int debug= 1;

typedef struct {
  pthread_mutex_t m;
  int entraron, salieron;
} Verif;
  
typedef struct {
  Reunion *r;
  int delay;
  char *nombre;
  Verif *pv;
} Param;

void *fun(void *ptr) {
  Param *param= ptr;
  Verif *pv= param->pv;
  if (debug)
    printf("Entrar de %s\n", param->nombre);
  entrar(param->r);
  pthread_mutex_lock(&pv->m);
  pv->entraron++;
  pthread_mutex_unlock(&pv->m);
  sleep(param->delay);
  if (debug)
    printf("Concluir de %s\n", param->nombre);
  pthread_mutex_lock(&pv->m);
  pv->salieron++;
  pthread_mutex_unlock(&pv->m);
  concluir(param->r);
  if (pv->salieron!=pv->entraron) {
    fprintf(stderr, "Funcionario %s salio antes de que todos concluyeran\n",
            param->nombre);
    exit(1);
  }
  return NULL;
}

void *testEnun(void *ptr) {
  Reunion *r= nuevaReunion();
  pthread_t f1, f2, f3;
  Verif v= { PTHREAD_MUTEX_INITIALIZER, 0, 0 };
  Param p1= { r, 1, "F1", &v };
  Param p2= { r, 4, "F2", &v };
  Param p3= { r, 4, "F3", &v };
  pthread_create(&f3, 0, fun, &p3);
  sleep(1);
  pthread_create(&f2, 0, fun, &p2);
  sleep(1);
  pthread_create(&f1, 0, fun, &p1);
  pthread_join(f1, NULL);
  pthread_join(f2, NULL);
  pthread_join(f3, NULL);
  return NULL;
}

#define N_REUNIONES 50

int main() {
  {
    printf("Test 1: un solo participante\n");
    Reunion *r= nuevaReunion();
    pthread_t f;
    Verif v= { PTHREAD_MUTEX_INITIALIZER, 0, 0 };
    Param p= { r, 1, "F", &v };
    pthread_create(&f, 0, fun, &p);
    pthread_join(f, NULL);
    printf("Test 1 aprobado\n");
  }
  {
    printf("Test 2: 3 participantes, igual al diagrama del enunciado\n");
    testEnun(NULL);
    printf("Test 2 aprobado\n");
  }
  {
    printf("Test 3: el mismo test 2 con muchas instancias en paralelo\n");
    debug= 0;
    pthread_t af[N_REUNIONES];
    for (int i= 0; i<N_REUNIONES; i++) {
      if (pthread_create(&af[i], 0, testEnun, NULL)!=0) {
        char *msg= "fallo pthread_create por exceso de threads\n";
        write(2, msg, strlen(msg));
        exit(i);
      }
    }
    for (int i= 0; i<N_REUNIONES; i++) {
      pthread_join(af[i], NULL);
    }
    printf("Test 3 aprobado\n");
  }
  printf("Felicitaciones: su tarea ha aprobado todos los tests.\n");
  printf("No olvide que su tarea sera inspeccionada para verificar\n");
  printf("el uso adecuado del mutex y la condicion.\n");
  return 0;
}
