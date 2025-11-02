#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura Sala
typedef struct Sala {
    char nome[50];          // Nome do cômodo
    struct Sala* esquerda;  // Ponteiro para a sala à esquerda
    struct Sala* direita;   // Ponteiro para a sala à direita
} Sala;

// Função para criar uma nova sala dinamicamente
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas da mansão
void explorarSalas(Sala* atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou a um cômodo sem mais caminhos!\n");
            break;
        }

        printf("Para onde deseja ir? (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho para a esquerda!\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há caminho para a direita!\n");
            }
        } else if (escolha == 's' || escolha == 'S') {
            printf("Saindo da exploração...\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Função principal
int main() {
    // Criando o mapa da mansão
    Sala* hall = criarSala("Hall de Entrada");
    Sala* cozinha = criarSala("Cozinha");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");

    // Montando a árvore manualmente
    hall->esquerda = cozinha;
    hall->direita = salaEstar;
    cozinha->esquerda = biblioteca;
    cozinha->direita = jardim;

    printf("Bem-vindo(a) ao Detective Quest!\n");
    printf("Explore a mansão e descubra todos os cômodos.\n");

    // Inicia a exploração
    explorarSalas(hall);

    // Libera a memória alocada
    free(jardim);
    free(biblioteca);
    free(salaEstar);
    free(cozinha);
    free(hall);

    printf("Exploração finalizada. Obrigado por jogar!\n");
    return 0;
}
