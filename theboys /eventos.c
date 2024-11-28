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
