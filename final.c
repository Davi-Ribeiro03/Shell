#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_MAX_DE_COMANDOS 100
#define NUM_MAX_DE_ARG 10

void ler_comando(char *);

int main()
{
    char comando[NUM_MAX_DE_COMANDOS];
    char *args[NUM_MAX_DE_ARG];
    int status;

    for (;;)
    {
        ler_comando(comando);

        // Quebrar o comando em argumentos
        char *token = strtok(comando, " ");
        int i = 0;

        do
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        } while (token != NULL && i < NUM_MAX_DE_ARG - 1);

        args[i] = NULL; // Marcar o fim dos argumentos

        // Saída do shell se o comando for "exit"
        if (strcmp(args[0], "exit") == 0)
        {
            printf("Saindo do shell...\n");
            break;
        }

        // Fork para executar o comando
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Erro ao criar processo filho");
            exit(1);
        }
        else if (pid == 0)
        {
            // Processo filho
            if (execvp(args[0], args) == -1)
            {
                perror("Erro ao executar comando");
                exit(1);
            }
        }
        else
        {
            // Processo Pai
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

void ler_comando(char *comando)
{
    printf("Shell>> ");
    fgets(comando, NUM_MAX_DE_COMANDOS, stdin);
    // Remover o caractere de nova linha no final
    comando[strcspn(comando, "\n")] = NULL;
}