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

void chega(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 0
{
    int esperar = 0;
    int tamanho = lista_tamanho(B->espera);
    H->base = B->id; //atualiza base de H
    if ((cjto_card(B->pres)<B->max) && (tamanho==0) ) // se há vagas em B e a fila de espera em B está vazia:
        esperar = 1; // espera = true
    else // senão:
        if (H->paciencia > (10 * tamanho)) // espera = (paciência de H) > (10 * tamanho da fila em B)
            esperar = 1;

    if (esperar){ // se espera:
        struct ev_t *espera = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento ESPERA (agora, H, B)
        if (!espera)
            return;
        espera->tempo = t;
        espera->heroi = H;
        espera->base = B;
        fprio_insere(LEF, espera, 1, espera->tempo); //melhor t ou espera->tempo?
    }else{ //senão:
        struct ev_t *desiste = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento DESISTE (agora, H, B)
        if (!desiste)
            return;
        desiste->tempo = t;
        desiste->heroi = H;
        desiste->base = B;
        fprio_insere(LEF, desiste, 2, desiste->tempo); //melhor t ou espera->tempo?
    }
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ",W->relogio, H->id, B->id, cjto_card(B->pres), B->max);
    if (esperar)
        printf("ESPERA\n");
    else
        printf("DESISTE\n");
}

void espera(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 1
{
    lista_insere(B->espera, H->id, -1); //adiciona H ao fim da fila de espera de B
    struct ev_t *avisa = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento AVISA (agora, B)
    if (!avisa)
        return;
    avisa->tempo = t;
    avisa->base = B;
    fprio_insere(LEF, avisa, 3, avisa->tempo);
    printf("%6d: ESPERA HEROI %2d BASE %d\n", W->relogio, H->id, B->id);
}

void desiste(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 2
{
    int dest = rand () % (N_BASES); // escolhe uma base destino D aleatória
    struct base_t *D = W->Bases[dest];
    struct ev_t *viaja = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento VIAJA (agora, H, D)
    if (!viaja)
        return;
    viaja->tempo = t;
    viaja->heroi = H;
    viaja->base = D;
    fprio_insere(LEF,viaja,6,viaja->tempo);
    printf("%6d: DESIST HEROI %2d BASE %d\n", W->relogio, H->id, B->id);
}

void avisa(int t, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 3
{
    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", W->relogio, B->id, cjto_card(B->pres), B->max);
    lista_imprime(B->pres);
    printf(" ]\n");
    while (((cjto_card(B->pres))<B->max) && (lista_tamanho(B->espera)>0))//enquanto houver vaga em B e houver heróis esperando na fila:
    {
        struct heroi_t *HN = malloc(sizeof(struct heroi_t));
        lista_retira(B->espera, &HN->id, 0);//retira primeiro herói (H') da fila de B
        cjto_insere(B->pres, HN->id); //adiciona H' ao conjunto de heróis presentes em B
        struct ev_t *entra = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento ENTRA (agora, H', B)
        if (!entra)
            return;
        entra->tempo = t;
        entra->heroi = HN;
        entra->base = B;
        fprio_insere(LEF,entra,4,entra->tempo); 
        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", W->relogio, B->id, HN->id);
    }
}

void entra(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 4
{
    int aleat = rand () % (20 - 1 + 1) + 1;
    int TPB = 15 + (H->paciencia * aleat); //calcula TPB = tempo de permanência na base: TPB = 15 + paciência de H * aleatório [1...20]
    struct ev_t *sai = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento SAI (agora + TPB, H, B)
    if (!sai)
        return;
    sai->tempo = t + TPB;
    sai->heroi = H;
    sai->base = B;
    fprio_insere(LEF,sai,5,sai->tempo);
    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d", W->relogio, H->id, B->id, sai->tempo);
}

void sai(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W) //tipo 5
{
    cjto_retira(B->pres, H->id); //retira H do conjunto de heróis presentes em B
    int dest = rand () % (N_BASES); // escolhe uma base destino D aleatória
    struct base_t *D = W->Bases[dest];
    struct ev_t *viaja = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento VIAJA (agora, H, D)
    if (!viaja)
        return;
    viaja->tempo = t;
    viaja->heroi = H;
    viaja->base = D;
    fprio_insere(LEF,viaja,6,viaja->tempo);
    struct ev_t *avisa = malloc(sizeof(struct ev_t));//cria e insere na LEF o evento AVISA (agora, B)
    if(!avisa)
        return;
    avisa->tempo = t;
    avisa->base = B; 
    fprio_insere(LEF,avisa,3,avisa->tempo);
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)", W->relogio, H->id, B->id, cjto_card(B->pres), B->max);
}

void viaja(int t, struct heroi_t *H, struct base_t *D, struct fprio_t *LEF, struct mundo_t *W) //tipo 6
{
    //calcula duração da viagem:
    struct base_t *B = W->Bases[H->base];
    int dist = (B->local->x * D->local->x) + (B->local->y * D->local->y);//distância = distância cartesiana entre a base atual de H e a base D 
    int durac = dist / H->veloc; //duração = distância / velocidade de H
    struct ev_t *chega = malloc(sizeof(struct ev_t));  //cria e insere na LEF o evento CHEGA (agora + duração, H, D)
    if (!chega)
        return;
    chega->tempo = t + durac;
    chega->heroi = H;
    chega->base = D;
    fprio_insere(LEF,chega,0,chega->tempo);
    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VELOC %d CHEGA %d\n", W->relogio, H->id, B->id, D->id, dist, H->veloc, chega->tempo);
}

void morre(int t, struct heroi_t *H, struct base_t *B, int M, struct fprio_t *LEF, struct mundo_t *W) //tipo 7
{
    cjto_retira(B->pres, H->id); //retira H do conjunto de heróis presentes em B
    H->status = 0; //muda o status de H para morto 
    struct ev_t *avisa = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento AVISA (agora, B)
    if (!avisa)
        return;
    avisa->tempo = t;
    avisa->base = B;
    fprio_insere(LEF,avisa,3,avisa->tempo);
    printf("%6d: MORRE HEROI %2d MISSAO %d", W->relogio, H->id, M);
}

int minimo_vetor(int dist[N_BASES], int a, int b){
    if (b == a)
        return a;
    int menor = minimo_vetor(dist, a, b-1);
    if (dist[b]<dist[menor])
            menor = b;
    return (menor);
}

void acha_BMP(int dist[N_BASES], int j, struct base_t *BMP, struct cjto_t *hab_rq, struct mundo_t *W)
{
    int i = minimo_vetor(dist, j, N_BASES); //i = acha a menor dist e retorna seu indice
    if (cjto_iguais(hab_rq, W->Bases[i]->habilidades)){ //pega a base mais proxima e verifica se tem as habilidades necessárias
        BMP=W->Bases[i];//se tem BMP = B;   
        return;
    }
    else //se não chama o algoritmo para o vetor começando da segunda posição acha_BMP(dist, i++, BMP)
        acha_BMP(dist, j++, BMP, hab_rq, W);
}

void missao(int t, struct ev_t *M, struct fprio_t *LEF, struct mundo_t *W) //tipo 8
{
    printf("Missão %d foi tentada", W->Missoes[M->mission->id]->id);
    int distancias[N_BASES]; //calcula a distância de cada base ao local da missão M
    struct base_t *B, *BMP;
    for (int i=0; i<N_BASES; i++){
        B = W->Bases[i];
        int dist = (M->mission->local->x * B->local->x) + (M->mission->local->y * B->local->y); //distância = distância cartesiana entre o local da missão e as bases do mundo 
        distancias[i] = dist; //a distancia está no indice correspondente a sua base
    }

    acha_BMP(distancias, 0, BMP, M->mission->hab, W); //encontra BMP = base mais próxima da missão cujos heróis possam cumpri-la 

    if (BMP){ //se houver uma BMP:
        printf("Missão cumprida!");
        BMP->nMission++;
        M->mission->status = 1; //marca a missão M como cumprida
        int H_pres = cjto_card(BMP->pres);
        int prese[H_pres];
        int vazio = 0;
        for (int i=0; i<N_HEROIS; i++){
            if(cjto_pertence(BMP->pres, W->Herois[i])){
                prese[vazio] = W->Herois[i]->id;
                vazio++;
            }
        }
        for(int i=0; i<H_pres; i++){ //para cada herói H presente na BMP:
            int risco = M->mission->perigo / (W->Herois[prese[i]]->paciencia + W->Herois[prese[i]]->xp + 1); //risco = perigo (M) / (paciência (H) + experiência (H) + 1.0) (float?)
            int aleat = rand () % (30);
            if (risco > aleat){ //se risco > aleatório (0, 30): (0 - 29 ou 0 - 30?)
                struct ev_t *morre = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento MORRE (agora, H)
                morre->tempo = t;
                morre->heroi = W->Herois[prese[i]];
                fprio_insere(LEF,morre,7,morre->tempo);
            }else //senão:
                W->Herois[prese[i]]->xp++; //incrementa a experiência de H
        }
    }else{ //senão:
        printf("Missão adiada");
        struct ev_t *mission = malloc(sizeof(struct ev_t)); //cria e insere na LEF o evento MISSAO (T + 24*60, M) para o dia seguinte
        mission->tempo = t + (24*60);
        fprio_insere(LEF,mission,8,mission->tempo);
    }
}

void fim(struct mundo_t *W, int event) //tipo 9
{
    int i;
    int cump,min,max,mort;
    printf("%d: FIM", T_FIM_DO_MUNDO);//encerra a simulação
    for (i=0; i<N_HEROIS; i++){ //apresenta estatísticas dos heróis
        struct heroi_t *H = W->Herois[i];
        printf("HEROI %2d ", H->id);
        if (H->status == 0)
            printf("MORTO ");
        else
            printf("VIVO ");
        printf("PAC %3d VEL %4d HABS [ ", H->paciencia, H->veloc);
        cjto_imprime(H->hab);
        printf(" ]\n");
    }
    for (i=0; i<N_BASES; i++){ //apresenta estatísticas das bases
        struct base_t *B = W->Bases[i];
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", B->id, B->max, lista_tamanho(B->espera), B->nMission);
    }
    printf("EVENTOS TRATADOS: %d\n", event); 
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", cump, N_MISSOES, cump/(N_MISSOES*1.0)); //apresenta estatísticas das missões
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max, min/(max*1.0));
    printf("TAXA MORTALIDADE: %.1f%%\n", mort);
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
    int habilidades;
    struct heroi_t *heroi = malloc (sizeof(struct heroi_t));
    if (!heroi)
        return;
    struct cjto_t *habi = malloc (sizeof(struct cjto_t));
    if (!habi){
        free(heroi);
        return;
    }
    
    habilidades = rand () % (3 - 1 + 1) + 1;
    
    heroi->id = i; //id = número sequencial [0...N_HEROIS-1]
    heroi->xp = 0; //experiência = 0
    heroi->status = 1; //heroi vivo
    heroi->paciencia = rand() % (101); //paciência = número aleatório [0...100]
    heroi->veloc = rand () % (5000 - 50 + 1) + 50; //velocidade  = número aleatório [50...5000] (em metros/minuto = 3 Km/h a 300 Km/h)
    habi = cjto_cria(10);
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

void inicia_bases(struct mundo_t *W) //criar cjto de habilidades
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
        base->local->x = rand() % (N_TAMANHO_MUNDO); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
        base->local->y = rand() % (N_TAMANHO_MUNDO); // ou usar W->tam_mundo
        base->max = rand() % (10 - 3 + 1) + 1; //lotação = número aleatório [3...10]
        heros = cjto_cria(base->max); //oberservar funcionamento
        base->pres = heros; // presentes = conjunto vazio (com capacidade para armazenar IDs de heróis até a lotação da base)
        esp = lista_cria();
        esp->tamanho = base->max;
        base->espera = esp;     
        W->Bases[i] = base;  
    }
    printf("Bases criadas\n");
}

void inicia_missoes(struct mundo_t *W)
{
    int i;
    for(i=0; i<N_MISSOES; i++){
        int habis;
        struct mission_t *missao = malloc(sizeof(struct mission_t));
        if (!missao)
            return;
        struct cjto_t *habi_rq = malloc(sizeof(struct cjto_t));
        if (!habi_rq){
            free(missao);
            return;
        }

        missao->id = i; //id = número sequencial [0...N_MISSOES-1]
        missao->local->x = rand() % (N_TAMANHO_MUNDO); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
        missao->local->y = rand() % (N_TAMANHO_MUNDO);
        habis = rand() % (10 - 6 + 1) + 1;
        habi_rq = cjto_cria(habis); //observar funcionamento
        int j;
        for (j=0; j<habis; j++){
            int h;
            h = rand () % (N_HABILIDADES);
            if (cjto_pertence(habi_rq, h))
                j--;
            else 
                cjto_insere(habi_rq,h);
        }
        missao->hab = habi_rq; //habilidades = conjunto com tamanho aleatório [6...10] de habilidades aleatórias
        missao->perigo = rand () % (101); //perigo      = número aleatório [0...100]
        missao->status = 0;
        W->Missoes[i] = missao;
    }
    printf("Criou missões\n");
}

void inicia_eventos(struct mundo_t *W, struct fprio_t *LEF)
{
    for(int i=0; i<N_HEROIS; i++){ //para cada herói H:
        struct heroi_t *H = W->Herois[i];
        int base = rand () % (N_BASES); //base  = número aleatório [0...N_BASES-1]
        int tempo = rand() % (4321);  //tempo = número aleatório [0...4320]  // 4320 = 60*24*3 = 3 dias
        struct ev_t *chega = malloc(sizeof(struct ev_t));  //criar e inserir na LEF o evento CHEGA (tempo, H, base)
        if (!chega)
            return;
        chega->tempo = tempo;
        chega->heroi = H;
        chega->base = W->Bases[base];
        fprio_insere(LEF,chega,0,chega->tempo); //qual melhor? chega->tempo ou tempo?
    }

    for(int i=0; i<N_MISSOES; i++){ //para cada missão M:
        int tempo = rand() % (T_FIM_DO_MUNDO); //tempo = número aleatório [0...T_FIM_DO_MUNDO]
        struct ev_t *mission = malloc(sizeof(struct ev_t)); // criar e inserir na LEF o evento MISSÃO (tempo, M)
        mission->tempo = tempo;
        fprio_insere(LEF,mission,8,mission->tempo); //qual melhor? chega->tempo ou tempo?
    }

    int tempo = T_FIM_DO_MUNDO; //tempo = T_FIM_DO_MUNDO
    struct ev_t *fim = malloc(sizeof(struct ev_t));
    fprio_insere(LEF,fim,9,tempo); //criar e inserir na LEF o evento FIM (tempo)
    printf("Eventos iniciados\n");
}

void destroi_mundo(struct mundo_t *W)
{
    if(!W)
        return;
    for (int i=0; i<N_HEROIS; i++){
        free(W->Herois[i]->hab);
        free(W->Herois[i]);
    }
    for (int i=0; i<N_BASES; i++){
        cjto_destroi(W->Bases[i]->pres);
        lista_destroi(W->Bases[i]->espera);
        free(W->Bases[i]);
    }
    for (int i=0; i<N_MISSOES; i++){
        cjto_destroi(W->Missoes[i]->hab);
        free(W->Missoes[i]);
    }
    free(W);
}

// programa principal
int main ()
{
    int tipo_ev,tempo,event;
    struct ev_t *ev;
    struct mundo_t *W = malloc(sizeof(struct mundo_t)); //iniciar as entidades e atributos do mundo
    if (!W)
        return -1;
    inicia_mundo(W);
    inicia_herois(W);
    inicia_bases(W);
    inicia_missoes(W);

    struct fprio_t *LEF = malloc(sizeof(struct fprio_t));
    if (!LEF){
        destroi_mundo(W);
        return -1;
    }
    LEF = fprio_cria(); // criar a lista de eventos futuros

    inicia_eventos(W,LEF); //criar os eventos iniciais

    while (W->relogio< T_FIM_DO_MUNDO){ //repetir até o fim da simulação //laço da simulação
        ev = fprio_retira(LEF, &tipo_ev, &tempo); //ev = 1º evento da lista de eventos futuros
        W->relogio = tempo; // relógio = tempo (ev)
        switch (tipo_ev) //caso tipo (ev) seja:
        {
            case 0:
                chega(tempo, ev->heroi, ev->base, LEF, W);
                event++;
                break;
            case 1:
                espera(tempo, ev->heroi, ev->base, LEF, W);
                event++;
                break;
            case 2:
                desiste(tempo, ev->heroi, ev->base, LEF, W);
                event++;
                break;
            case 3:
                avisa(tempo, ev->base, LEF, W);
                event++;
                break;
            case 4:
                entra(tempo, ev->heroi, ev->base, LEF, W);
                event++;
                break;
            case 5:
                sai(tempo, ev->heroi, ev->base, LEF, W);
                event++;
                break;
            case 6:
                viaja(tempo, ev->heroi, ev->base, LEF, W);
                event++;
                break;
            case 7:
                morre(tempo, ev->heroi, ev->base, ev->mission->id, LEF, W);
                event++;
                break;
            case 8:
                //missao(tempo, ev, LEF, W);
                if (ev->mission->status == 1)
                    event++;
                break;
            case 9:
                //fim(W,event); //estatisticas
                break;
        }
    }
    fprio_destroi(LEF);
    destroi_mundo(W);// destruir o mundo

  return (0) ;
}
