#include <stdio.h>
#include <stdlib.h>
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

    img.matriz = (int **)malloc(img.alt * sizeof(int *));

    if (img.matriz == NULL) {
        printf("Erro ao alocar memória para as linhas da matriz\n");
        exit(EXIT_FAILURE);
    }

        for (int i = 0; i < img.alt; i++) {
            img.matriz[i] = (int *)malloc(img.larg * sizeof(int));
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


void escurecer(int parametro,Imagem imagem){
    for(int i=0;i<imagem.alt;i++){
        for(int j=0;j<imagem.larg;j++){
            imagem.matriz[i][j]= (imagem.matriz[i][j]-parametro<0)? 0:imagem.matriz[i][j]-parametro;
        }
    }

    salvarImagem(imagem,"lena256-escurecida.pgm");
}

void negativo(Imagem imagem){
    for(int i=0;i<imagem.alt;i++){
        for(int j=0;j<imagem.larg;j++){
            imagem.matriz[i][j]= 255 - imagem.matriz[i][j];
        }
    }

    salvarImagem(imagem,"lena256-negativo.pgm");
}

void clarearAD(int parametro, Imagem imagem) {
    for (int i = 0; i < imagem.alt; i++) {
        for (int j = 0; j < imagem.larg; j++) {
            imagem.matriz[i][j] = (imagem.matriz[i][j] + parametro > 255) ? 255 : imagem.matriz[i][j] + parametro;
        }
    }

    salvarImagem(imagem,"lena256-CAD.pgm");
}

void clarearMUL(int parametro,Imagem imagem){
    for(int i=0;i<imagem.alt;i++){
        for(int j=0;j<imagem.larg;j++){
            imagem.matriz[i][j]= (imagem.matriz[i][j]*parametro>255)? 255:imagem.matriz[i][j] * parametro;
        }
    }

    salvarImagem(imagem,"lena256-CMUL.pgm");
}


Imagem transporMatriz(Imagem imagem) {
    Imagem transposta = imagem;
    for (int i = 0; i < imagem.alt; i++) {
        for (int j = 0; j < imagem.larg; j++) {
            transposta.matriz[j][i] = imagem.matriz[i][j];
        }
    }
    transposta.alt = imagem.larg;
    transposta.larg = imagem.alt;
    return transposta;
}

Imagem reflexaoVertical(Imagem imagem) {
    Imagem Rvertical = imagem;
    for (int i = 0; i < imagem.alt; i++) {
        for (int j = 0; j < imagem.larg; j++) {
            Rvertical.matriz[i][j] = imagem.matriz[imagem.larg - 1 - i][j];
        }
    }

    return Rvertical;
}

Imagem reflexaoHorizontal(Imagem imagem) {
    Imagem RHorizontal = imagem;
    for (int i = 0; i < imagem.alt; i++) {
        for (int j = 0; j < imagem.larg; j++) {
            RHorizontal.matriz[i][j] = imagem.matriz[i][imagem.alt - 1 - j];
        }
    }

    return RHorizontal;
}


//0 para -90 e 1 para 90
void girar90(int parametro,Imagem imagem){
    Imagem resultado;
    if(parametro){
        resultado = transporMatriz(imagem);
        resultado = reflexaoVertical(resultado);
        salvarImagem(resultado,"lena256+90g.pgm");
    }
    else{
        resultado = transporMatriz(imagem);
        salvarImagem(resultado,"lena256-90g.pgm");
    }


}

void girar180(Imagem imagem){
    Imagem R = reflexaoHorizontal(imagem);
     salvarImagem(R,"lena256-180.pgm");
}

void liberarImagem(Imagem *imagem){
    for (int i = 0; i < imagem->alt; i++) {
        free(imagem->matriz[i]);
    }
    free(imagem->matriz);
}

void mostrarImagem(Imagem imagem){
    printf("%s\n",imagem.tipo);
    printf("%d\n",imagem.alt);
    printf("%d\n",imagem.larg);
    printf("%d\n",imagem.max);
    for (int i = 0; i < imagem.alt; i++) {
        for (int j = 0; j < imagem.larg; j++) {
            printf("%i ",imagem.matriz[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Imagem imagem = leitorImagem("lena256.pgm");
    //mostrarImagem(imagem);
    escurecer(80,imagem);
    clarearAD(80,imagem);
    clarearMUL(2,imagem);
    girar180(imagem);
    girar90(0,imagem);
    girar90(1,imagem);
    liberarImagem(&imagem);

    //(50, &copiaImagem[0]);
    //salvarImagem(copiaImagem[0], "lena256_escurecida.pgm");

    /** \brief
    clarearAD(30, &copiaImagem[1]);
    salvarImagem(copiaImagem[1], "lena256_clareada_soma.pgm");

    // Clareia a cópia da imagem usando multiplicação
    clarearMUL(2, &copiaImagem[2]);
    salvarImagem(copiaImagem[2], "lena256_clareada_multiplicacao.pgm");

    // Gera o negativo da cópia da imagem
    negativo(&copiaImagem[3]);
    salvarImagem(copiaImagem[3], "lena256_negativo.pgm");

    // Gira a cópia da imagem em 90 graus
    girar90(1, &copiaImagem[4]);
    salvarImagem(copiaImagem[4], "lena256_rotacao90.pgm");

    // Gira a cópia da imagem em 180 graus
    girar180(&copiaImagem[5]);
    salvarImagem(copiaImagem[5], "lena256_rotacao180.pgm");

    // Gira a cópia da imagem em 270 graus (ou -90)
    girar90(0, &copiaImagem[6]);
    salvarImagem(copiaImagem[6], "lena256_rotacao270.pgm");
     *
     */

    // Clareia a cópia da imagem usando soma



    return 0;
}
