#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10  // Tamanho da tabela hash simples

typedef struct Sala {
    char nome[50];          
    char pista[100];        
    struct Sala* esquerda;  
    struct Sala* direita;   
} Sala;

typedef struct PistaNode {
    char pista[100];            
    struct PistaNode* esquerda; 
    struct PistaNode* direita;  
} PistaNode;

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* proximo;
} HashNode;

// ----------------------------
// Funções de sala
// ----------------------------
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    if (pista != NULL) strcpy(novaSala->pista, pista);
    else novaSala->pista[0] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// ----------------------------
// Funções BST de pistas
// ----------------------------
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novoNode = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(novoNode->pista, pista);
    novoNode->esquerda = NULL;
    novoNode->direita = NULL;
    return novoNode;
}

PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) return criarPistaNode(pista);
    if (strcmp(pista, raiz->pista) < 0) raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0) raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// ----------------------------
// Funções hash
// ----------------------------
int hash(char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) soma += chave[i];
    return soma % HASH_SIZE;
}

void inserirNaHash(HashNode* tabela[], const char* pista, const char* suspeito) {
    int index = hash((char*)pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[index];
    tabela[index] = novo;
}

char* encontrarSuspeito(HashNode* tabela[], const char* pista) {
    int index = hash((char*)pista);
    HashNode* atual = tabela[index];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

// ----------------------------
// Exploração interativa
// ----------------------------
void explorarSalas(Sala* atual, PistaNode** raizPistas) {
    char escolha;
    Sala* proxima;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        }

        printf("Para onde deseja ir? (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &escolha);
        proxima = NULL;

        if (escolha == 'e' || escolha == 'E') proxima = atual->esquerda;
        else if (escolha == 'd' || escolha == 'D') proxima = atual->direita;
        else if (escolha == 's' || escolha == 'S') {
            printf("Finalizando exploração...\n");
            break;
        } else {
            printf("Opção inválida!\n");
        }

        atual = proxima;
    }
}

// ----------------------------
// Verificação final
// ----------------------------
int verificarSuspeitoFinal(PistaNode* raiz, HashNode* tabela[], const char* suspeito) {
    if (raiz == NULL) return 0;
    int count = 0;
    char* s = encontrarSuspeito(tabela, raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0) count = 1;
    count += verificarSuspeitoFinal(raiz->esquerda, tabela, suspeito);
    count += verificarSuspeitoFinal(raiz->direita, tabela, suspeito);
    return count;
}

// ----------------------------
// Main
// ----------------------------
int main() {
    // Criando salas
    Sala* hall = criarSala("Hall de Entrada", "Tapete com marcas suspeitas");
    Sala* cozinha = criarSala("Cozinha", "Faca fora do lugar");
    Sala* salaEstar = criarSala("Sala de Estar", "Janela aberta");
    Sala* biblioteca = criarSala("Biblioteca", "Livro caído no chão");
    Sala* jardim = criarSala("Jardim", "Pegadas na lama");

    hall->esquerda = cozinha;
    hall->direita = salaEstar;
    cozinha->esquerda = biblioteca;
    cozinha->direita = jardim;

    // BST
    PistaNode* raizPistas = NULL;

    // Hash
    HashNode* tabela[HASH_SIZE] = {NULL};
    inserirNaHash(tabela, "Tapete com marcas suspeitas", "Carlos Verde");
    inserirNaHash(tabela, "Faca fora do lugar", "Ana Azul");
    inserirNaHash(tabela, "Janela aberta", "Carlos Verde");
    inserirNaHash(tabela, "Livro caído no chão", "Ana Azul");
    inserirNaHash(tabela, "Pegadas na lama", "Paulo Amarelo");

    printf("Bem-vindo(a) ao Detective Quest!\n");
    explorarSalas(hall, &raizPistas);

    printf("\n=== Pistas coletadas em ordem alfabética ===\n");
    exibirPistas(raizPistas);

    // Suspeitos
    char* suspeitos[] = {"Carlos Verde", "Ana Azul", "Paulo Amarelo"};
    int n_suspeitos = 3;

    printf("\nSuspeitos disponíveis:\n");
    for (int i = 0; i < n_suspeitos; i++) printf("%d. %s\n", i+1, suspeitos[i]);

    char acusacao[50];
    int escolha;
    printf("Escolha o número do suspeito que deseja acusar: ");
    scanf("%d", &escolha);
    if (escolha >= 1 && escolha <= n_suspeitos) strcpy(acusacao, suspeitos[escolha-1]);
    else {
        printf("Escolha inválida. Acusação não registrada.\n");
        strcpy(acusacao, "");
    }

    // Determina culpado real e motivo
    char culpado[50] = "Carlos Verde";
    char* motivo = "As pistas 'Tapete com marcas suspeitas' e 'Janela aberta' apontam para ele.";

    if (strlen(acusacao) > 0) {
        int pistas = verificarSuspeitoFinal(raizPistas, tabela, acusacao);
        if (pistas >= 2) printf("Acusação correta! O culpado é realmente %s.\n", acusacao);
        else printf("Acusação incorreta! Não há evidências suficientes contra %s.\n", acusacao);
    }

    printf("\n=== Resposta do caso ===\n");
    printf("O culpado era: %s\n", culpado);
    printf("Motivo: %s\n", motivo);

    // Libera memória
    free(jardim); free(biblioteca); free(salaEstar); free(cozinha); free(hall);
    liberarPistas(raizPistas);
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = tabela[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }

    printf("\nInvestigação finalizada.\n");
    return 0;
}
