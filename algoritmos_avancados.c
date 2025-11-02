#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------
// Estrutura da Mansão
// ----------------------------
typedef struct Sala {
    char nome[50];          // Nome do cômodo
    char pista[100];        // Pista opcional da sala
    struct Sala* esquerda;  // Sala à esquerda
    struct Sala* direita;   // Sala à direita
} Sala;

// ----------------------------
// Estrutura da BST de pistas
// ----------------------------
typedef struct PistaNode {
    char pista[100];            // Conteúdo da pista
    struct PistaNode* esquerda; // Filho esquerdo
    struct PistaNode* direita;  // Filho direito
} PistaNode;

// ----------------------------
// Função para criar uma sala
// ----------------------------
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (!novaSala) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    if (pista != NULL) strcpy(novaSala->pista, pista);
    else novaSala->pista[0] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// ----------------------------
// Função para criar nó de pista na BST
// ----------------------------
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novoNode = (PistaNode*) malloc(sizeof(PistaNode));
    if (!novoNode) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novoNode->pista, pista);
    novoNode->esquerda = NULL;
    novoNode->direita = NULL;
    return novoNode;
}

// ----------------------------
// Inserir pista na BST
// ----------------------------
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz; // Não insere duplicatas
}

// ----------------------------
// Exibir pistas em ordem alfabética (in-order)
// ----------------------------
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ----------------------------
// Explorar salas com coleta de pistas (corrigido)
// ----------------------------
void explorarSalasComPistas(Sala* atual, PistaNode** raizPistas) {
    char escolha;
    Sala* proxima;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Coleta a pista, se existir
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        }

        printf("Para onde deseja ir? (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &escolha);

        // Inicializa próxima sala como NULL
        proxima = NULL;

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                proxima = atual->esquerda;
            } else {
                printf("Não há caminho para a esquerda!\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                proxima = atual->direita;
            } else {
                printf("Não há caminho para a direita!\n");
            }
        } else if (escolha == 's' || escolha == 'S') {
            printf("Saindo da exploração...\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }

        // Atualiza a sala atual
        atual = proxima;
    }
}

// ----------------------------
// Função principal
// ----------------------------
int main() {
    // Criando salas da mansão com pistas
    Sala* hall = criarSala("Hall de Entrada", "Um tapete com marcas suspeitas");
    Sala* cozinha = criarSala("Cozinha", "Uma faca fora do lugar");
    Sala* salaEstar = criarSala("Sala de Estar", "Uma janela aberta");
    Sala* biblioteca = criarSala("Biblioteca", "Um livro caído no chão");
    Sala* jardim = criarSala("Jardim", "Pegadas na lama");

    // Montando a árvore da mansão
    hall->esquerda = cozinha;
    hall->direita = salaEstar;
    cozinha->esquerda = biblioteca;
    cozinha->direita = jardim;

    // Raiz da BST de pistas
    PistaNode* raizPistas = NULL;

    printf("Bem-vindo(a) ao Detective Quest\n");
    printf("Explore a mansão e colete todas as pistas.\n");

    // Inicia exploração
    explorarSalasComPistas(hall, &raizPistas);

    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\n=== Todas as pistas coletadas em ordem alfabética ===\n");
    exibirPistas(raizPistas);

    // Libera memória das salas
    free(jardim);
    free(biblioteca);
    free(salaEstar);
    free(cozinha);
    free(hall);

    // Função para liberar a BST (recursiva)
    void liberarPistas(PistaNode* raiz) {
        if (raiz != NULL) {
            liberarPistas(raiz->esquerda);
            liberarPistas(raiz->direita);
            free(raiz);
        }
    }
    liberarPistas(raizPistas);

    printf("\nExploração finalizada. Obrigado por jogar!\n");
    return 0;
}
