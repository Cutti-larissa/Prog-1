#include <stdio.h>
#include "racional.h"

/* calcula o Máximo Divisor Comum pelo método de Euclides */
long mdc (long a, long b){
  long sobra;
  
  if ((a==0) | (b==0))
    return(1);
  
  sobra = 1;  
  while (sobra != 0){
    sobra = a % b;
    a = b;
    if (sobra != 0)
      b = sobra;
  }
  
  return(b);
}

/* Mínimo Múltiplo Comum entre a e b */
long mmc (long a, long b){
  return((a*b)/mdc(a,b));
}

/* "Retira" o sinal do número */
long modulo (long a){
  if (a<0)
    a = (a * (-1));
  
  return(a);
}

/* Recebe um número racional e o simplifica, caso seja inválido
* devolve o racional sem simplificar */
struct racional simplifica_r (struct racional r){
  int div; 
  
  if (!valido_r(r))
    return(r);
  
  if(r.den<0){    
    r.num = (r.num * (-1));
    r.den = (r.den * (-1));
  } 
  
  div = mdc(modulo(r.num),modulo(r.den));
  r.num = r.num / div;
  r.den = r.den / div;  
  
  return(r);
}

struct racional cria_r (long numerador, long denominador){
  struct racional r;
  
  r.num = numerador;
  r.den = denominador;
  r = simplifica_r(r);
  
  return(r);
}

int valido_r (struct racional r){
  if(r.den == 0)
    return(0); 
  
  return(1);
}

int subtrai_r (struct racional r1, struct racional r2, struct racional *r3){
  if ((!valido_r(r1)) || (!valido_r(r2)) || (!r3))
    return(0);
  
  r3->den = mmc(r1.den,r2.den);
  r3->num = (((r1.num*r3->den)/r1.den) - ((r2.num*r3->den)/r2.den));  
  return(1);
}

int compara_r (struct racional r1, struct racional r2){
  struct racional result;
  
  result.num = 0;
  result.den = 1;
  
  if ((!valido_r(r1)) || (!valido_r(r2)))
    return(-2);
  
  subtrai_r(r1,r2,&result);
  
  if (result.num < 0)
    return(-1);
  else if (result.num == 0)
    return(0);
  return(1);
}

int soma_r (struct racional r1, struct racional r2, struct racional *r3){
  if ((!valido_r(r1)) | (!valido_r(r2)) | (!r3))
    return(0);
  
  r3->den = mmc(r1.den,r2.den);
  r3->num = (((r1.num*r3->den)/r1.den) + ((r2.num*r3->den)/r2.den));
  simplifica_r(*r3);
  
  return(1);
}

void imprime_r (struct racional r){
  r = simplifica_r(r);
  if (!valido_r(r))
    printf("NaN ");
  else if (r.num == 0) 
    printf("0 ");
  else if (r.den == 1)
    printf("%ld ", r.num);
  else if (r.num == r.den)
    printf("1 ");
  else
    printf("%ld/%ld ", r.num,r.den);
}
