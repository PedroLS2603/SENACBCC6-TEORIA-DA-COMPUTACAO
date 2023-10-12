typedef struct Transicao {
  int origem;
  int destino;
  char leitura;
  char escrita;
  char movimento;
} Transicao;

typedef struct MT {
  char * alfabeto;
  Transicao * transicoes;
  int * estados;
  int num_estados;
  int num_transicoes;
} MT;
