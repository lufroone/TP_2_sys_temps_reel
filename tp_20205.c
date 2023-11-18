#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>
#include<pthread.h>
#include <semaphore.h>

#define delai 399900
#define P(c) (pthread_mutex_lock(c))
#define V(d) (pthread_mutex_unlock(d))

sem_t on_reprends;

int signaux = 0;
pthread_mutex_t maj_signaux = PTHREAD_MUTEX_INITIALIZER;

void on_prends(){
    P(&maj_signaux);
    signaux += 1;
    V(&maj_signaux);
}

void on_libere(){
    P(&maj_signaux);
    signaux -= 1;
    V(&maj_signaux);
}

void do_work(unsigned int duration){
    unsigned long int nbr_boucle = duration * delai;

    for(int i=0;i<nbr_boucle;i++){
        asm volatile("nop");
    }
}

void exec_action(int signum) {
    printf("recu\n");
    on_prends();
    sem_post(&on_reprends);
}

int main(){    
    int nb_signaux;
    sem_init(&on_reprends, 0, 0);
    signal(SIGUSR1, exec_action);
    printf("le PID est %d\n", getpid());
    while (1) {
        P(&maj_signaux);
        nb_signaux = signaux;
        V(&maj_signaux);
        
        if (nb_signaux > 0) {
            printf("Exec en attente\n");
            do_work(5000);
            sem_wait(&on_reprends);
            on_libere();

        } else {
            printf("en attente\n");
            do_work(1000);
        }
    }
    return 0;
}