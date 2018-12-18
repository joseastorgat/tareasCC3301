#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "t1.h"

int main() {
  {
    printf("Tests de borrar_bits\n");
    // borrar_bits(0b00010001001, 0b1, 1) es 0
    uint res= borrar_bits(0x89, 1, 1);
    if (res != 0) {
      fprintf(stderr, "borrar_bits(0b00010001001, 0b1, 1) es %x, ", res);
      fprintf(stderr, "debio ser 0\n");
      exit(1);
    }
    // borrar_bits(0b111011001, 0b10, 2) es 0b110010001
    res= borrar_bits(0x1d9, 2, 2);
    if (res != 0x191) {
      fprintf(stderr, "borrar_bits(0b111011001, 0b10, 2) es %x, ", res);
      fprintf(stderr, "debio ser 0b110010001 (0x191)\n");
      exit(1);
    }
    // borrar_bits(0b111011001, 0b101, 3) es 0b110001001
    res= borrar_bits(0x1d9, 5, 3);
    if (res != 0x189)  {
      fprintf(stderr, "borrar_bits(0b111011001, 0b101, 3) es %x, ", res);
      fprintf(stderr, "debio ser 0b110001001 (0x189)\n");
      exit(1);
    }
    printf("Aprobado\n");
  }

  {
    printf("Tests de reemplazo\n");
    char *src= "";
    char *res= reemplazo(src, 'z', "ca va");
    if (strcmp(res, src)!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'z', \"ca va\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"%s\"\n", src);
      exit(1);
    }

    src= "bonjour a tous";
    res= reemplazo(src, 'z', "ca va");
    if (strcmp(res, src)!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'z', \"ca va\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"%s\"\n", src);
      exit(1);
    }
    src= "hola que tal";
    res= reemplazo(src, 'a', "xyz");
    if (strcmp(res, "holxyz que txyzl")!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'a', \"xyz\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"holxyz que txyzl\"\n");
      exit(1);
    }

    src= "hola que tal";
    res= reemplazo(src, 'a', "");
    if (strcmp(res, "hol que tl")!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'a', \"\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"hol que tl\"\n");
      exit(1);
    }

    printf("Aprobado\n");
  }

  {
    printf("Tests de reemplazo\n");
    //                0123456789012 3456
    char src[25]="%&/(hola que tal\0    )=?";
    char *r= &src[4]; // "hola que tal"
    // La idea es que antes de r estan los caracters %&/( que no deben
    // modificarse.  Despues de r estan )=? que tampoco deben modificarse.
    reemplazar(r, 'a', "opa");
    // r es "holopa que topal"
    if (strcmp(r, "holopa que topal")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"opa\") es \"%s\", ", r);
      fprintf(stderr, "debio ser \"holopa que topal\"\n");
      exit(1);
    }
    // verifica que no se haya modificado antes de r o despues de r
    if (strncmp(src,"%&/(",4)!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"opa\") modifico antes de r\n");
      exit(1);
    }
    if (strcmp(&src[4+17], ")=?")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"opa\") modifico despues de r\n");
      exit(1);
    }

    reemplazar(r, 'o', "");
    //       01234567890123
    // r es "hlpa que tpal"
    if (strcmp(r, "hlpa que tpal")!=0) {
      fprintf(stderr, "reemplazar(r, 'o', \"\") es \"%s\", ", r);
      fprintf(stderr, "debio ser \"hlpa que tpal\"\n");
      exit(1);
    }
    // verifica que no se haya modificado antes de r o despues de r
    if (strncmp(src,"%&/(",4)!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico antes de r\n");
      exit(1);
    }
    if (strcmp(&src[4+17], ")=?")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico despues de r\n");
      exit(1);
    }
    strcpy(&r[14], ")=?"); // para verificar que no se modifique despues de r
    reemplazar(r, 'a', "*");
    // r es "hlp* que tp*l"
    if (strcmp(r, "hlp* que tp*l")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"*\") es \"%s\", ", r);
      fprintf(stderr, "debio ser \"hlp* que tp*l\"\n");
      exit(1);
    }
    // verifica que no se haya modificado antes de r o despues de r
    if (strncmp(src,"%&/(",4)!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico antes de r\n");
      exit(1);
    }
    if (strcmp(&src[4+14], ")=?")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico despues de r\n");
      exit(1);
    }
    reemplazar(r, 'a', "*");

    printf("Aprobado\n");
  }

  printf("Felicitaciones!  Aprobo todos los tests.  Puede entregar.\n");

  return 0;
}

