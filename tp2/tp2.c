#include <stdio.h>
#include "racional.h"

/* Lê os valores do vetor */
void le_vet(int tam, struct racional v[]){
  int i;
  long num, den;
  struct racional r;
  
  for(i=0;i<tam;i++){
    scanf("%ld", &num);
    scanf("%ld", &den);
    r = cria_r(num,den);
    v[i] = r;
  }
  
}

/* Retira os valores inválidos do vetor e modifica seu tamanho */
void limpa_vet(int *tam, struct racional v[]){
  struct racional aux;
  int i,j;

  for(i=0;i<*tam;i++){
    j=(*tam-1);
    if(!valido_r(v[i])){
      if (valido_r(v[j])){
        aux=v[j];
        v[j]=v[i];
        v[i]=aux;
        (*tam)--;
      }
      else{
        (*tam)--;
        i--;
      }
    }
  }
  
}

/* Ordena o vetor */
void ordena_vet(long tam, struct racional v[]){
  struct racional aux;
  int i,j;
  
  for(i=0; i<tam;i++){
    for (j=i+1; j<tam; j++){
      if (compara_r(v[i],v[j])==1){
        aux = v[j];
        v[j] = v[i];
        v[i] = aux;
      }
    }
  }

}

/* Soma todos os elementos do vetor */
struct racional soma_vet(long tam, struct racional v[]){
  struct racional result;
  int i;

  result.num = 0;
  result.den = 1;
  
  for (i=0; i<tam; i++){
    soma_r(result,v[i],&result);
  }  
 
  return(result);
}

/* programa principal */
int main (){
  int n,i;
  struct racional soma;
  struct racional vetor_r [100];

  scanf("%d", &n);

  le_vet(n,vetor_r);
  printf("VETOR = ");
  for (i=0;i<n;i++)
    imprime_r(vetor_r[i]);
  printf("\n");

  limpa_vet(&n,vetor_r);
  printf("VETOR = ");
  for (i=0;i<n;i++)
    imprime_r(vetor_r[i]); 
  printf("\n");
  
  ordena_vet(n,vetor_r);
  printf("VETOR = ");
  for (i=0;i<n;i++)
    imprime_r(vetor_r[i]); 
  printf("\n");
  
  soma = soma_vet(n,vetor_r);
  printf("SOMA = ");
    imprime_r(soma);
  printf("\n");
  
  return (0) ;
}
