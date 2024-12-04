#ifndef INICIA
#define INICIA

/*Inicializa todos os elementos da struct mundo*/
void inicia_mundo(struct mundo_t *W);

/*Inicializa todos os elementos de cada herois e 
  adiciona cada um ao vetor de herois da struct mundo*/
void inicia_herois(struct mundo_t *W);

/*Inicializa todos os elementos de cada base e 
  adiciona cada uma ao vetor de bases da struct mundo*/
void inicia_bases(struct mundo_t *W);

/*Inicializa todos os elementos de cada missão e 
  adiciona cada uma ao vetor de missões da struct mundo*/
void inicia_missoes(struct mundo_t *W);

/*Inicializa todos os eventos e adiciona 
  cada um a Lista de Eventos Futuros*/
void inicia_eventos(struct mundo_t *W, struct fprio_t *LEF);

/*Libera memória alocada em todas as inicializações*/
void destroi_mundo(struct mundo_t *W);

#endif
