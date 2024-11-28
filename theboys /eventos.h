#ifndef EVENTOS
#define EVENTOS

/*tipo 0*/
void chega(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 1*/
void espera(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 2*/
void desiste(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 3*/
void avisa(int t, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 4*/
void entra(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 5*/
void sai(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 6*/
void viaja(int t, struct heroi_t *H, struct base_t *D, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 7*/
void morre(int t, struct heroi_t *H, struct base_t *B, int M, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 8*/
void missao(int t, struct ev_t *M, struct fprio_t *LEF, struct mundo_t *W);

/*tipo 9*/
void fim(struct mundo_t *W, int event);
#endif
