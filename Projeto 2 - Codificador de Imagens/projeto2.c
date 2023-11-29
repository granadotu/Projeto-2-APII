/*---------------------------------------------------------------------------------\
|                                                                                  |
|        Integrantes:                        TIA:                                  |
|        Artur Granado Novaes;               32336901                              |       
|        Marcelo Luis Simone Lucas;          32340370                              |
|                                                                                  |
|        Link do GitHub: https://github.com/granadotu/Projeto-2-APII               |
|                                                                                  |
\---------------------------------------------------------------------------------*/     

// Implementacao das bibliotecas que serao usadas no codigo
#include <stdio.h>
#include <stdlib.h>

#define linhaMAX 768
#define colunaMAX 1024


// Funcao que visualiza se a entrada eh toda preta 'P' ou toda branca 'B'
int igual(int linha_inicio, int linha_fim, int coluna_inicio, int coluna_fim, int **matriz) {
  int esp = matriz[linha_inicio][coluna_inicio];
  for (int i = linha_inicio; i < linha_fim; i++) {
    for (int j = coluna_inicio; j < coluna_fim; j++) {
      if (matriz[i][j] != esp) {
        return 0;
      }
    }
  }
  return 1;
}

// Funcao que faz o corte da imagem e retorna o codigo
void corte(int linha_inicio, int linha_fim, int coluna_inicio, int coluna_fim, int **matriz) {
  if (linha_inicio >= linha_fim || coluna_inicio >= coluna_fim) {
    return;
  } else {
    if (igual(linha_inicio, linha_fim, coluna_inicio, coluna_fim, matriz)) {
      int cor = matriz[linha_inicio][coluna_inicio];
      // Se a imagem for igual a 0, imprime 'B'
      if (cor == 0) {
        printf("B");
      } else if (cor == 1) {
        // Se a imagem for toda igual a 1, imprime 'P'
        printf("P");
      }
    } else {
      // Se nao for tudo 0 ou 1, 'corta', imprimindo um 'X'
      printf("X");

      // Corta a imagem binária pela metade, tanto horizontalmente quanto verticalmente
      int linha_metade = (linha_inicio + linha_fim + 1) / 2;
      int coluna_metade = (coluna_inicio + coluna_fim + 1) / 2;

      // 1 Quadrante
      corte(linha_inicio, linha_metade, coluna_inicio, coluna_metade, matriz);

      // 2 Quadrante
      corte(linha_inicio, linha_metade, coluna_metade, coluna_fim, matriz);

      // 3 Quadrante 
      if (linha_metade < linha_fim) {
        corte(linha_metade, linha_fim, coluna_inicio, coluna_metade, matriz);
      }

      // 4 Quadrante
      if (coluna_metade < coluna_fim) {
        corte(linha_metade, linha_fim, coluna_metade, coluna_fim, matriz);
      }
    }
  }
}

// Funcao para entrada manual [ -m ]
void manual() {
  int linha, coluna;

  printf("\nDigite o numero de linhas: ");
  scanf("%d", &linha);
  printf("Digite o numero de colunas: ");
  scanf("%d", &coluna);

  if (linha > linhaMAX || coluna > colunaMAX) {
    printf("Valores fornecidos excedem o maximo de linhas e colunas permitidas.\n");
    return;
  }
  
  // Aloca uma matriz para armazenar a imagem
  int **matriz = (int **)malloc(linha * sizeof(int *));
  for(int i = 0; i < linha; i++) {
    matriz[i] = (int *)malloc(coluna * sizeof(int));
  }

  // Imprime as posicoes da matriz para se fazer a imagem binaria e pede o valor de cada posicao
  printf("Informe a imagem binaria na matriz abaixo:\n\n");

  for (int i = 0; i < linha; i++) {
    for (int j = 0; j < coluna; j++) {
      printf("[%d][%d]: ", i, j);
      scanf("%d", &matriz[i][j]);
    }
  }

  // Imprime a imagem binária que o usuário fez
  printf("\nImagem binaria escolhida:\n\n");

  for (int i = 0; i < linha; i++) {
    for (int j = 0; j < coluna; j++) {
      printf("%d   ", matriz[i][j]);
    }
    printf("\n\n");
  }

  // Chama a funcao corte
  corte(0, linha, 0, coluna, matriz);
  printf("\n");
} 


// Funcao para entrada arquivo [ -f ]
void arquivo(char *nome_arquivo) {
  int coluna, linha;

  // Abre o arquivo em modo de leitura
  FILE *arq = fopen(nome_arquivo, "r");
  if (arq == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  // Le as primeiras duas linhas do arquivo
  char buffer[100];
  fgets(buffer, sizeof(buffer), arq);

  // Obtem as dimensoes da imagem
  if (fscanf(arq, "%d %d", &linha, &coluna) != 2 || linha <= 0 || coluna <= 0) {
    printf("Erro ao ler as dimensoes da imagem do arquivo.\n");
    fclose(arq);
    return;
  }

  // Verifica se as dimensoes sao validas
  if (linha > linhaMAX || coluna > colunaMAX) {
    printf("As dimensoes fornecidas excedem o maximo permitido.\n");
    fclose(arq);
    return;
  }

  // Aloca uma matriz para armazenar a imagem
  int **matriz = (int **)malloc(linha * sizeof(int *));
  for(int i = 0; i < linha; i++) {
    matriz[i] = (int *)malloc(coluna * sizeof(int));
  }

  // Le os dados da imagem do arquivo
  for (int i = 0; i < linha; i++) {
    for (int j = 0; j < coluna; j++) {
      int valor;
      if (fscanf(arq, "%d", &valor) != 1) {
        printf("Erro ao ler valor na posicao [%d][%d].\n", i, j);
        fclose(arq);
        return;
      }
      matriz[i][j] = valor;
    }
  }

  // Fecha o arquivo
  fclose(arq);

  // Mostra a imagem binaria
  printf("\nImagem binaria:\n\n");
  for (int i = 0; i < linha; i++) {
    for (int j = 0; j < coluna; j++) {
      printf("%d   ", matriz[i][j]);

    }
    printf("\n\n");
  }

  // Chama a funcao corte
  corte(0, linha, 0, coluna, matriz);
  printf("\n\n");
}


// Funcao de ajuda [ -? ]
void ajuda() {
  printf("Uso: ImageEncoder [ -? | -m | -e ]\n");
  printf("Codifica imagens binarias dadas em formato de matriz.\n");
  printf("Argumentos:\n\n");
  printf("-?, --help: Apresenta essa orientacao na tela.\n\n");
  printf("-m, --manual: Ativa o modo de entrada manual.\n\n");
  printf("-f, --file: Considera a imagem representada no arquivo PBM (Portable bitmap).\n\n");  
  printf("-e, --encerrar: Encerra o programa.\n\n");
}

// Funcao principal
int main() {
  char escolha;
  char nome_arq[100];

  // Loop para a escolha das opcoes
  do {
    printf("Escolha uma opcao:\n");
    printf("[ -? | -m | -f | -e ]\n");
    scanf(" %c", &escolha);

    // Switch para a escolha dos 'casos' (opcoes)
    switch (escolha) {
    case 'm':
      manual();
      break;
    case 'f':
      printf("\nDigite o nome do arquivo: ");
      scanf("%s", nome_arq);
      arquivo(nome_arq);
      break;

    case '?':
      ajuda();
      break;
    // Funcao para o encerramento do programa [ -e ]
    case 'e':
      printf("Encerrando o programa.\n");
      break;
    default:
      printf("Opcao invalida. Tente novamente.\n\n");
    }
  } while (escolha != 'e');

  return 0;
}
