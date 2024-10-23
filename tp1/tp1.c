/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* programa principal */
int main (){
    int i,n,max;
    
    struct racional r1,r2;
    struct racional soma_t, subtrai_t, multiplica_t, divide_t;
 
    srand (0);

    scanf("%d", &n);
    scanf("%d", &max);

    for (i=1;i<=n;i++){
        
        printf("%d: ", i);

        r1 = sorteia_r (-max,max);
        r2 = sorteia_r (-max,max);

        if (!valido_r(r1)){ 
          printf("INVALIDO \n");
          imprime_r(r2);
          return(1);
        }
/**/
        if (!valido_r(r2)){ 
          imprime_r(r1);
          printf("INVALIDO \n");
          return(1);
        }

        imprime_r(r1);
        imprime_r(r2);

        soma_t = soma_r(r1,r2);
        subtrai_t = subtrai_r(r1,r2);
        multiplica_t = multiplica_r(r1,r2);
        divide_t = divide_r(r1,r2);

        if (!valido_r(divide_t)){
          printf("DIVISAO INVALIDA \n");
          return(1);
        }

        imprime_r(soma_t);
        imprime_r(subtrai_t);
        imprime_r(multiplica_t);
        imprime_r(divide_t);

        printf("\n");
    }   
  return (0) ;
}
