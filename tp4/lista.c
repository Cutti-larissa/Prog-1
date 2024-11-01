#include <stdio.h>
#include <stdlib.h> 
#include "lista.h"

struct lista_t *lista_cria (){
    struct lista_t *lst malloc(sizeof(struct lista_t)); //cria lst
    if (!lst)
        return(NULL);
    lst->prim=NULL;
    lst->ult=NULL;
    lst->tamanho=0;
    return(lst);
}

int lista_insere (struct lista_t *lst, int item, int pos){
 19     int i;
 20     struct item_t *itm = malloc(sizeof(struct item_t)); //cria item_t
 21
 22     if(!lst)
 23         return(-1);
 24     if (!itm || pos<-1)
 25         return(-1); //erro de alocação de memória ou posição indefinida
 26
 27     itm->valor = item;
 28
 29     if (pos > lst->tamanho || pos == -1){
 30         itm->ant=lst->ult; // se não tiver itens recebe NULL se não recebe o ponteiro do ultimo
 31         itm->prox=NULL; //Como vai ser o ultimo o proximo é null
 32         if (lst->tamanho == 0)
 33             lst->prim=itm;
 34         if(lst->ult)
 35             lst->ult->prox=itm;
 36         lst->ult = itm;
 37     }else{
 38         if(lst->tamanho==0){
 39             itm->ant=NULL;
 40             itm->prox=NULL;
 41             lst->ult=itm;
 42         }else{
 43             struct item_t *aux;
 44             aux=lst->prim;
 45             for(i=0;i<pos;i++)
 46                 aux=aux->prox;
 47             itm->ant=aux->ant;
 48             itm->prox=aux;
 49             if(pos!=0)
 50                 aux->ant->prox=itm;
 51             aux->ant=itm;
 52         }
 53     }
 54     if(pos==0)
 55         lst->prim=itm;
 56     lst->tamanho++;
 57     return(lst->tamanho); //retorna o tamanho da lista
}

int lista_retira (struct lista_t *lst, int *item, int pos){
 61     int i;
 62     struct item_t *aux;
 63     if(!lst)
 64         return(-1);
 65     if (!item || lst->tamanho == 0 || pos<-1 || pos>lst->tamanho)
 66         return(-1); //Erro de acesso de posição indefinida
 67     if (pos == -1 || pos == lst->tamanho-1){
 68         *item = lst->ult->valor;
 69         aux = lst->ult;
 70         if (lst->tamanho==1){
 71             lst->prim=NULL;
 72             lst->ult=NULL;
 73         }else{
 74             lst->ult->ant->prox = NULL;
 75             lst->ult=lst->ult->ant;
 76         }
 77     }else{
 78         aux=lst->prim;
 79         for (i=0;i<pos;i++)
 80                 aux=aux->prox;
 81         *item = aux->valor;
 82         if(pos==0){
 83             aux->prox->ant=NULL;
 84             lst->prim=aux->prox;
 85         }else{
 86             aux->ant->prox=aux->prox;
 87             aux->prox->ant=aux->ant;
 88         }
 89     }
 90     free(aux);
 91     lst->tamanho--;
 92     return(lst->tamanho);
 93 }
 int lista_consulta (struct lista_t *lst, int *item, int pos){
 96     int i;
 97     struct item_t *aux; //cria aux
 98     if(!lst)
 99         return(-1);
100     if (!item || pos>=lst->tamanho || pos <-1 || lst->tamanho == 0)
101         return(-1); //erro de consulta de posição inexistente
102     if(pos == -1){
103         *item=lst->ult->valor;
104         return(lst->tamanho);
105     }
106     aux=lst->prim;
107     for (i=1;i<=pos;i++)
108         aux=aux->prox;
109     *item=aux->valor;
110     return(lst->tamanho);
111 }

 int lista_procura (struct lista_t *lst, int valor){
114     int i;
115     struct item_t *aux;
116     if(!lst)
117         return(-1);
118     aux=lst->prim;
119     for(i=1;i<=lst->tamanho;i++){
120         if(aux->valor==valor)
121             return(i-1); // achou e devolve a posição
122         aux=aux->prox;
123     }
124     return(-1); //Não achou ou acessou posição inválida
125 }
126
127 int lista_tamanho (struct lista_t *lst){
128     if(!lst)
129         return(-1);
130     return(lst->tamanho);
131 }
132
133 void lista_imprime (struct lista_t *lst){
134     int i;
135     struct item_t *aux;
136     aux=lst->prim;
137     for (i=1;i<=lst->tamanho;i++){
138         printf("%d",aux->valor);
139         if (i!=lst->tamanho)
140             printf(" ");
141         aux=aux->prox;
142     }
143 }
 struct lista_t *lista_destroi (struct lista_t *lst){
146     struct item_t *aux;
147     aux=lst->prim;
148     while(lst->prim != NULL) {
149         lst->prim=aux->prox;
150         lst->tamanho--;
151         free(aux); //libera cada elemento
152         aux=lst->prim;
153     }
154
155     //free(lst->ult);
156     free(lst);
157     return(NULL);
