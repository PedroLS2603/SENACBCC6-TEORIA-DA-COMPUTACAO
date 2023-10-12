#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.c"
#include "maquina.h"

bool reconhece(MT maquina, char * fita) { 
  unsigned int cabecote = 0;

  int estado_atual = maquina.estados[0];
  bool fita_aceita = false;

  while (true) {
    char leitura = fita[cabecote];
    bool quebrar_laco = true;
    
    if(estado_atual == maquina.estados[maquina.num_estados - 1]) {
        fita_aceita = true;
        break;
    }
    
    for(int i = 0; i < maquina.num_transicoes; i++) {
      Transicao transicao = maquina.transicoes[i];
      
      if(transicao.origem == estado_atual && transicao.leitura == leitura) {
        estado_atual = transicao.destino;
        fita[cabecote] = transicao.escrita;
        if(transicao.movimento == 'E') {
          cabecote -= 1;
        }
        else {
          cabecote += 1;
        }
        quebrar_laco = false;
        break;
      } 
    }

    if(quebrar_laco) {
      break;
    }
  }

  return fita_aceita;
}

MT configurar_maquina(char * caminho_arquivo) {
  int count_linha = 0;
  unsigned int tamanho_buffer = 100;

  FILE * arquivo = fopen(caminho_arquivo, "r");
  char buffer[tamanho_buffer];
  MT maquina;
  char * testes;

  printf("Configurando máquina...\n");
  while (fgets(buffer, tamanho_buffer, arquivo)){
    int num_transicoes;
    int num_testes;
    // Obtendo os caracteres do alfabeto
    if(count_linha == 0) {
      unsigned int tamanho_alfabeto = strlen(buffer);
      maquina.alfabeto = malloc(tamanho_alfabeto * sizeof(char));
      for(int i = 0; i < tamanho_alfabeto; i++) {
        maquina.alfabeto[i] = buffer[i];
      }
      printf("Alfabeto: %s", maquina.alfabeto);
    }

    // Configurando espaço de estados
    if(count_linha == 1) {
      int num_estados = atoi(buffer); // https://www.educative.io/answers/how-to-convert-a-string-to-an-integer-in-c
      maquina.num_estados = num_estados;
      maquina.estados = malloc(num_estados * sizeof(int));
      printf("Estados: ");
      for(int i = 0; i < num_estados; i++) {
        maquina.estados[i] = i+1;
        printf("[%d] ", maquina.estados[i]);
      }
    }

    // Configurando conjunto de transições

    if(count_linha == 2){
      num_transicoes = atoi(buffer);
      maquina.transicoes = malloc(num_transicoes * sizeof(Transicao));
      maquina.num_transicoes = num_transicoes;
      printf("\nTransições: \n");
      for(int i = 0; i < num_transicoes; i++) {
        fgets(buffer, tamanho_buffer, arquivo);
        Transicao transicao;

        
        transicao.origem = buffer[0] - '0'; // https://stackoverflow.com/questions/628761/convert-a-character-digit-to-the-corresponding-integer-in-c
        if(buffer[2] == '-') {
          transicao.leitura = '\n';
          
        } else {
          transicao.leitura = buffer[2];
        }

        if(buffer[4] == '-') {
          transicao.escrita = '\n';
          
        } else {
          transicao.escrita = buffer[4];
        }
        transicao.movimento = buffer[6];
        transicao.destino = buffer[8] - '0';
        printf("(%d, %c) -> (%c, %c, %d)\n", buffer[0] - '0', buffer[2], buffer[4], buffer[6], buffer[8] - '0');
        maquina.transicoes[i] = transicao;
      }

      
      count_linha += num_transicoes - 1;
    }


    // Separando cenário de testes
    if(count_linha == 2 + num_transicoes) {
      num_testes = atoi(buffer);
      testes = malloc(num_testes * sizeof(char[100]));
      printf("\n%d strings a serem testadas\n", num_testes);
      for(int i = 0; i < num_testes; i++) {
        fgets(buffer, tamanho_buffer, arquivo);
        char fita[100];
        strcpy(fita,buffer);
        remove_quebra(fita);
        
        printf("\n%d: %s -> %s\n", i + 1, fita,(reconhece(maquina, buffer) ? "OK" : "NOT OK"));
      }
    }

    count_linha += 1; 
  }
  fclose(arquivo);


  return maquina;
}