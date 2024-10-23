#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Maximo Divisor Comum entre a e b      */
long mdc (long a, long b){
  if (b==0)
    return(a);
  return(mdc(b, a%b));
}

/* Minimo Multiplo Comum entre a e b */
long mmc (long a, long b){
  return((a*b)/mdc(a,b));
}

int valido_r (struct racional *r){
  if(!r)
    return(0);
  if((r->den == 0))
    return(0); 
  return(1);
}

/* "Retira" o sinal do número */
long modulo (long a){
  if (a<0)
    a = (a * (-1));
  return(a);
}

int simplifica_r (struct racional *r){
  int div;
  if(!r)
    return(0);
  if (!valido_r(r))
    return(0);
  if(r->den<0){    
    r->num = -r->num;
    r->den = -r->den;} 
  div = mdc(modulo(r->num),modulo(r->den));
  r->num = r->num / div;
  r->den = r->den / div;  
  return(1);
}

struct racional *cria_r (long numerador, long denominador){
  struct racional *pr = malloc(sizeof(struct racional));
  if (!pr)
    return(NULL);
  pr->num = numerador;
  pr->den = denominador;
  simplifica_r(pr);
  return(pr);
}

void destroi_r (struct racional *r){
  if (r){
    free(r);
    r = NULL;
  }
}

int soma_r (struct racional *r1, struct racional *r2, struct racional *r3){
  long num_1,num_2,novo_den;
  if(!r1 || !r2 || !r3)
    return(0);
  if ((!valido_r(r1)) || (!valido_r(r2)))
    return(0);
  novo_den = mmc(r1->den,r2->den);
  num_1 = ((r1->num*novo_den)/r1->den);
  num_2 = ((r2->num*novo_den)/r2->den);
  r3->num = (num_1 + num_2);
  r3->den = novo_den;
  simplifica_r(r3);
  return(1);
}

int compara_r (struct racional *r1, struct racional *r2){
  long num_r1, num_r2, den_r3;
  if((!r1)||(!r2))
    return(-2);
  if ((!valido_r(r1)) || (!valido_r(r2)))
    return(-2);
  den_r3 = mmc(r1->den,r2->den);
  num_r1 = ((r1->num*den_r3)/r1->den);
  num_r2 = ((r2->num*den_r3)/r2->den);
  if (num_r1 < num_r2)
    return(-1);
  else if (num_r1 > num_r2)
    return(1);
  return(0);
}

void imprime_r (struct racional *r){
  if (!r)
    printf("NULL");
  else{
    simplifica_r(r);
    if (!valido_r(r))
      printf("NaN");
    else if (r->num == 0) 
      printf("0");
    else if (r->den == 1)
      printf("%ld", r->num);
    else if (r->num == r->den)
      printf("1");
    else
      printf("%ld/%ld", r->num,r->den);
  }
}
