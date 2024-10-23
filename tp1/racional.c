/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções. */

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max){
     return((rand() % (2*max + 1)) + min);    //(2*max + 1) separa o intervalo e (+min) desloca ele
}

/* Atribui os valores de numerador e denominador indicados a um racional */
struct racional cria_r (long numerador, long denominador){
     struct racional r;
     
     r.num = numerador;
     r.den = denominador;
     
     return(r);
}

/* Verifica se o racional é válido, ou seja, se o denominador não é 0 */
int valido_r (struct racional r){
     if(r.den == 0)
          return(0);
     
     return(1);
}

/* Retira o sinal do número para que a 
* conta do mdc possa ser realizada */
long modulo (long a){
     if (a<0){
          a = (a * (-1));
     }
     
     return(a);
}

/* calcula o Máximo Divisor Comum pelo método de Euclides */
long mdc (long a, long b){
     long sobra;

     if ((a==0) | (b==0)){ //testa se não irá dividir por 0 no método
          return(1);
     }
     
     sobra = 1;
     
     while (sobra != 0){
          sobra = a % b;
          a = b;
          if (sobra != 0){
               b = sobra;
          }
     }
     
     return(b);
}

/* Recebe um número racional e o simplifica, caso seja inválido
* devolve o racional sem simplificar */
struct racional simplifica_r (struct racional r){
     int div;
     
     if (!valido_r(r))
          return(r);
     
     /*desloca o sinal de negativo para o numerador, 
     * caso o denominador seja negativo, 
     * caso os dois sejam negativos retornará um positivo*/
     if(r.den<0){    
          r.num = (r.num * (-1));
          r.den = (r.den * (-1));
     }
     
     div = mdc(modulo(r.num),modulo(r.den)); //acha o mdc para reduzir ao máximo os valores
     r.num = r.num / div;
     r.den = r.den / div;
     
     return(r);
}

/* Retorna um número racional aleatório na forma simplificada */
struct racional sorteia_r (long min, long max){
     long numerador,denominador;
     
     numerador = aleat(min,max);
     denominador = aleat(min,max);
     
     return(simplifica_r(cria_r(numerador,denominador)));
     /* Seria possível remover o simplifica dessa linha, porém foi deixado para ficar de acordo com as especificações do .h*/
}

/* Mínimo Múltiplo Comum entre a e b */
long mmc (long a, long b){
     return((a*b)/ mdc(a,b));
}

/*Retorna a soma de dois racionais r1 e r2*/
struct racional soma_r (struct racional r1, struct racional r2){
     struct racional soma_t;
     
     soma_t.den = mmc(r1.den,r2.den);
     soma_t.num = (((r1.num*soma_t.den)/r1.den) + ((r2.num*soma_t.den)/r2.den));
     
     return(soma_t);
}

/*Retorna a subtração de dois racionais r1 e r2*/
struct racional subtrai_r (struct racional r1, struct racional r2){
     struct racional subtrai_t;
     
     subtrai_t.den = mmc(r1.den,r2.den);
     subtrai_t.num = (((r1.num*subtrai_t.den)/r1.den) - ((r2.num*subtrai_t.den)/r2.den));
     
     return(subtrai_t);
}

/*Retorna a multiplicação de dois racionais r1 e r2*/
struct racional multiplica_r (struct racional r1, struct racional r2){
     struct racional multiplica_t;

     multiplica_t.den = (r1.den * r2.den);
     multiplica_t.num = (r1.num * r2.num);
     
     return(multiplica_t);
}

/* Inverte o numerador e o denominador de um racional
* para realizar a divisão pelo método de multiplicação pelo inverso */
struct racional inverte_r (struct racional r){
     int aux;
     
     aux = r.num;
     r.num = r.den;
     r.den = aux;
     
     return(r);
}

/*Retorna a divisão de dois racionais r1 e r2*/
struct racional divide_r (struct racional r1, struct racional r2){
     struct racional divide_t;
     
     divide_t = multiplica_r(r1,inverte_r(r2));
     
     return(divide_t);
}

/* Imprime um racional simplificado */
void imprime_r (struct racional r){
     r = simplifica_r(r);
     
     if (r.num == 0) 
          printf("0 ");
     else if (r.den == 1){
          printf("%ld ", r.num);
     }
     else if (r.num == r.den){
          printf("1 ");
     }
     else{
          printf("%ld/%ld ", r.num,r.den);
     }
}
