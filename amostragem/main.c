#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct IMAGEM {
    char tipo[3]; // Aumente o tamanho para incluir o caractere nulo
    int alt;
    int larg;
    int max;
    int **matriz; // Altere o tipo da matriz para int**
} Imagem;

Imagem leitorImagem(const char *nome) {
    Imagem img;
    char linha[100];

    FILE *arquivo = fopen(nome, "r");
    if(arquivo==NULL){
        printf("Erro ao abrir\n");
        exit(1);
    }

    fgets(linha, 100, arquivo);
    sscanf(linha, "%s", img.tipo);

    // Descartar uma linha, se necessário
    fgets(linha, 100, arquivo);

    // Ler e analisar os dados de altura e largura
    fgets(linha, 100, arquivo);
    sscanf(linha, "%d %d", &img.alt, &img.larg);

    // Ler e analisar o valor máximo
    fgets(linha, 100, arquivo);
    sscanf(linha, "%d", &img.max);

    img.matriz = malloc(img.alt * sizeof(*img.matriz));

    if (img.matriz == NULL) {
        printf("Erro ao alocar memória para as linhas da matriz\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < img.alt; i++) {
        img.matriz[i] = malloc(img.larg * sizeof(int));
        if (img.matriz[i] == NULL) {
            printf("Erro ao alocar memória para as colunas da matriz\n");

            for (int j = 0; j < i; j++) {
                free(img.matriz[j]);
            }
            free(img.matriz);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < img.alt; i++) {
        for (int j = 0; j < img.larg; j++) {
            fgets(linha,100,arquivo);
            sscanf(linha, "%i", &img.matriz[i][j]);
        }
    }

    fclose(arquivo);

    return img;
}

void salvarImagem(Imagem imagem, const char *nome) {
    FILE *arquivo = fopen(nome, "w");
    fprintf(arquivo, "%s\n", imagem.tipo);
    fprintf(arquivo, "%d %d\n", imagem.alt, imagem.larg);
    fprintf(arquivo, "%d\n", imagem.max);
    for (int i = 0; i < imagem.alt; i++) {
        for (int j = 0; j < imagem.larg; j++) {
            fprintf(arquivo, "%d\n", imagem.matriz[i][j]);
        }
    }

    fclose(arquivo);
}

void amostragem4x(Imagem imagem) {
    Imagem resultado;
    resultado.alt = 2 * imagem.alt;
    resultado.larg = 2 * imagem.larg;
    resultado.max = imagem.max; // Precisamos manter o valor máximo

    resultado.matriz = (int **)malloc(resultado.alt * sizeof(int *));
    if (resultado.matriz == NULL) {
        printf("Erro ao alocar memória para as linhas da matriz resultado\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < resultado.alt; i++) {
        resultado.matriz[i] = (int *)malloc(resultado.larg * sizeof(int));
        if (resultado.matriz[i] == NULL) {
            printf("Erro ao alocar memória para as colunas da matriz resultado\n");

            for (int j = 0; j < i; j++) {
                free(resultado.matriz[j]);
            }
            free(resultado.matriz);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < imagem.alt; i++) {
        for (int j = 0; j < imagem.larg; j++) {
            resultado.matriz[i * 2][j * 2] = resultado.matriz[i * 2][j * 2 + 1] = resultado.matriz[i * 2 + 1][j * 2] = resultado.matriz[i * 2 + 1][j * 2 + 1] = imagem.matriz[i][j];
        }
    }

    salvarImagem(resultado, "lena-512.pgm");

    for (int i = 0; i < resultado.alt; i++) {
        free(resultado.matriz[i]);
    }
    free(resultado.matriz);
}

void amostragem(Imagem imagem) {
    Imagem resultado;
    resultado.alt = imagem.alt/2;
    resultado.larg = imagem.larg/2;
    resultado.max = imagem.max; // Precisamos manter o valor máximo

    resultado.matriz = (int **)malloc(resultado.alt * sizeof(int *));
    if (resultado.matriz == NULL) {
        printf("Erro ao alocar memória para as linhas da matriz resultado\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < resultado.alt; i++) {
        resultado.matriz[i] = (int *)malloc(resultado.larg * sizeof(int));
        if (resultado.matriz[i] == NULL) {
            printf("Erro ao alocar memória para as colunas da matriz resultado\n");

            for (int j = 0; j < i; j++) {
                free(resultado.matriz[j]);
            }
            free(resultado.matriz);
            exit(EXIT_FAILURE);
        }
    }
    int media=0;
    for (int i = 0; i < resultado.alt; i++) {
        for (int j = 0; j < resultado.larg; j++) {
            media = imagem.matriz[i * 2][j * 2] + imagem.matriz[i * 2][j * 2 + 1] + imagem.matriz[i * 2 + 1][j * 2] + imagem.matriz[i * 2 + 1][j * 2 + 1];
            resultado.matriz[i][j] = media/4;
        }
    }

    salvarImagem(resultado, "lena128.pgm");

    for (int i = 0; i < resultado.alt; i++) {
        free(resultado.matriz[i]);
    }
    free(resultado.matriz);
}

int main()
{
    Imagem imagem = leitorImagem("lena256.pgm");
    amostragem4x(imagem);
    free(imagem.matriz);
    return 0;
}
