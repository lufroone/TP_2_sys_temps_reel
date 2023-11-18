#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>
#include<pthread.h>
#include<stdio.h>

#define delai 399900
#define P(c) (pthread_mutex_lock(c))
#define V(d) (pthread_mutex_unlock(d))

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

void exec_action(int signum){
    printf("recu\n");
    on_prends();
}

int main(){    
    int nb_signaux;
    signal(SIGUSR1, exec_action);
    printf("le PID est %d\n", getpid());
    while(1){
        P(&maj_signaux);
        nb_signaux = signaux;
        V(&maj_signaux);

        if(nb_signaux>0){
            printf("Exec en attente\n");
            do_work(5000);
            on_libere();

        } else {
            printf("pause\n\n");
            do_work(1000);
        }
    }
    return 0;
}

