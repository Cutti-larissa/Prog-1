#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "eventos.h"

void chega(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  int esperar = 0;
  int tam_fila = lista_tamanho(B->espera);
  H->base = B->id; 
  if ((cjto_card(B->pres)<B->max) && (tam_fila==0) ) 
    esperar = 1; 
  else 
    if (H->paciencia > (10 * tam_fila)) 
      esperar = 1;
  int presentes = cjto_card(B->pres);
  if (esperar)
  { 
    struct ev_t *espera = malloc(sizeof(struct ev_t));
    if (!espera)
      return;
    espera->heroi = H;
    espera->base = B;
    fprio_insere(LEF, espera, 1, tempo);
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n",W->relogio, H->id, B->id, presentes, B->max);
  }
  else
  { 
    struct ev_t *desiste = malloc(sizeof(struct ev_t));
    if (!desiste)
      return;
    desiste->heroi = H;
    desiste->base = B;
    fprio_insere(LEF, desiste, 2, tempo);
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n",W->relogio, H->id, B->id, presentes, B->max);
  }
}

void espera(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  lista_insere(B->espera, H->id, -1); 
  int tam_fila = lista_tamanho(B->espera);
  if (tam_fila > B->fila_max)
      B->fila_max = tam_fila;
  struct ev_t *avisa = malloc(sizeof(struct ev_t)); 
  if (!avisa)
    return;
  avisa->base = B;
  fprio_insere(LEF, avisa, 3, tempo);
  printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", W->relogio, H->id, B->id, tam_fila - 1);
}  

void desiste(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  int dest = rand () % (N_BASES); 
  struct base_t *D = W->Bases[dest];
  struct ev_t *viaja = malloc(sizeof(struct ev_t));
  if (!viaja)
    return;
  viaja->heroi = H;
  viaja->base = D;
  fprio_insere(LEF, viaja, 6, tempo);
  printf("%6d: DESIST HEROI %2d BASE %d\n", W->relogio, H->id, B->id);
}

void avisa(int tempo, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  int presentes = cjto_card(B->pres);
  printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", W->relogio, B->id, presentes, B->max);
  lista_imprime(B->espera);
  printf(" ]\n");
  while (((cjto_card(B->pres))<B->max) && (lista_tamanho(B->espera)>0))
  {
    int id_heroi;
    lista_retira(B->espera, &id_heroi, 0);
    cjto_insere(B->pres, id_heroi); 
    struct ev_t *entra = malloc(sizeof(struct ev_t));
    if (!entra)
      return;
    entra->heroi = W->Herois[id_heroi];
    entra->base = B;
    fprio_insere(LEF, entra, 4, tempo); 
    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", W->relogio, B->id, id_heroi);
  }
}

void entra(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  int aleat = rand () % (20 - 1 + 1) + 1;
  int TPB = 15 + (H->paciencia * aleat);
  int h_sai = tempo + TPB;
  int presentes = cjto_card(B->pres);
  struct ev_t *sai = malloc(sizeof(struct ev_t));
  if (!sai)
    return;
  sai->heroi = H;
  sai->base = B;
  fprio_insere(LEF, sai, 5, h_sai);
  printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", W->relogio, H->id, B->id, presentes, B->max, h_sai);
}

void sai(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  cjto_retira(B->pres, H->id); 
  int presentes = cjto_card(B->pres);
  int dest = rand () % (N_BASES); 
  struct base_t *D = W->Bases[dest];
  struct ev_t *viaja = malloc(sizeof(struct ev_t));
  if (!viaja)
    return;
  viaja->heroi = H;
  viaja->base = D;
  fprio_insere(LEF, viaja, 6, tempo);
  struct ev_t *avisa = malloc(sizeof(struct ev_t));
  if (!avisa)
    return;
  avisa->base = B; 
  fprio_insere(LEF, avisa, 3, tempo);
  printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", W->relogio, H->id, B->id, presentes, B->max);
}

void viaja(int tempo, struct heroi_t *H, struct base_t *D, struct fprio_t *LEF, struct mundo_t *W)
{
  struct base_t *B = W->Bases[H->base]; 
  int dist = hypot((B->local.x - D->local.x),(B->local.y - D->local.y));  
  int durac = dist / H->veloc;
  int h_chega = tempo + durac;
  struct ev_t *chega = malloc(sizeof(struct ev_t));
  if (!chega)
    return;
  chega->heroi = H;
  chega->base = D;
  fprio_insere(LEF, chega, 0, tempo + durac);
  printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VELOC %d CHEGA %d\n", W->relogio, H->id, B->id, D->id, dist, H->veloc, h_chega);
}

void morre(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  cjto_retira(B->pres, H->id);
  H->status = 0;  
  struct ev_t *avisa = malloc(sizeof(struct ev_t));
  if (!avisa)
    return;
  avisa->base = B;
  fprio_insere(LEF, avisa, 3, tempo);
  printf("%6d: MORRE HEROI %2d MISSAO %d\n", W->relogio, H->id, H->morte);
}

/*Acha o menor valor no vetor de distancias e retorna seu indice 
  que é igual ao id da Base com essa distância da missão*/
int minimo_vetor(int dist[N_BASES], int a, int b){
  int menor = a;
  for (int i = a +1; i<b; i++)
    if (dist[i]<dist[menor])
      menor = i;
  return (menor);
}

void missao(int tempo, struct ev_t *M, struct fprio_t *LEF, struct mundo_t *W) 
{
  M->missao->tentativas++;
  printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", W->relogio, M->missao->id, M->missao->tentativas);
  cjto_imprime(M->missao->hab);
  printf(" ]\n");
  int distancias[N_BASES];
  struct base_t *B, *BMP;
  BMP = NULL;
  for (int i=0; i<N_BASES; i++)
  {
    B = W->Bases[i];
    int dist = hypot((M->missao->local.x - B->local.x),(M->missao->local.y - B->local.y)); 
    distancias[i] = dist; 
  }

  struct cjto_t *habilidades = cjto_cria(N_HABILIDADES);
  if (!habilidades)
    return;
  for (int j = 0; j < N_BASES && !BMP; j++)
  {
    int menor = minimo_vetor(distancias, j, N_BASES);
    for (int l = 0; l < N_HEROIS; l++) 
      if (cjto_pertence(W->Bases[menor]->pres, l))
      { 
        struct cjto_t *aux = cjto_uniao(habilidades, W->Herois[l]->hab);
        cjto_destroi(habilidades);
        habilidades = aux;
      }
    if (cjto_iguais(M->missao->hab, habilidades)) 
      BMP = W->Bases[menor];  
  }
  if (BMP)
  {
    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", W->relogio, M->missao->id, BMP->id);
    cjto_imprime(habilidades);
    printf(" ]\n");
    BMP->nMissoes++;
    W->mCumpridas++;
    for (int l = 0; l < N_HEROIS; l++)
    {
      if (cjto_pertence(BMP->pres, l))
      {
          int risco = M->missao->perigo / (W->Herois[l]->paciencia + W->Herois[l]->xp + 1);
          int aleat = rand () % (31);
          if (risco > aleat)
          {
            W->Herois[l]->morte = BMP->id;
            struct ev_t *morre = malloc(sizeof(struct ev_t)); 
            if (!morre)
              return;
            morre->base = BMP;
            morre->heroi = W->Herois[l];
            fprio_insere(LEF, morre, 7, tempo);
          }else 
            W->Herois[l]->xp++; 
      }
    }
  }
  else
  { 
    printf("%6d: MISSAO %d IMPOSSIVEL\n", W->relogio, M->missao->id);
    int h_missao = tempo + 1440;
    struct ev_t *mission = malloc(sizeof(struct ev_t));
    if (!mission)
      return;
    mission->missao = M->missao;
    fprio_insere(LEF, mission, 8, h_missao);
  }
  cjto_destroi(habilidades);
}

void fim(struct mundo_t *W, int evt_trat)
{
  int i, tent_min = 1, tent_max = 0 , mortos = 0;
  printf("%d: FIM\n", T_FIM_DO_MUNDO);
  printf("\n");
  for (i = 0; i < N_HEROIS; i++)
  {
    struct heroi_t *H = W->Herois[i];
    printf("HEROI %2d ", H->id);
    if (H->status == 0)
    {
      printf("MORTO ");
      mortos++;
    }
    else
      printf("VIVO ");
    printf("PAC %3d VEL %4d HABS [ ", H->paciencia, H->veloc);
    cjto_imprime(H->hab);
    printf(" ]\n");
  }
  printf("\n");

  for (i = 0; i < N_BASES; i++)
  { 
    struct base_t *B = W->Bases[i];
    printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", B->id, B->max, B->fila_max, B->nMissoes);
  }
  printf("\n");

  for (i = 0; i < N_MISSOES; i++)
    if (W->Missoes[i]->tentativas > tent_max)
      tent_max = W->Missoes[i]->tentativas;

  printf("EVENTOS TRATADOS: %d\n", evt_trat);
  double mis_cump = ((W->mCumpridas/(N_MISSOES * 1.0))*100);
  printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", W->mCumpridas, N_MISSOES, mis_cump); 
  double tentativas = ((tent_min / (tent_max * 1.0))*10000);
  printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", tent_min, tent_max, tentativas);
  double mortalidade = (mortos / (N_HEROIS * 1.0))*100;
  printf("TAXA MORTALIDADE: %.1f%%\n", mortalidade);
}
