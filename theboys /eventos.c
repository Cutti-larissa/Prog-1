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
  H->base = B->id; //atualiza base de H
  if ((cjto_card(B->pres)<B->max) && (tam_fila==0) ) // se há vagas em B e a fila de espera em B está vazia:
    esperar = 1; // espera = true
  else // senão:
    if (H->paciencia > (10 * tam_fila)) // espera = (paciência de H) > (10 * tamanho da fila em B)
      esperar = 1;

  if (esperar){ // se espera: //transformar em um só muda o tipo??
    struct ev_t *espera = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento ESPERA (agora, H, B)
    espera->heroi = H;
    espera->base = B;
    fprio_insere(LEF, espera, 1, tempo);
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA",W->relogio, H->id, B->id, cjto_card(B->pres), B->max);
  }else{ //senão:
    struct ev_t *desiste = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento DESISTE (agora, H, B)
    desiste->heroi = H;
    desiste->base = B;
    fprio_insere(LEF, desiste, 2, tempo);
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE",W->relogio, H->id, B->id, cjto_card(B->pres), B->max);
  }
}

void espera(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  lista_insere(B->espera, H->id, -1); //adiciona H ao fim da fila de espera de B
  int tam_fila = lista_tamanho(B->espera);
  if (tam_fila > B->fila_max)
      B->fila_max = tam_fila;
  struct ev_t *avisa = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento AVISA (agora, B)
  avisa->base = B;
  fprio_insere(LEF, avisa, 3, tempo);
  printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", W->relogio, H->id, B->id, tam_fila - 1);
}

void desiste(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  int dest = rand () % (N_BASES); // escolhe uma base destino D aleatória
  struct base_t *D = W->Bases[dest];
  struct ev_t *viaja = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento VIAJA (agora, H, D)
  viaja->heroi = H;
  viaja->base = D;
  fprio_insere(LEF, viaja, 6, tempo);
  printf("%6d: DESIST HEROI %2d BASE %d\n", W->relogio, H->id, B->id);
}

void avisa(int tempo, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", W->relogio, B->id, cjto_card(B->pres), B->max);
  lista_imprime(B->espera);
  printf(" ]\n");
  while (((cjto_card(B->pres))<B->max) && (lista_tamanho(B->espera)>0))//enquanto houver vaga em B e houver heróis esperando na fila:
  {
    struct heroi_t *HN = malloc(sizeof(struct heroi_t));
    lista_retira(B->espera, &HN->id, 0);//retira primeiro herói (H') da fila de B
    cjto_insere(B->pres, HN->id); //adiciona H' ao conjunto de heróis presentes em B
    struct ev_t *entra = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento ENTRA (agora, H', B)
    entra->heroi = HN;
    entra->base = B;
    fprio_insere(LEF, entra, 4, tempo); 
    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", W->relogio, B->id, HN->id);
  }
}

void entra(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  int aleat = rand () % (20 - 1 + 1) + 1;
  int TPB = 15 + (H->paciencia * aleat); //calcula TPB = tempo de permanência na base: TPB = 15 + paciência de H * aleatório [1...20]
  struct ev_t *sai = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento SAI (agora + TPB, H, B)
  sai->heroi = H;
  sai->base = B;
  fprio_insere(LEF, sai, 5, tempo + TPB);
  printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d", W->relogio, H->id, B->id, cjto_card(B->pres), B->max, sai->tempo);
}

void sai(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  cjto_retira(B->pres, H->id); //retira H do conjunto de heróis presentes em B
  int dest = rand () % (N_BASES); // escolhe uma base destino D aleatória
  struct base_t *D = W->Bases[dest];
  struct ev_t *viaja = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento VIAJA (agora, H, D)
  viaja->heroi = H;
  viaja->base = D;
  fprio_insere(LEF, viaja, 6, tempo);
  struct ev_t *avisa = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento AVISA (agora, B)
  avisa->base = B; 
  fprio_insere(LEF, avisa, 3, tempo);
  printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)", W->relogio, H->id, B->id, cjto_card(B->pres), B->max);
}

void viaja(int tempo, struct heroi_t *H, struct base_t *D, struct fprio_t *LEF, struct mundo_t *W)
{
  struct base_t *B = W->Bases[H->base]; //calcula duração da viagem:
  //distância = distância cartesiana entre a base atual de H e a base D 
  //int dist = hypot((B->local->x - D->local->x),(B->local->y - D->local->y));  
  int dist = sqrt(((B->local->x - D->local->x)*(B->local->x - D->local->x)) + ((B->local->y - D->local->y)*(B->local->y - D->local->y)));
  int durac = dist / H->veloc; //duração = distância / velocidade de H
  struct ev_t *chega = malloc(sizeof(struct ev_t));  //cria e insere na LEF o evento CHEGA (agora + duração, H, D)
  chega->heroi = H;
  chega->base = D;
  fprio_insere(LEF, chega, 0, tempo + durac);
  printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VELOC %d CHEGA %d\n", W->relogio, H->id, B->id, D->id, dist, H->veloc, tempo + durac);
}

void morre(int tempo, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W)
{
  cjto_retira(B->pres, H->id); //retira H do conjunto de heróis presentes em B
  H->status = 0; //muda o status de H para morto 
  struct ev_t *avisa = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento AVISA (agora, B)
  avisa->base = B;
  fprio_insere(LEF, avisa, 3, tempo);
  printf("%6d: MORRE HEROI %2d MISSAO %d", W->relogio, H->id, H->morte);
}

/*Acha o menor valor no vetor de distancias e retorna seu indice 
  que é igual ao id da Base com essa distância da missão*/
int minimo_vetor(int dist[N_BASES], int a, int b){
  if (b == a)
    return (a);
  int menor = minimo_vetor(dist, a, b-1);
  if (dist[b]<dist[menor])
    menor = b;
  return (menor);
}

/*Acha a base mais próxima apta para realizar a missão,
  caso a missão seja impossível retorna com BMP = NULL*/
struct cjto_t *acha_BMP(int dist[N_BASES], int a, struct base_t *BMP, struct cjto_t *hab_rq, struct mundo_t *W) //chat
{
  if (a == N_BASES)
    return (NULL);
  int i = minimo_vetor(dist, a, N_BASES); //acha a base mais próxima
  struct cjto_t *habilidades = malloc(sizeof(struct cjto_t));
  for (int l = 0; l<N_HEROIS; l++) //para cada heroi
    if (cjto_pertence(W->Bases[i]->pres, l)) //se ele está presente na base
      habilidades = cjto_uniao(habilidades, W->Herois[l]->hab); //adiciona as habilidades dele ao cjto de habilidades da base
  if (cjto_iguais(hab_rq, habilidades)) // verifica se a base tem as habilidades necessárias
  { 
    BMP = W->Bases[i]; //se tem BMP = Base;   
    return(habilidades);
  }else //se não chama o algoritmo para o vetor começando da segunda posição acha_BMP(dist, i++, BMP)
    acha_BMP(dist, a++, BMP, hab_rq, W);
}

void missao(int tempo, struct ev_t *M, struct fprio_t *LEF, struct mundo_t *W) //adicionar mensagens depuração
{
  M->missao->tentativa++;
  printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", W->relogio, M->missao->id, M->missao->tentativa);
  cjto_imprime(M->missao->hab);
  printf(" ]\n");
  int distancias[N_BASES]; //calcula a distância de cada base ao local da missão M
  struct base_t *B, *BMP;
  for (int i=0; i<N_BASES; i++){
    B = W->Bases[i];
    //int dist = hypot((M->missao->local->x - D->local->x),(M->missao->local->y - D->local->y));  //distância = distância cartesiana entre o local da missão e as bases do mundo
    int dist = sqrt(((M->missao->local->x - B->local->x)*(M->missao->local->x - B->local->x)) + ((M->missao->local->y - B->local->y)*(M->missao->local->y - B->local->y)));
    distancias[i] = dist; //a distancia está no indice correspondente a sua base
  }
    
  struct cjto_t *habilid = acha_BMP(distancias, 0, BMP, M->missao->hab, W); //encontra BMP = base mais próxima da missão cujos heróis possam cumpri-la 
    
  if (BMP) //se houver uma BMP:
  {
    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", W->relogio, M->missao->id, BMP->id);
    cjto_imprime(habilid);
    printf(" ]\n");
    M->missao->status = 1; //marca a missão M como cumprida
    BMP->nMissoes++;
    W->mCumpridas++;
    for (int l = 0; l<N_HEROIS; l++){ //para cada heroi
      if (cjto_pertence(BMP->pres, l)){ //se ele está presente na base
          int risco = M->missao->perigo / (W->Herois[l]->paciencia + W->Herois[l]->xp + 1); //risco = perigo (M) / (paciência (H) + experiência (H) + 1.0) (float?)
          int aleat = rand () % (30);
          if (risco > aleat) //se risco > aleatório (0, 30): (0 - 29 ou 0 - 30?)
          {
            W->Herois[l]->morte = BMP->id; 
            struct ev_t *morre = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento MORRE (agora, H)
            morre->heroi = W->Herois[l];
            fprio_insere(LEF, morre, 7, tempo);
          }else{ //senão:
            W->Herois[l]->xp++; //incrementa a experiência de H
            printf("%6d: MISSAO %d IMPOSSIVEL", W->relogio, M->missao->id);
          }
      }
    }
  }else{ //senão:
    printf("Missão adiada");
    struct ev_t *mission = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento MISSAO (T + 24*60, M) para o dia seguinte
    mission->missao = M->missao;
    fprio_insere(LEF, mission, 8, tempo + 1440);
  }
}

void fim(struct mundo_t *W, int evt_trat)
{
  int i, mortos = 0;
  printf("%d: FIM", T_FIM_DO_MUNDO);//encerra a simulação
  for (i=0; i<N_HEROIS; i++){ //apresenta estatísticas dos heróis
    struct heroi_t *H = W->Herois[i];
    printf("HEROI %2d ", H->id);
    if (H->status == 0){
      printf("MORTO ");
      mortos++;
    }else
      printf("VIVO ");
    printf("PAC %3d VEL %4d HABS [ ", H->paciencia, H->veloc);
    cjto_imprime(H->hab);
    printf(" ]\n");
  }
  for (i=0; i<N_BASES; i++){ //apresenta estatísticas das bases
    struct base_t *B = W->Bases[i];
    printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", B->id, B->max, B->fila_max, B->nMission);
  }
  printf("EVENTOS TRATADOS: %d\n", evt_trat); 
  printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", W->mCumpridas, N_MISSOES, W->mCumpridas/(N_MISSOES*1.0)); //apresenta estatísticas das missões
  //printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max, min/(max*1.0))
  int mortalidade = mortos / N_HEROIS*(1.0);
  printf("TAXA MORTALIDADE: %.1f%%\n", mortalidade);
}
