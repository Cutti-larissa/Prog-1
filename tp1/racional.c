/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções. */

#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

long aleat (long min, long max){
     return((rand() % (2*max + 1)) + min);
}

struct racional cria_r (long numerador, long denominador){
     struct racional r;     
     r.num = numerador;
     r.den = denominador;
     return(r);
}

int valido_r (struct racional r){
     if(r.den == 0)
          return(0);     
     return(1);
}

long modulo (long a){
     if (a<0)
          a = (a * (-1));
     return(a);
}

long mdc (long a, long b){
     if ((a==0) | (b==0)) 
          return(1);
     
     long sobra = 1;
     
     while (sobra != 0){
          sobra = a % b;
          a = b;
          if (sobra != 0)
               b = sobra;
     }
     return(b);
}

struct racional simplifica_r (struct racional r){
     if (!valido_r(r))
          return(r);
     
     if(r.den<0){    
          r.num = (r.num * (-1));
          r.den = (r.den * (-1));
     }
     
     int div = mdc(modulo(r.num),modulo(r.den)); 
     r.num = r.num / div;
     r.den = r.den / div;
     
     return(r);
}

struct racional sorteia_r (long min, long max){
     long numerador = aleat(min,max);
     long denominador = aleat(min,max);
     
     return(simplifica_r(cria_r(numerador,denominador)));
}

long mmc (long a, long b){
     return((a*b)/ mdc(a,b));
}

struct racional soma_r (struct racional r1, struct racional r2){
     struct racional soma_t;
     
     soma_t.den = mmc(r1.den,r2.den);
     soma_t.num = (((r1.num*soma_t.den)/r1.den) + ((r2.num*soma_t.den)/r2.den));
     
     return(soma_t);
}

struct racional subtrai_r (struct racional r1, struct racional r2){
     struct racional subtrai_t;
     
     subtrai_t.den = mmc(r1.den,r2.den);
     subtrai_t.num = (((r1.num*subtrai_t.den)/r1.den) - ((r2.num*subtrai_t.den)/r2.den));
     
     return(subtrai_t);
}

struct racional multiplica_r (struct racional r1, struct racional r2){
     struct racional multiplica_t;

     multiplica_t.den = (r1.den * r2.den);
     multiplica_t.num = (r1.num * r2.num);
     
     return(multiplica_t);
}

struct racional inverte_r (struct racional r){
     int aux = r.num;
     r.num = r.den;
     r.den = aux;
     
     return(r);
}

struct racional divide_r (struct racional r1, struct racional r2){
     struct racional divide_t = multiplica_r(r1,inverte_r(r2));
     return(divide_t);
}

void imprime_r (struct racional r){
     r = simplifica_r(r);
     
     if (r.num == 0){ 
          printf("0 ");
     }else if (r.den == 1){
          printf("%ld ", r.num);
     }else if (r.num == r.den){
          printf("1 ");
     }else
          printf("%ld/%ld ", r.num,r.den);
}
