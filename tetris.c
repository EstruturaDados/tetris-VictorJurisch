#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAP_FILA 5
#define CAP_PILHA 3

typedef struct {
    char tipo;
    int id;
} Peca;

typedef struct {
    Peca itens[CAP_FILA];
    int frente;
    int retaguarda;
    int tamanho;
} FilaCircular;

typedef struct {
    Peca itens[CAP_PILHA];
    int topo;
} Pilha;

/* --------- Fila (Nível Novato) --------- */
void inicializarFila(FilaCircular* fila) {
    fila->frente = 0;
    fila->retaguarda = -1;
    fila->tamanho = 0;
}

int filaCheia(FilaCircular* fila) {
    return fila->tamanho == CAP_FILA;
}

int filaVazia(FilaCircular* fila) {
    return fila->tamanho == 0;
}

Peca gerarPeca(int id) {
    static const char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 7];
    p.id = id;
    return p;
}

int enqueue(FilaCircular* fila, Peca p) {
    if (filaCheia(fila)) return 0;
    fila->retaguarda = (fila->retaguarda + 1) % CAP_FILA;
    fila->itens[fila->retaguarda] = p;
    fila->tamanho++;
    return 1;
}

Peca dequeue(FilaCircular* fila) {
    Peca invalida = {'\0', -1};
    if (filaVazia(fila)) return invalida;
    Peca p = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAP_FILA;
    fila->tamanho--;
    return p;
}

void mostrarFila(const FilaCircular* fila) {
    if (filaVazia((FilaCircular*)fila)) {
        printf("Fila: [VAZIA]\n");
        return;
    }
    printf("Fila (%d/%d): ", fila->tamanho, CAP_FILA);
    int pos = fila->frente;
    for (int i = 0; i < fila->tamanho; i++) {
        printf("[%c:%d] ", fila->itens[pos].tipo, fila->itens[pos].id);
        pos = (pos + 1) % CAP_FILA;
    }
    printf("\n");
}

/* --------- Pilha (Nível Aventureiro) --------- */
void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
}

int pilhaCheia(Pilha* pilha) {
    return pilha->topo == CAP_PILHA - 1;
}

int pilhaVazia(Pilha* pilha) {
    return pilha->topo == -1;
}

int push(Pilha* pilha, Peca p) {
    if (pilhaCheia(pilha)) return 0;
    pilha->itens[++pilha->topo] = p;
    return 1;
}

Peca pop(Pilha* pilha) {
    Peca invalida = {'\0', -1};
    if (pilhaVazia(pilha)) return invalida;
    return pilha->itens[pilha->topo--];
}

void mostrarPilha(const Pilha* pilha) {
    if (pilhaVazia((Pilha*)pilha)) {
        printf("Reserva: [VAZIA]\n");
        return;
    }
    printf("Reserva (%d/%d): ", pilha->topo + 1, CAP_PILHA);
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c:%d] ", pilha->itens[i].tipo, pilha->itens[i].id);
    }
    printf("\n");
}

/* --------- Integração (Nível Mestre) --------- */
void trocarFrentePilha(FilaCircular* fila, Pilha* pilha) {
    if (filaVazia(fila)) {
        printf("Fila vazia! Impossivel trocar.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("Pilha vazia! Impossivel trocar.\n");
        return;
    }
    Peca tmp = fila->itens[fila->frente];
    fila->itens[fila->frente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = tmp;
    printf("Troca realizada: frente da fila <-> topo da pilha.\n");
}

void trocarTresPrimeiros(FilaCircular* fila, Pilha* pilha) {
    if (pilha->topo != CAP_PILHA - 1) {
        printf("A pilha deve ter exatamente %d pecas.\n", CAP_PILHA);
        return;
    }
    if (fila->tamanho < 3) {
        printf("A fila deve ter ao menos 3 pecas.\n");
        return;
    }
    int iFila0 = fila->frente;
    int iFila1 = (fila->frente + 1) % CAP_FILA;
    int iFila2 = (fila->frente + 2) % CAP_FILA;

    Peca tmp;

    tmp = fila->itens[iFila0];
    fila->itens[iFila0] = pilha->itens[2];
    pilha->itens[2] = tmp;

    tmp = fila->itens[iFila1];
    fila->itens[iFila1] = pilha->itens[1];
    pilha->itens[1] = tmp;

    tmp = fila->itens[iFila2];
    fila->itens[iFila2] = pilha->itens[0];
    pilha->itens[0] = tmp;

    printf("Troca realizada: 3 primeiros da fila <-> 3 da pilha.\n");
}

/* --------- Utilidades --------- */
void mostrarStatus(const FilaCircular* fila, const Pilha* pilha) {
    printf("\n===== STATUS ATUAL =====\n");
    if (fila) {
        mostrarFila(fila);
    } else {
        printf("Fila: [INDISPONIVEL]\n");
    }
    if (pilha) {
        mostrarPilha(pilha);
    } else {
        printf("Reserva: [INDISPONIVEL]\n");
    }
    printf("========================\n\n");
}

/* --------- Níveis --------- */
void nivelNovato(void) {
    FilaCircular fila;
    inicializarFila(&fila);
    int proximoId = 1;

    for (int i = 0; i < CAP_FILA; i++) enqueue(&fila, gerarPeca(proximoId++));

    int opc;
    for (;;) {
        mostrarStatus(&fila, NULL);
        printf("Menu Novato:\n");
        printf("1 - Jogar peca (remover da frente)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) return;

        if (opc == 1) {
            Peca jogada = dequeue(&fila);
            if (jogada.tipo != '\0') {
                printf("Peca %c (ID:%d) jogada.\n", jogada.tipo, jogada.id);
                enqueue(&fila, gerarPeca(proximoId++));
            }
        } else if (opc == 0) {
            break;
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

void nivelAventureiro(void) {
    FilaCircular fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    int proximoId = 1;

    for (int i = 0; i < CAP_FILA; i++) enqueue(&fila, gerarPeca(proximoId++));

    int opc;
    for (;;) {
        mostrarStatus(&fila, &pilha);
        printf("Menu Aventureiro:\n");
        printf("1 - Jogar peca (remover da frente)\n");
        printf("2 - Enviar frente da fila para reserva (push)\n");
        printf("3 - Usar peca da reserva (pop)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) return;

        if (opc == 1) {
            Peca jogada = dequeue(&fila);
            if (jogada.tipo != '\0') {
                printf("Peca %c (ID:%d) jogada.\n", jogada.tipo, jogada.id);
                enqueue(&fila, gerarPeca(proximoId++));
            }
        } else if (opc == 2) {
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                Peca p = dequeue(&fila);
                push(&pilha, p);
                enqueue(&fila, gerarPeca(proximoId++));
                printf("Peca %c (ID:%d) enviada para reserva.\n", p.tipo, p.id);
            } else if (pilhaCheia(&pilha)) {
                printf("Reserva cheia.\n");
            } else {
                printf("Fila vazia.\n");
            }
        } else if (opc == 3) {
            Peca usada = pop(&pilha);
            if (usada.tipo != '\0') {
                printf("Peca %c (ID:%d) usada da reserva.\n", usada.tipo, usada.id);
            } else {
                printf("Reserva vazia.\n");
            }
        } else if (opc == 0) {
            break;
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

void nivelMestre(void) {
    FilaCircular fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    int proximoId = 1;

    for (int i = 0; i < CAP_FILA; i++) enqueue(&fila, gerarPeca(proximoId++));

    int opc;
    for (;;) {
        mostrarStatus(&fila, &pilha);
        printf("Menu Mestre:\n");
        printf("1 - Jogar peca (remover da frente)\n");
        printf("2 - Enviar frente da fila para reserva (push)\n");
        printf("3 - Usar peca da reserva (pop)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) return;

        if (opc == 1) {
            Peca jogada = dequeue(&fila);
            if (jogada.tipo != '\0') {
                printf("Peca %c (ID:%d) jogada.\n", jogada.tipo, jogada.id);
                enqueue(&fila, gerarPeca(proximoId++));
            }
        } else if (opc == 2) {
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                Peca p = dequeue(&fila);
                push(&pilha, p);
                enqueue(&fila, gerarPeca(proximoId++));
                printf("Peca %c (ID:%d) enviada para reserva.\n", p.tipo, p.id);
            } else if (pilhaCheia(&pilha)) {
                printf("Reserva cheia.\n");
            } else {
                printf("Fila vazia.\n");
            }
        } else if (opc == 3) {
            Peca usada = pop(&pilha);
            if (usada.tipo != '\0') {
                printf("Peca %c (ID:%d) usada da reserva.\n", usada.tipo, usada.id);
            } else {
                printf("Reserva vazia.\n");
            }
        } else if (opc == 4) {
            trocarFrentePilha(&fila, &pilha);
        } else if (opc == 5) {
            trocarTresPrimeiros(&fila, &pilha);
        } else if (opc == 0) {
            break;
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

/* --------- Main --------- */
int main(void) {
    srand((unsigned)time(NULL));

    for (;;) {
        int opc;
        printf("\nMENU PRINCIPAL\n");
        printf("1 - Nivel Novato\n");
        printf("2 - Nivel Aventureiro\n");
        printf("3 - Nivel Mestre\n");
        printf("0 - Sair\n");
        printf("Escolha o nivel: ");
        if (scanf("%d", &opc) != 1) return 0;

        switch (opc) {
            case 1: nivelNovato(); break;
            case 2: nivelAventureiro(); break;
            case 3: nivelMestre(); break;
            case 0: 
                printf("Encerrando.\n");
                return 0;
            default:
                printf("Opcao invalida.\n");
        }
    }
}
