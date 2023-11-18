
#define _POSIX_C_SOURCE 200809L


#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void do_work(unsigned int duration) {
    clock_t start_time = clock();
    while ((clock() - start_time) < duration * (CLOCKS_PER_SEC / 1000)) {
        asm volatile("nop");
    }
}

void* signal_thread(void* arg) {
    (void) arg;
    sigset_t set;
    int sig;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    printf("le PID est %d\n", getpid());

    while (1) {
        sigwait(&set, &sig);
        if (sig == SIGUSR1) {
            printf("Thread a reçu le signal SIGUSR1! Exécution de do_work...\n");
            do_work(1000);
            printf("do_work terminé.\n");
        }
    }
    return NULL;
}

int main() {
    pthread_t tid;
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    pthread_create(&tid, NULL, signal_thread, NULL);

    printf("Thread principal en cours d'exécution. Envoyez le signal SIGUSR1 à ce processus.\n");

    pthread_join(tid, NULL); 

    return 0;
}
