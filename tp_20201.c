#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


void handle_sigusr1(int signum) {
    (void) signum;
    printf("Processus enfant (PID %d): Signal SIGUSR1 reçu!\n", getpid());
}

int main() {
    pid_t pid = fork(); 

    if (pid == 0) { 
        signal(SIGUSR1, handle_sigusr1);
        printf("Processus enfant (PID %d) en attente du signal SIGUSR1...\n", getpid());

        while (1) {
            pause(); 
        }
    } else if (pid > 0) { 
        printf("Processus parent (PID %d) en cours d'exécution...\n", getpid());
        printf("Envoyez le signal SIGUSR1 au processus enfant (PID %d) à l'aide de la commande kill.\n", pid);

        
        while (1) {
            sleep(10);
        }
    } else {
        perror("Erreur lors du fork");
        return 1;
    }

    return 0;
}
