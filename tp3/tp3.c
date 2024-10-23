#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Libera espaço alocado para o vetor */
void libera_v(struct racional **v){
  if (v){
    free(v);
    v = NULL;
  }
}

/* Lê elementos do vetor */
void le_vet(int tam, struct racional **v){
  int i;
  long num, den;
  for(i=0;i<tam;i++){
    scanf("%ld", &num);
    scanf("%ld", &den);
    v[i] = cria_r(num,den);
  }
}

/* Retira os valores inválidos do vetor e modifica seu tamanho */
void limpa_vet(int *tam, struct racional **v){
  int i,j;
  for(i=0;i<*tam;i++){
    j=(*tam-1);
    if(!valido_r(v[i])){
      if (valido_r(v[j])){
        destroi_r(v[i]);
        v[i] = v[j];
        (*tam)--;
      }else{
        destroi_r(v[j]);
        (*tam)--;
        i--;
       }
    }
  }
}

/* Ordena o vetor */
void ordena_vet(int tam, struct racional **v){
  struct racional *aux;
  int i,j;
  for(i=0; i<tam-1;i++){
    for (j=i+1; j<tam; j++){
      if (compara_r(v[i],v[j])==1){
        aux = v[j];
        v[j] = v[i];
        v[i] = aux;}
    }
  }
}

/* Soma todos os elementos do vetor */
void soma_vet(int tam, struct racional *soma, struct racional **v){
  int i;
  soma->num = 0;
  soma->den = 1;
  for (i=0; i<tam; i++){
    soma_r(soma,v[i],soma);
  }
}

/* Imprime elementos do vetor */
void imprime_v(int n, struct racional **v){
  int i;
  printf("VETOR = ");
  for (i=0;i<n;i++){
    imprime_r(v[i]);
    if (i!=n-1)
      printf(" ");}
  printf("\n");
}

void libera_rv(int n, struct racional **v){
  int i;
  for (i=0; i<n; i++){
    if(v[i]){
      destroi_r(v[i]);
      v[i]=NULL;}
  }
}

/* programa principal */
int main (){
  int n;
  struct racional *soma = malloc(sizeof(struct racional));
  if (!soma)
    return(1);

  scanf("%d", &n);
  
  struct racional **vetor = calloc(n, sizeof(struct racional *));
  if (!vetor)
    return (1);
  
  le_vet(n,vetor);
  imprime_v(n,vetor);

  limpa_vet(&n,vetor);
  imprime_v(n,vetor);
  
  ordena_vet(n,vetor);
  imprime_v(n,vetor);
  
  soma_vet(n,soma,vetor);
  printf("SOMA = ");
  imprime_r(soma);
  printf("\n");

  libera_rv(n,vetor);
  imprime_v(n,vetor);

  libera_v(vetor);
  destroi_r(soma);
  
  return (0) ;
}
