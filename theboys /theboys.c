// programa principal do projeto "The Boys - 2024/2"
// Autor: Larissa Schaldach Cutti, GRR 20240590

#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100

void chega(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF) //tipo 0
{
    int espera = 0;
    H->base = B->id; //atualiza base de H
    if ((cjto_card(B->pres)<B->max) && (fila_tamanho(B->espera)==0) ) // se há vagas em B e a fila de espera em B está vazia:
        espera = 1; // espera = true
    else // senão:
        if (H->paciencia > (10 * fila_tamanho(B->espera))) // espera = (paciência de H) > (10 * tamanho da fila em B)
            espera = 1;

    if (espera){ // se espera:
        struct ev_t *espera = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento ESPERA (agora, H, B)
        espera->tempo = t;
        espera->heroi = H;
        espera->base = B;
        fprio_insere(LEF, espera, 1, espera->tempo);
    }else{ //senão:
        struct ev_t *desiste = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento DESISTE (agora, H, B)
        desiste->tempo = t;
        desiste->heroi = H;
        desiste->base = B;
        fprio_insere(LEF, desiste, 2, desiste->tempo); //cria e insere na LEF o evento DESISTE (agora, H, B)
    }
}

void espera(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF) //tipo 1
{
    fila_insere(B->espera, H); //adiciona H ao fim da fila de espera de B
    struct ev_t *avisa = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento AVISA (agora, B)
    avisa->tempo = t;
    avisa->base = B;
    fprio_insere(LEF, avisa, 3, avisa->tempo);
}

void desiste(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 2
{
    int dest = rand () % (N_BASES); // escolhe uma base destino D aleatória
    int D = W->Bases[dest];
    struct ev_t *viaja = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento VIAJA (agora, H, D)
    viaja->tempo = t;
    viaja->heroi = H;
    viaja->base = D;
    fprio_insere(LEF,viaja,6,viaja->tempo);
}

void avisa(int t, struct base_t *B, struct fprio_t *LEF) //tipo 3
{
    while (((cjto_card(B->pres))<B->lotação) && (fila_tamanho(B->espera)>0))//enquanto houver vaga em B e houver heróis esperando na fila:
    {
        struct heroi_t *HN = malloc(sizeof(struct heroi_t));
        fila_retira(B->espera, HN);//retira primeiro herói (H') da fila de B
        cjto_insere(B->pres, HN); //adiciona H' ao conjunto de heróis presentes em B
        struct ev_t *entra = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento ENTRA (agora, H', B)
        entra->tempo = t;
        entra->heroi = HN;
        entra->base = B;
        fprio_insere(LEF,entra,4,entra->tempo); 
    }
}

void entra(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF) //tipo 4
{
    int aleat = rand () % (20 - 1 + 1) + 1;
    int TPB = 15 + (H->paciencia * aleat); //calcula TPB = tempo de permanência na base: TPB = 15 + paciência de H * aleatório [1...20]
    struct ev_t *sai = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento SAI (agora + TPB, H, B)
    sai->tempo = t + TPB;
    sai->heroi = H;
    sai->base = B;
    fprio_insere(LEF,sai,5,sai->tempo);
}

void sai(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 5
{
    cjto_retira(B->pres,H); //retira H do conjunto de heróis presentes em B
    int dest = rand () % (N_BASES); // escolhe uma base destino D aleatória
    int D = W->Bases[dest];
    struct ev_t *viaja = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento VIAJA (agora, H, D)
    viaja->tempo = t;
    viaja->heroi = H;
    viaja->base = D;
    fprio_insere(LEF,viaja,6,viaja->tempo);
    struct ev_t *avisa = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento AVISA (agora, B)
    avisa->tempo = t;
    avisa->base = B;
    fprio_insere(LEF,avisa,3,avisa->tempo);
}

void viaja(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 6
{
    int dist, durac;
    int achou = 0;//calcula duração da viagem:
    struct base_t *Bat;
    for (int i=0; i<N_BASES && !achou; i++)
        if (W->Bases[i]->id == H->base){
            achou = 1;
            Bat = W->Bases[i];
        }
    if (Bat){ //reformular
        dist = (Bat->local->x * B->local->x) + (Bat->local->y * B->local->y);//distância = distância cartesiana entre a base atual de H e a base D 
        durac = dist / H->veloc; //duração = distância / velocidade de H
    }
    struct ev_t *chega = malloc(sizeof(struct ev_t));  //cria e insere na LEF o evento CHEGA (agora + duração, H, D)
    chega->tempo = t + durac;
    chega->heroi = H;
    chega->base = B;
    fprio_insere(LEF,chega,0,chega->tempo);
}

void morre(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF) //tipo 7
{
    cjto_retira(B->pres,H); //retira H do conjunto de heróis presentes em B
    H->status = 0; //muda o status de H para morto 
    struct ev_t *avisa = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento AVISA (agora, B)
    avisa->tempo = t;
    avisa->base = B;
    fprio_insere(LEF,avisa,3,avisa->tempo);
}

void missao(int t, struct mission_t *M, struct fprio_t *LEF, struct mundo_t *W) //tipo 8
{
    //calcula a distância de cada base ao local da missão M
    int achou = 0;
    struct base_t *B, *BMP;
    for (int i=0; i<N_BASES && !achou; i++){
        B = W->Bases[i];
        int dist = (M->local->x * B->local->x) + (M->local->y * B->local->y); //distância = distância cartesiana entre o local da missão e as bases do mundo 
        //ordenar um vetor das menores dist
        //testa da mais perto para mais longe se tem habilidades
        BMP = B;
    }
    //encontra BMP = base mais próxima da missão cujos heróis possam cumpri-la 
    if (BMP != NULL){ //se houver uma BMP:
        M->status = 1; //marca a missão M como cumprida
        int H_pres = cjto_card(BMP->pres);
        struct cjto_t *H_aux = cjto_copia(BMP->pres);
        struct heroi_t *presentes[N_HEROIS];
        for(int i = 0; i<H_pres; i++){
            for (int j = 0; j<N_HEROIS; j++){
                if (cjto_pertence(BMP->pres, W->Herois[j]) == 1)
                            presentes[i] = W->Herois[j];
            }
        }
        for(int i=0; i<H_pres; i++){ //para cada herói H presente na BMP:
            int risco = M->perigo / (presentes[i]->paciencia + presentes[i]->xp + 1); //risco = perigo (M) / (paciência (H) + experiência (H) + 1.0) (float?)
            int aleat = rand () % (30);
            if (risco > aleat){ //se risco > aleatório (0, 30): (0 - 29 ou 0 - 30?)
                struct ev_t *morre = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento MORRE (agora, H)
                morre->tempo = t;
                morre->heroi = presentes[i];
                fprio_insere(LEF,morre,7,morre->tempo);
            }else //senão:
                presentes[i]->xp++; //incrementa a experiência de H
        }
    }else{ //senão:
        struct ev_t *mission = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento MISSAO (T + 24*60, M) para o dia seguinte*/
        mission->tempo = t + (24*60);
        fprio_insere(LEF,mission,8,mission->tempo);
    }
}

void fim(int t) //tipo 9
{
    //encerra a simulação
    //apresenta estatísticas dos heróis
    //apresenta estatísticas das bases
    //apresenta estatísticas das missões
}

void inicia_mundo(struct mundo_t *W)
{
    W->nHerois = N_HEROIS;
    W->nBases = N_BASES;
    W->nMissoes = N_MISSOES;
    W->nHabilidades = N_HABILIDADES;
    W->tam_mundo->x = N_TAMANHO_MUNDO;
    W->tam_mundo->y = N_TAMANHO_MUNDO;
    W->relogio = 0;
    printf("Mundo criado\n");
}

void inicia_herois(struct mundo_t *W)
{
  int i;
  for (i=0; i<N_HEROIS; i++)
  {
    struct heroi_t *heroi = malloc (sizeof(struct heroi_t));
    if (!heroi)
        return;
    struct cjto_t *habi = malloc (sizeof(struct cjto_t));
    if (!habi){
        free(heroi);
        return;
    }
    
    int habilidades = rand () % (3 - 1 + 1) + 1;
    
    heroi->id = i; //id = número sequencial [0...N_HEROIS-1]
    heroi->xp = 0; //experiência = 0
    heroi->status = 1; //heroi vivo
    heroi->paciencia = rand() % (101); //paciência = número aleatório [0...100]
    heroi->veloc = rand () % (5000 - 50 + 1) + 50; //velocidade  = número aleatório [50...5000] (em metros/minuto = 3 Km/h a 300 Km/h)
    habi = cjto_cria(habilidades);
    int j;
    for (j=0; j<habilidades; j++){
        int h;
        h = rand () % (10);
        if (cjto_pertence(habi, h))
            j--;
        else 
            cjto_insere(habi,h);
    }
    heroi->hab = habi; //habilidades = conjunto com tamanho aleatório [1...3] de habilidades aleatórias
    W->Herois[i] = heroi;
  }
  printf("Herois criados\n");
}

void inicia_bases(struct mundo_t *W)
{
    int i;
    for(i=0; i<N_BASES; i++)
    {
        struct base_t *base = malloc (sizeof(struct base_t));
        if (!base)
            return;
        struct cjto_t *heros = malloc(sizeof(struct cjto_t));
        if (!heros){
            free(base);
            return;
        }
        struct lista_t *esp = malloc(sizeof(struct lista_t));
        if (!esp){
            free(base);
            free(heros);
            return;
        }

        base->id = i; //id = número sequencial [0...N_BASES-1]
        base->local->x = rand() % (20000); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
        base->local->y = rand() % (20000);
        base->max = rand() % (10 - 3 + 1) + 1; //lotação = número aleatório [3...10]
        heros = cjto_cria(base->max);
        base->pres = heros; // presentes = conjunto vazio (com capacidade para armazenar IDs de heróis até a lotação da base)
        esp = fila_cria(base->max);
        base->espera = esp;     
        W->Bases[i] = base;  
    }
}

void inicia_missoes(struct mundo_t *W)
{
    int i;
    for(i=0; i<N_MISSOES; i++){
        struct mission_t *missao = malloc(sizeof(struct mission_t));
        struct cjto_t *habi_rq = malloc(sizeof(struct cjto_t));

        missao->id = i; //id = número sequencial [0...N_MISSOES-1]
        missao->local->x = rand() % (20000); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
        missao->local->y = rand() % (20000);
        int habis = rand() % (10 - 6 + 1) + 1;
        habi_rq = cjto_cria(habis);
        int j;
        for (j=0; j<habis; j++){
            int h;
            h = rand () % (10);
            if (cjto_pertence(habi_rq, h))
                j--;
            else 
                cjto_insere(habi_rq,h);
        }
        missao->hab = habi_rq; //habilidades = conjunto com tamanho aleatório [6...10] de habilidades aleatórias
        missao->perigo = rand () % (101); //perigo      = número aleatório [0...100]
        W->Missoes[i] = missao;
    }
}

void inicia_eventos(struct mundo_t *W, struct fprio_t *LEF)
{
    for(int i=0; i<N_HEROIS; i++){ //para cada herói H:
        struct heroi_t *H = W->Herois[i];
        H->base = rand () % (N_BASES); //base  = número aleatório [0...N_BASES-1]
        int tempo = rand() % (4321);  //tempo = número aleatório [0...4320]  // 4320 = 60*24*3 = 3 dias
        struct ev_t *chega = malloc(sizeof(struct ev_t));  //criar e inserir na LEF o evento CHEGA (tempo, H, base)
        chega->tempo = tempo;
        chega->heroi = H;
        chega->base = W->Bases[H->base];
        fprio_insere(LEF,chega,0,chega->tempo);
    }

    for(int i=0; i<N_MISSOES; i++){ //para cada missão M:
        int tempo = rand() % (T_FIM_DO_MUNDO); //tempo = número aleatório [0...T_FIM_DO_MUNDO]
        struct ev_t *mission = malloc(sizeof(struct ev_t)); // criar e inserir na LEF o evento MISSÃO (tempo, M)
        mission->tempo = tempo;
        fprio_insere(LEF,mission,8,mission->tempo);
    }

    int tempo = T_FIM_DO_MUNDO; //tempo = T_FIM_DO_MUNDO
    struct ev_t *fim = malloc(sizeof(struct ev_t));
    fprio_insere(LEF,fim,9,tempo); //criar e inserir na LEF o evento FIM (tempo)
}

// programa principal
int main ()
{
    int ev,prio,t;
    struct heroi_t *H;
    struct base_t *B;
    struct mundo_t *W = malloc(sizeof(struct mundo_t)); //iniciar as entidades e atributos do mundo
    if (!H || !B || !W)
        return -1;
    inicia_mundo(W);
    inicia_herois(W);
    inicia_bases(W);
    inicia_missoes(W);

    struct fprio_t *LEF;
    LEF = fprio_cria(); // criar a lista de eventos futuros

    inicia_eventos(W,LEF); //criar os eventos iniciais
    W->relogio = T_INICIO; //relógio = 0

    while (W->relogio< T_FIM_DO_MUNDO){ //repetir até o fim da simulação
        fprio_retira(LEF, &ev, &prio, &t, H, B); //ev = 1º evento da lista de eventos futuros
        W->relogio = prio; // relógio = tempo (ev)
        switch (ev) //caso tipo (ev) seja:
        {
            case 0:
                chega(t, H, B, LEF);
                break;
            case 1:
                espera(t, H, B, LEF);
                break;
            case 2:
                desiste(t, H, B, LEF, W);
                break;
            case 3:
                avisa(t, B, LEF);
                break;
            case 4:
                entra(t, H, B, LEF);
                break;
            case 5:
                sai(t, H, B, LEF, W);
                break;
            case 6:
                viaja(t, H, B, LEF, W);
                break;
            case 7:
                morre(t, H, B, LEF);
                break;
            case 8:
                //missao(t, M, LEF, W);
        }
    }
/* 
  
apresentar resultados*/

  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}
