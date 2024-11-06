#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRUTAS 1

// Struct
typedef struct {
  int codigo;
  char nome[20];
  int quantidade;
  float preco;
} Fruta;

// Funções
void adicionarFruta(Fruta estoque[], int *contador, int *codigoAtual);
void excluirFruta(Fruta estoque[], int *contador);
void venderFruta(Fruta estoque[], int contador);
void exibirEstoque(Fruta estoque[], int contador);
void relatorioVendas();
void atualizarFruta(Fruta estoque[], int contador);
void buscarFruta(Fruta estoque[], int contador);
int verificarDuplicidade(Fruta estoque[], int contador, char nome[]);

int main() {
  Fruta *estoque = malloc(MAX_FRUTAS * sizeof(Fruta));
  if (estoque == NULL) {
      printf("Erro na alocação de memória!\n");
      return 1;
  }
  // Fruta estoque[MAX_FRUTAS];
  int contador = 0;
  int codigoAtual = 1;
  int opcao;

  do {
    printf("\n--- Banca de Frutas - Opções: ---\n\n");
    printf("1. Adicionar fruta\n");
    printf("2. Lista de frutas\n");
    printf("3. Buscar fruta\n");
    printf("4. Atualizar fruta\n");
    printf("5. Excluir fruta\n");
    printf("6. Vender fruta\n");
    printf("7. Relatório de vendas\n"); 
    printf("8. Sair\n\n");
    
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    printf("\n");
    
    switch (opcao) {
    case 1:
      adicionarFruta(estoque, &contador, &codigoAtual);
      break;
    case 2:
      exibirEstoque(estoque, contador);
      break;
    case 3:
      buscarFruta(estoque, contador);
      break;
    case 4:
      atualizarFruta(estoque, contador);
      break;
    case 5:
      excluirFruta(estoque, &contador);
      break;
    case 6:
      venderFruta(estoque, contador);
      break;
    case 7:
      relatorioVendas();
      break;
    case 8:
      printf("Saindo...\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (opcao != 9);

  free(estoque);
  return 0;
}

// Conversos para minúscula
void toLower(char str[]) {
     for(int i = 0; str[i]; i++){
         str[i] = tolower(str[i]);
     }
}

// Verifica se tem fruta repetida
int verificarDuplicidade(Fruta estoque[], int contador, char nome[]) {
  toLower(nome);

  for (int i = 0; i < contador; i++) {
    char nomeEstoque[20];
    strcpy(nomeEstoque, estoque[i].nome);
    toLower(nomeEstoque);

    if (strcmp(nomeEstoque, nome) == 0) {
      return 1;
    }
  }
  return 0;
}

// Adicionar
void adicionarFruta(Fruta estoque[], int *contador, int *codigoAtual) {
  if (*contador < MAX_FRUTAS) {
    char nomeTemp[20];
    printf("Nome da fruta: ");
    scanf("%s", nomeTemp);
    
    // Verificar se já existe no estoque
    if (verificarDuplicidade(estoque, *contador, nomeTemp)) {
      printf("Erro: Fruta com esse nome já cadastrada!\n");
      return;
    }

    estoque[*contador].codigo = (*codigoAtual)++;
    strcpy(estoque[*contador].nome, nomeTemp);
    printf("Quantidade: ");
    scanf("%d", &estoque[*contador].quantidade);
    printf("Preço por unidade: ");
    scanf("%f", &estoque[*contador].preco);
    (*contador)++;
    printf("Fruta adicionada ao estoque com código %d!\n",
           estoque[*contador - 1].codigo);
  } else {
    printf("Estoque cheio!\n");
  }
}

// Vender Fruta
void venderFruta(Fruta estoque[], int contador) {
  char nome[20];
  int quantidade;
  printf("Nome da fruta para venda: ");
  scanf("%s", nome);
  toLower(nome);

  printf("Quantidade a vender: ");
  scanf("%d", &quantidade);

  for (int i = 0; i < contador; i++) {
    char nomeEstoque[20];
    strcpy(nomeEstoque, estoque[i].nome);
    toLower(nomeEstoque);

    if (strcmp(nomeEstoque, nome) == 0) {
      if (estoque[i].quantidade >= quantidade) {
        estoque[i].quantidade -= quantidade;

        // Registra a venda em um arquivo
        FILE *arquivo = fopen("vendas.txt", "a");
        if (arquivo == NULL) {
          printf("Erro ao abrir o arquivo para gravação.\n");
          exit(1);
        }

        fprintf(arquivo,
                "Venda: %s | Quantidade: %d | Preço: %.2f | Total: %.2f\n",
                estoque[i].nome, quantidade, estoque[i].preco,
                quantidade * estoque[i].preco);
        fclose(arquivo);
        printf("Venda realizada com sucesso!\n");
        return;
      } else {
        printf("Quantidade insuficiente no estoque.\n");
        return;
      }
    }
  }
  printf("Fruta não encontrada.\n");
}

// Exibir Frutas
void exibirEstoque(Fruta estoque[], int contador) {
  printf("\n--- Estoque de Frutas ---\n");
  for (int i = 0; i < contador; i++) {
    printf("Código: %d | Fruta: %s | Quantidade: %d | Preço: %.2f\n",
           estoque[i].codigo, estoque[i].nome, estoque[i].quantidade,
           estoque[i].preco);
  }
}

// Atualizar fruta
void atualizarFruta(Fruta estoque[], int contador) {
  char nome[20];
  printf("Nome da fruta para atualizar: ");
  scanf("%s", nome);

  toLower(nome);

  for (int i = 0; i < contador; i++) {
    char nomeEstoque[20];
    strcpy(nomeEstoque, estoque[i].nome);
    toLower(nomeEstoque);

    if (strcmp(nomeEstoque, nome) == 0) {
      printf("Nova quantidade: ");
      scanf("%d", &estoque[i].quantidade);
      printf("Novo preço por unidade: ");
      scanf("%f", &estoque[i].preco);
      printf("Informações da fruta atualizadas com sucesso!\n");
      return;
    }
  }
  printf("Fruta não encontrada.\n");
}

// Excluir
void excluirFruta(Fruta estoque[], int *contador) {
  char nome[20];
  printf("Digite o nome da fruta a ser excluída: ");
  scanf("%s", nome);
  toLower(nome);

  for (int i = 0; i < *contador; i++) {
    char nomeEstoque[20];
    strcpy(nomeEstoque, estoque[i].nome);
    toLower(nomeEstoque);

    if (strcmp(nomeEstoque, nome) == 0) {
      if (estoque[i].quantidade == 0) {
        for (int j = i; j < *contador - 1; j++) {
          estoque[j] = estoque[j + 1];
        }
        (*contador)--;
        printf("Fruta %s excluída com sucesso!\n", nome);
        return;
      } else {
        printf(
            "Não é possível excluir a fruta %s, ainda há estoque disponível.\n",
            nome);
        return;
      }
    }
  }
  printf("Fruta %s não encontrada.\n", nome);
}

// Função para buscar uma fruta pelo nome no estoque
void buscarFruta(Fruta estoque[], int contador) {
  char nome[20];
  printf("Nome da fruta para buscar: ");
  scanf("%s", nome);

  toLower(nome);

  for (int i = 0; i < contador; i++) {
    char nomeEstoque[20];
    strcpy(nomeEstoque, estoque[i].nome);
    toLower(nomeEstoque);

    if (strcmp(nomeEstoque, nome) == 0) {
      printf("Fruta encontrada: %s | Quantidade: %d | Preço: %.2f\n",
             estoque[i].nome, estoque[i].quantidade, estoque[i].preco);
      return;
    }
  }
  printf("Fruta não encontrada.\n");
}

// Exibir relatório de vendas
void relatorioVendas() {
  printf("\n--- Relatório de Vendas ---\n");

  FILE *arquivo = fopen("vendas.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de vendas.\n");
    return;
  }

  char linha[100];
  while (fgets(linha, sizeof(linha), arquivo)) {
    printf("%s", linha);
  }

  fclose(arquivo);
}
