#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fila_espera.h"
#include "entidades.h"

void chega(int t, struct heroi_t *H, struct base_t *B) //tipo 0
{
    int espera = 0;
    H->base = B->id; //atualiza base de H
    if ((cjto_card(B->pres)<B->lotação) && (fila_tamanho(B->espera)==0) ) // se há vagas em B e a fila de espera em B está vazia:
        espera = 1; // espera = true
    else // senão:
        if (H->paciencia > (10 * fila_tamanho(B->espera))) // espera = (paciência de H) > (10 * tamanho da fila em B)
            espera = 1;

    if (espera){ // se espera:
        struct ev_t *espera = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento ESPERA (agora, H, B)
        espera->tempo = t;
        espera->heroi = H;
        espera->base = B;
        fprio_insere(LEF, espera, 1, t);
    }else{ //senão:
        struct ev_t *desiste = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento DESISTE (agora, H, B)
        desiste->tempo = t;
        desiste->heroi = H;
        desiste->base = B;
        fprio_insere(LEF, desiste, 1, t); //cria e insere na LEF o evento DESISTE (agora, H, B)
    }
}

void espera(int t, struct heroi_t *H, struct base_t *B) //tipo 1
{
   fila_insere(B->espera,H) //adiciona H ao fim da fila de espera de B
    struct ev_t *avisa = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento AVISA (agora, B)
    avisa->tempo = t;
    avisa->Base = B;
}

void desiste(int t, struct heroi_t *H, struct base_t *B) //tipo 2
{
   /* escolhe uma base destino D aleatória
cria e insere na LEF o evento VIAJA (agora, H, D)*/
}

void avisa(int t, struct base_t *B) //tipo 3
{
   /*enquanto houver vaga em B e houver heróis esperando na fila:
    retira primeiro herói (H') da fila de B
    adiciona H' ao conjunto de heróis presentes em B
    cria e insere na LEF o evento ENTRA (agora, H', B)*/
}

void entra(int t, struct heroi_t *H, struct base_t *B) //tipo 4
{
/*calcula TPB = tempo de permanência na base:
    TPB = 15 + paciência de H * aleatório [1...20]
cria e insere na LEF o evento SAI (agora + TPB, H, B)*/
}

void sai(int t, struct heroi_t *H, struct base_t *B) //tipo 5
{
    /*retira H do conjunto de heróis presentes em B
escolhe uma base destino D aleatória
cria e insere na LEF o evento VIAJA (agora, H, D)
cria e insere na LEF o evento AVISA (agora, B)*/
}

void viaja(int t, struct heroi_t *H, struct base_t *B) //tipo 6
{
    /*calcula duração da viagem:
    distância = distância cartesiana entre a base atual de H e a base D 
    duração = distância / velocidade de H
cria e insere na LEF o evento CHEGA (agora + duração, H, D)*/
}

void morre(int t, struct heroi_t *H, struct base_t *B) //tipo 7
{
    /*retira H do conjunto de heróis presentes em B
muda o status de H para morto 
cria e insere na LEF o evento AVISA (agora, B)*/
}

void missao(int t, int m) //tipo 8
{
/*calcula a distância de cada base ao local da missão M
encontra BMP = base mais próxima da missão cujos heróis possam cumpri-la 
se houver uma BMP:
    marca a missão M como cumprida
    para cada herói H presente na BMP:
        risco = perigo (M) / (paciência (H) + experiência (H) + 1.0)
        se risco > aleatório (0, 30):
            cria e insere na LEF o evento MORRE (agora, H)
        senão:
            incrementa a experiência de H
senão:
    cria e insere na LEF o evento MISSAO (T + 24*60, M) para o dia seguinte*/
}

void fim(int t) //tipo 9
{
    /*encerra a simulação
  apresenta estatísticas dos heróis
  apresenta estatísticas das bases
  apresenta estatísticas das missões*/
}
