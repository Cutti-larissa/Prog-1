#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fila_espera.h"
#include "entidades.h"
#include "fprio.h"

void chega(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF) //tipo 0
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
    fila_insere(B->espera,H); //adiciona H ao fim da fila de espera de B
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
