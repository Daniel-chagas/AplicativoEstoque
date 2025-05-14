#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

// Definições de Estruturas 
typedef struct Produto {
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
    int estoque_minimo;
} Produto;

typedef struct NoProduto {
    Produto produto;
    struct NoProduto* proximo;
} NoProduto;

typedef struct ListaEstoque {
    NoProduto* inicio;
    int tamanho;
} ListaEstoque;

// Funções da Lista 
void inicializarLista(ListaEstoque* lista) {
    lista->inicio = NULL;
    lista->tamanho = 0;
}

bool produtoExiste(ListaEstoque* lista, int codigo) {
    NoProduto* atual = lista->inicio;
    while (atual != NULL) {
        if (atual->produto.codigo == codigo) return true;
        atual = atual->proximo;
    }
    return false;
}

void adicionarProduto(ListaEstoque* lista, Produto novoProduto) {
    if (produtoExiste(lista, novoProduto.codigo)) {
        printf("Erro: Código %d já existe!\n", novoProduto.codigo);
        return;
    }

    NoProduto* novoNo = (NoProduto*)malloc(sizeof(NoProduto));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria");
        exit(EXIT_FAILURE);
    }

    novoNo->produto = novoProduto;
    novoNo->proximo = lista->inicio; 
    lista->inicio = novoNo;
    lista->tamanho++;

    printf("Produto '%s' (codigo %d) adicionado.\n", novoProduto.nome, novoProduto.codigo);
}

Produto* buscarProduto(ListaEstoque* lista, int codigo) {
    NoProduto* atual = lista->inicio;
    while (atual != NULL) {
        if (atual->produto.codigo == codigo) {
            return &(atual->produto);
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Ordenação Bubble Sort 
void ordenarPorValorTotal(ListaEstoque* lista) {
    if (lista->tamanho < 2) return;

    bool trocou;
    do {
        trocou = false;
        NoProduto** atual = &(lista->inicio);
        while ((*atual)->proximo != NULL) {
            NoProduto* noAtual = *atual;
            NoProduto* noProximo = noAtual->proximo;

            float valorAtual = noAtual->produto.preco * noAtual->produto.quantidade;
            float valorProximo = noProximo->produto.preco * noProximo->produto.quantidade;

            if (valorAtual < valorProximo) {
                noAtual->proximo = noProximo->proximo;
                noProximo->proximo = noAtual;
                *atual = noProximo;
                trocou = true;
            }
            atual = &((*atual)->proximo);
        }
    } while (trocou);
}

// Relatório ABC 
void gerarRelatorioABC(ListaEstoque* lista) {
    ordenarPorValorTotal(lista);

    printf("\n Relatorio ABC (Itens mais valiosos primeiro) \n");
    NoProduto* atual = lista->inicio;
    while (atual != NULL) {
        float valorTotal = atual->produto.preco * atual->produto.quantidade;
        printf(" %s (Codigo: %d)\n  Valor total em estoque: R$ %.2f\n",
               atual->produto.nome, atual->produto.codigo, valorTotal);
        atual = atual->proximo;
    }
}

// ---------------------------- Interface ----------------------------
void exibirDadosProduto(const Produto* produto) {
    if (produto == NULL) return;
    printf("\nCódigo: %d\n", produto->codigo);
    printf("Nome: %s\n", produto->nome);
    printf("Preço: R$ %.2f\n", produto->preco);
    printf("Quantidade: %d\n", produto->quantidade);
    printf("Estoque Mínimo: %d\n", produto->estoque_minimo);
}

// ---------------------------- Main ----------------------------
int main() {
    ListaEstoque estoque;
    inicializarLista(&estoque);

    // Cadastro inicial de produtos
    Produto produtosIniciais[] = {
        {1001, "Caneta", 1.99, 50, 10},
        {1002, "Lapis", 0.75, 100, 25},
        {1003, "Borracha", 0.50, 80, 20}
    };

    for (int i = 0; i < 3; i++) {
        adicionarProduto(&estoque, produtosIniciais[i]);
    }

    // Busca de produto
    int codigoBusca = 1002;
    Produto* encontrado = buscarProduto(&estoque, codigoBusca);
    if (encontrado != NULL) {
        printf("\nProduto encontrado:");
        exibirDadosProduto(encontrado);
    } else {
        printf("\nProduto %d não encontrado.\n", codigoBusca);
    }

    // Relatório 
    gerarRelatorioABC(&estoque);

    
    return 0;
}
