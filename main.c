#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

// Definições dos estados do portão
#define StAberto 0
#define StFechado 1
#define StAbrindo 2
#define StFechando 3
#define StEmergencia 4
#define StIncompleto 5

// Variáveis globais que armazenam o estado do portão e a emergência
int EstadoAtual = StFechado; // Estado inicial: Portão Fechado
int ProximoEstado = StFechado; // Próximo estado, inicialmente igual ao atual
bool emergencia_ativada = false; // Flag que indica se a emergência está ativada

// Declaração das threads e o mutex (para sincronização entre as threads)
pthread_t thread_portao; // Thread para controlar abertura/fechamento
pthread_mutex_t lock;    // Mutex para garantir que as variáveis globais sejam manipuladas de forma segura

// Função que simula a abertura do portão
void abrePortao() {
    printf("Portão ABRINDO...\r\n");
    for (int tempo_abertura = 1; tempo_abertura <= 10; tempo_abertura++) {
        sleep(1); // Simula o tempo de abertura de 1 segundo por ciclo
        pthread_mutex_lock(&lock); // Bloqueia o acesso às variáveis compartilhadas
        if (emergencia_ativada) { // Verifica se a emergência foi ativada
            printf("!!! EMERGÊNCIA ATIVADA !!!\n");
            printf("Emergência ativada! Portão parou de abrir após %d segundos.\r\n", tempo_abertura);
            EstadoAtual = StEmergencia;  // Muda o estado para emergência
            pthread_mutex_unlock(&lock); // Libera o acesso às variáveis
            return; // Interrompe a abertura devido à emergência
        }
        pthread_mutex_unlock(&lock); // Libera o acesso às variáveis
    }
    pthread_mutex_lock(&lock);
    EstadoAtual = StAberto; // Após 10 segundos, o portão está totalmente aberto
    printf("Portão está totalmente ABERTO.\r\n");
    pthread_mutex_unlock(&lock);
}

// Função que simula o fechamento do portão
void fechaPortao() {
    printf("Portão FECHANDO...\r\n");
    for (int tempo_fechamento = 1; tempo_fechamento <= 10; tempo_fechamento++) {
        sleep(1); // Simula o tempo de fechamento de 1 segundo por ciclo
        pthread_mutex_lock(&lock); // Bloqueia o acesso às variáveis compartilhadas
        if (emergencia_ativada) { // Verifica se a emergência foi ativada
            printf("!!! EMERGÊNCIA ATIVADA !!!\n");
            printf("Emergência ativada! Portão parou de fechar após %d segundos.\r\n", tempo_fechamento);
            EstadoAtual = StEmergencia;  // Muda o estado para emergência
            pthread_mutex_unlock(&lock); // Libera o acesso às variáveis
            return; // Interrompe o fechamento devido à emergência
        }
        pthread_mutex_unlock(&lock); // Libera o acesso às variáveis
    }
    pthread_mutex_lock(&lock);
    EstadoAtual = StFechado; // Após 10 segundos, o portão está totalmente fechado
    printf("Portão está totalmente FECHADO.\r\n");
    pthread_mutex_unlock(&lock);
}

// Função que será executada pela thread para controlar o portão
void* controlePortao(void* comando) {
    char* acao = (char*)comando; // Recebe o comando para abrir ou fechar o portão

    if (strcmp(acao, "ABRIR") == 0) {
        abrePortao(); // Chama a função para abrir o portão
    } else if (strcmp(acao, "FECHAR") == 0) {
        fechaPortao(); // Chama a função para fechar o portão
    }

    return NULL; // Fim da execução da thread
}

// Thread principal que monitora os comandos do usuário e a emergência
void* monitorarComandos(void* arg) {
    char comando[20]; // Variável para armazenar o comando digitado pelo usuário

    // Exibe a mensagem inicial de comando
    printf("Digite um comando (ABRIR, FECHAR, EMERGENCIA, SAIR): \n");

    while (1) {
        scanf("%s", comando); // Aguarda o usuário digitar um comando

        pthread_mutex_lock(&lock); // Bloqueia o acesso às variáveis compartilhadas

        if (strcmp(comando, "SAIR") == 0) { // Verifica se o comando é SAIR
            printf("Encerrando o programa...\r\n");
            exit(0);  // Encerra o programa
        }

        // Agora a emergência pode ser ativada independentemente do estado do portão
        if (strcmp(comando, "EMERGENCIA") == 0) {
            emergencia_ativada = true; // Ativa a flag de emergência
            printf("!!! EMERGÊNCIA ATIVADA !!!\n");
            EstadoAtual = StEmergencia; // Atualiza o estado para emergência, mesmo se o portão estiver aberto ou fechado
        } 
        // Verifica se o comando é ABRIR e se o estado atual permite isso
        else if (strcmp(comando, "ABRIR") == 0 && (EstadoAtual == StFechado || EstadoAtual == StEmergencia)) {
            emergencia_ativada = false; // Desativa a emergência
            ProximoEstado = StAbrindo; // Define o próximo estado
            pthread_create(&thread_portao, NULL, controlePortao, "ABRIR"); // Cria uma thread para abrir o portão
        } 
        // Verifica se o comando é FECHAR e se o estado atual permite isso
        else if (strcmp(comando, "FECHAR") == 0 && (EstadoAtual == StAberto || EstadoAtual == StEmergencia)) {
            emergencia_ativada = false; // Desativa a emergência
            ProximoEstado = StFechando; // Define o próximo estado
            pthread_create(&thread_portao, NULL, controlePortao, "FECHAR"); // Cria uma thread para fechar o portão
        } else {
            printf("Comando inválido ou o portão não está no estado correto para essa ação.\n");
        }

        pthread_mutex_unlock(&lock); // Libera o acesso às variáveis compartilhadas
    }
    return NULL;
}

int main(void) {
    pthread_t thread_comandos; // Thread para monitorar os comandos do usuário

    // Inicializa o mutex
    pthread_mutex_init(&lock, NULL);

    // Cria a thread que ficará monitorando os comandos do usuário
    pthread_create(&thread_comandos, NULL, monitorarComandos, NULL);

    // Aguarda a thread de comandos finalizar
    pthread_join(thread_comandos, NULL);

    // Destroi o mutex ao final
    pthread_mutex_destroy(&lock);

    return 0;
}
