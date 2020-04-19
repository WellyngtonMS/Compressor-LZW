#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "biblioteca.h"

int pos = 0;

int calcularTamanhoArquivo(char *nomeArquivo){
    FILE *arquivo;

    if((arquivo = fopen(nomeArquivo, "rb")) != NULL){
        long posicaoAtual = ftell(arquivo);
        int tamanho;
        fseek(arquivo, 0, SEEK_END);
        tamanho = ftell(arquivo);
        fseek(arquivo, posicaoAtual, SEEK_SET);
        return tamanho;
    }
    else{
        printf("Arquivo inexistente!\n");
        exit(1);
    }
    fclose(arquivo);
}

void addBits(int value, int qBits, FILE *nomeArquivoBinario){
    int byte = pos / 8;
    int shift = pos % 8;
    int disp = 8 - shift;
    unsigned int mask = 0;
    int falta = qBits - disp;
    int falta2 = 0;
    int i;
    if(falta > 8){
        falta2 = falta - 8;
        falta = 8;
    }

    mask = 0;
    for (i = 0; i < disp; i++){
        mask = mask | (1 << i);
    }
    bufferBits[byte] = (bufferBits[byte] & (~mask & 0xFF)) | ((value >> (falta + falta2)) & mask);

    if(falta > 0){
        mask = 0;
        for(i = 0; i < (8 - falta); i++){
            mask = mask | (1 << i);
        }
        if(falta2 == 0){
            bufferBits[byte + 1] = (bufferBits[byte + 1] & mask) | ( (value << (8 - falta)) & (~mask & 0xFF));
        }
        else{
            bufferBits[byte + 1] = ((value >> falta2) & 0xFF);
        }
    }

    if(falta2 > 0){
        mask = 0;
        for (i = 0; i < (8 - falta2); i++){
            mask = mask | (1 << i);
        }
        bufferBits[byte + 2] = (bufferBits[byte + 2] & mask) | ((value << (8 - falta2)) & (~mask & 0xFF));
    }

    pos += qBits;
    byte = pos / 8;
    if(byte >= SIZE - 3){
        fwrite(bufferBits, 1, byte, nomeArquivoBinario);
        pos -= byte * 8;
        shift = pos % 8;
        for(i = 0; i < (8-shift); i++){
            mask = mask | (1 << i);
        }
        bufferBits[0] = bufferBits[byte] & ~mask;
    }
}

int getBits(int qBits){
    int value = 0;
    int byte = pos / 8;
    int shift = pos % 8;
    int disp = 8 - shift;
    unsigned int mask = 0;
    int falta = qBits - disp;
    int falta2 = 0;
    int i;
    if(falta > 8){
        falta2 = falta - 8;
        falta = 8;
    }

    mask = 0;
    for(i = 0; i < disp; i++){
        mask = mask | (1 << i);
    }
    value = (vetor[byte] & mask) << (falta + falta2);

    if(falta > 0){
        if(falta2 == 0){
            value = value | ((vetor[byte + 1]) >> (8 - falta));
        }
        else{
            value = value | (vetor[byte + 1] << falta2);
        }
    }

    if(falta2 > 0){
        value = value | (vetor[byte + 2] >> (8 - falta2));
    }
    pos += qBits;

    return value;
}

void salvaFim(FILE *nomeArquivoBinario){
    int byte = pos / 8;
    int shift = pos % 8;
    if(shift > 0){
        byte++;
    }
    if(byte > 0){
        fwrite(bufferBits, 1, byte, nomeArquivoBinario);
    }
    fclose(nomeArquivoBinario);
}

char* lerArquivo(char *nomeArquivo, int *tamanho){
    FILE *arquivo;
    char *buffer = (char *)malloc(*tamanho+1 *sizeof(char));
    buffer[*tamanho] = '\0';
    if((arquivo = fopen(nomeArquivo, "rb")) != NULL){
        if(buffer == NULL) {
            printf("Erro ao tentar alocar o buffer de memória.\n");
        }
        size_t elementos_lidos = fread(buffer, sizeof(char), *tamanho, arquivo);
        if(elementos_lidos != *tamanho){
            printf("Erro ao tentar ler o arquivo inteiro.\n");
        }
    }
    else{
        printf("Arquivo inexistente!\n");
        exit(1);
    }
    fclose(arquivo);
    return buffer;
}

void iniciarDicionario(TDicionario *exemplar){
    int i;
    for(i = 0; i < 256; i++){
        exemplar[i].indice = i;
        exemplar[i].palavra = malloc(2);
        exemplar[i].palavra[0] = (char)i;
        exemplar[i].palavra[1] = '\0';
    }
}

int procura(TDicionario *auxiliar, int tamanhoDicionario, char *palavra){
    int i;
    for(i = 0; i < tamanhoDicionario; i++){
        if(strcmp(auxiliar[i].palavra, palavra) == 0){
            return i;
        }
    }
    return -1;
}

int procuraNumero(TDicionario *auxiliar, int tamanhoDicionario, int numero){
    int i;
    for(i = 0; i < tamanhoDicionario; i++){
        if(auxiliar[i].indice == numero){
            return i;
        }
    }
    return -1;
}

void codifica(TDicionario *exemplar, char *buffer, int valor, int k, FILE *nomeArquivoBinario){
    int tamanhoDicionario = 256, i = 0, j, aux;
    char *stringTemporaria = "";
    char *stringTemporaria2;

    printf("Comprimindo arquivo...\n");

    while(buffer[i] != '\0'){
        int auxiliar = strlen(stringTemporaria);
        stringTemporaria2 = malloc(auxiliar+2);

        for(j = 0; j < auxiliar; j++){
            stringTemporaria2[j] = stringTemporaria[j];
        }
        stringTemporaria2[auxiliar] = buffer[i];
        stringTemporaria2[auxiliar+1] = '\0';

        aux = procura(exemplar, tamanhoDicionario, stringTemporaria2);
        if(aux != -1){
            stringTemporaria = stringTemporaria2;
        }
        else{
            aux = procura(exemplar, tamanhoDicionario, stringTemporaria);
            if(tamanhoDicionario < valor){
                exemplar[tamanhoDicionario].palavra = stringTemporaria2;
                addBits(aux, k, nomeArquivoBinario);
                tamanhoDicionario++;
                free(stringTemporaria);
                stringTemporaria = malloc(2);
                stringTemporaria[0] = buffer[i];
                stringTemporaria[1] = '\0';
            }
            else{
                addBits(aux, k, nomeArquivoBinario);
                free(stringTemporaria);
                stringTemporaria = malloc(2);
                stringTemporaria[0] = buffer[i];
                stringTemporaria[1] = '\0';
            }
        }
        i++;
    }

    if(strcmp(stringTemporaria, "") != 0){
        int aux1 = procura(exemplar, tamanhoDicionario, stringTemporaria);
        if(tamanhoDicionario < valor){
            exemplar[tamanhoDicionario].palavra = stringTemporaria2;
            addBits(aux1, k, nomeArquivoBinario);
            tamanhoDicionario++;
        }
        else{
            addBits(aux1, k, nomeArquivoBinario);
        }
    }
	printf("Sucesso na tarefa de compactação!\n");
}

void decodifica(TDicionario *exemplar, char *nomeArquivoSaida, int k){
    int tamanhoDicionario = 256, max = 0;
    max = pow(2, k);
    FILE *arquivoSaida = fopen(nomeArquivoSaida, "wb");
    if (arquivoSaida <= 0){
        fclose(arquivoSaida);
        printf("Erro ao criar o arquivo %s!\n", nomeArquivoSaida);
        exit(1);
    }
    printf("Descomprimindo arquivo...\n");

    int valor = 0, i = 0, z = 0, j = 0;
    valor = getBits(k);

    char *stringTemporaria = malloc(2);
    stringTemporaria[0] = (char)valor;
    stringTemporaria[1] = '\0';

    fwrite(&valor, sizeof(char), 1, arquivoSaida);

    while(valor = getBits(k)){
        char *entrada;
        int aux = procuraNumero(exemplar, tamanhoDicionario, valor);

        if(aux != -1){
            entrada = exemplar[aux].palavra;
        }
        else{
            if(valor == tamanhoDicionario){
                entrada = malloc(strlen(stringTemporaria)+2);
                for(i = 0, z = strlen(stringTemporaria); i < z; i++){
                    entrada[i] = stringTemporaria[i];
                }
                entrada[strlen(stringTemporaria)] = stringTemporaria[0];
                entrada[strlen(stringTemporaria)+1] = '\0';
            }
            else{
                printf("Erro! Arquivo descomprimido incorretamente.\n");
                exit(1);
            }
        }

        for(i = 0, j = strlen(entrada); i < j; i++){
            fwrite(&entrada[i], sizeof(char), 1, arquivoSaida);
        }

        char *auxiliar = malloc(strlen(stringTemporaria)+2);

        for(i = 0, j = strlen(stringTemporaria); i < j; i++){
            auxiliar[i] = stringTemporaria[i];
        }
        auxiliar[strlen(stringTemporaria)] = entrada[0];
        auxiliar[strlen(stringTemporaria)+1] = '\0';

        if(tamanhoDicionario < max){
            exemplar[tamanhoDicionario].indice = tamanhoDicionario;
            exemplar[tamanhoDicionario].palavra = auxiliar;
            tamanhoDicionario++;
            stringTemporaria = entrada;
        }
        else{
            tamanhoDicionario++;
            stringTemporaria = entrada;
        }
    }

    fclose(arquivoSaida);
    printf("Sucesso na tarefa de descompactação!\n");
}
