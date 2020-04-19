#include <stdio.h>
#include <stdlib.h>
#define SIZE 4095

unsigned char bufferBits[SIZE];
unsigned char *vetor;

typedef struct dicionario{
    char *palavra;
    int indice;
}TDicionario;

int calcularTamanhoArquivo(char *nomeArquivo);
void addBits(int value, int qBits, FILE *nomeArquivoBinario);
int getBits(int qBits);
void salvaFim(FILE *nomeArquivoBinario);
char* lerArquivo(char *nomeArquivo, int *tamanho);
void iniciarDicionario(TDicionario *exemplar);
int procura(TDicionario *auxiliar, int tamanhoDicionario, char *palavra);
int procuraNumero(TDicionario *auxiliar, int tamanhoDicionario, int numero);
void codifica(TDicionario *exemplar, char *buffer, int valor, int k, FILE *nomeArquivoBinario);
void decodifica(TDicionario *exemplar, char *nomeArquivoSaida, int k);
