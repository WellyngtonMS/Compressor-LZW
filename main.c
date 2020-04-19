#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <ctype.h>
#include "biblioteca.h"

int main(){
    setlocale(LC_ALL, "Portuguese");
    char nome[100];
    int opcao = 0;
    int tamanho = 0;
    char *bufferArquivo = '\0';
    int valor = 0, aux = 0, i = 0, tam = 0;

    printf("> Escolha uma opção\n> 1. compressor\n> 2. descompressor\n> ");
    scanf("%d", &opcao);
    if(opcao == 1){
        printf("> Digite o nome do arquivo com extensão (Ex.: arquivo.txt // arquivo.bin)\n> ");
        scanf("%s", nome);
        fflush(stdin);
        tamanho = calcularTamanhoArquivo(nome);

        printf("> O arquivo possui %d bytes\n", tamanho);
        printf("> Digite o valor de K em bits\n> ");
        scanf("%d", &valor);
        fflush(stdin);
        aux = valor;
        valor = pow(2, valor);

        FILE *output;
        char nomeArquivoBinario[30];
        snprintf(nomeArquivoBinario, 30, "arquivo_codificado_%d.bin", aux);
        output = fopen(nomeArquivoBinario, "wb");
        if (output <= 0){
            fclose(output);
            printf("Erro ao criar o arquivo %s!\n", nomeArquivoBinario);
            exit(1);
        }
        bufferArquivo = lerArquivo(nome, &tamanho);
        TDicionario alfabeto[valor];
        iniciarDicionario(alfabeto);
        codifica(alfabeto, bufferArquivo, valor, aux, output);

        salvaFim(output);
        fclose(output);
    }
    else if(opcao == 2){
        printf("> Digite o nome do arquivo com extensão (Ex.: arquivo.txt // arquivo.bin)\n> ");
        scanf("%s", nome);
        fflush(stdin);
        tamanho = calcularTamanhoArquivo(nome);
        tam = strlen(nome);
        while(i != tam){
            if(isdigit(nome[i])){
                aux = i;
            }
            i++;
        }
        aux = atoi(&nome[aux]);
        char nomeArquivo[30];
        snprintf(nomeArquivo, 30, "arquivo_decodificado_%d.txt", aux);
        FILE *arquivoEntrada = fopen(nome, "rb");
        if(!arquivoEntrada){
            fclose(arquivoEntrada);
            printf("Erro ao abrir arquivo binário!\n");
            exit(1);
        }
        vetor = malloc(tamanho * sizeof(char));
        fread(vetor, 1, tamanho, arquivoEntrada);
        printf("> O arquivo possui %d bytes\n", tamanho);
        valor = pow(2, aux);
        TDicionario alfabeto2[valor];
        iniciarDicionario(alfabeto2);
        decodifica(alfabeto2, nomeArquivo, aux);
    }
    else{
        printf("Opção inválida!\n");
    }
    return 0;
}
