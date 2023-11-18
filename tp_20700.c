#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define PERIOD 1000000000L   // Période en nanosecondes (1 seconde)

#define I 399900

void do_work(unsigned int duration){
    printf("travail commencez\n");
    unsigned long int nbr_boucle = duration * I;

    for(int i=0;i<nbr_boucle;i++){
        asm volatile("nop");
    }
    printf("travail terminé\n");
}

void timer_handler(int sig, siginfo_t *si, void *uc) {
    do_work(5000);
}

int main() {
    struct sigaction sa;
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    // Configuration de l'action du signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }
    // Configuration de l'événement timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        return 1;
    }
    // Configuration de l'intervalle et de la première expiration du timer
    its.it_value.tv_sec = 3;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 10;
    its.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        return 1;
    }
    // Attendre les signaux indéfiniment
    while (1) {
        pause();
    }
    return 0;
}

/*
Pour execution :
gcc -o tp_20700 tp_20700.c -lrt -D_POSIX_C_SOURCE=199309L
*/


/*

Ce code est un exemple de la manière dont on peut créer une tâche périodique en utilisant les timers POSIX. Décortiquons chaque partie du code.

    Déclarations et définitions initiales:

        #define PERIOD 1000000000L: C'est la définition d'une constante qui représente une période d'une seconde en nanosecondes.

        travail_500ms(): Cette fonction simule une tâche qui dure 500 ms. Elle utilise nanosleep pour "dormir" pendant 500 ms.

        timer_handler(): C'est le gestionnaire de signal qui est appelé chaque fois que le timer expire. À chaque expiration, il appelle la fonction travail_500ms.

    Configuration du signal:

        struct sigaction sa;: Cette structure est utilisée pour définir comment le signal doit être traité.

        sa.sa_flags = SA_SIGINFO;: On indique qu'on veut utiliser la version "élargie" du gestionnaire de signal qui fournit des informations supplémentaires sur le signal.

        sa.sa_sigaction = timer_handler;: Ici, on définit la fonction timer_handler comme gestionnaire de signal.

        sigaction(SIGRTMIN, &sa, NULL): Cette fonction associe le signal SIGRTMIN (un signal en temps réel) à la structure d'action sa.

    Configuration de l'événement timer:

        struct sigevent sev;: Cette structure définit comment le timer doit notifier le processus de son expiration.

        sev.sigev_notify = SIGEV_SIGNAL;: On indique que le timer doit notifier le processus en envoyant un signal.

        sev.sigev_signo = SIGRTMIN;: Le signal à envoyer est SIGRTMIN.

        sev.sigev_value.sival_ptr = &timerid;: On passe une valeur au gestionnaire de signal. Ici, c'est l'adresse du timer.

        timer_create(CLOCK_REALTIME, &sev, &timerid): Cette fonction crée un nouveau timer. CLOCK_REALTIME est le type d'horloge. &sev est la structure de configuration pour les notifications. &timerid est l'endroit où le timer créé sera stocké.

    Configuration de l'intervalle et de la première expiration du timer:

        struct itimerspec its;: Cette structure définit la première expiration du timer et son intervalle.

        its.it_value.tv_sec = 1; et its.it_interval.tv_sec = 1;: Ces deux lignes définissent la première expiration et l'intervalle à 1 seconde.

        timer_settime(timerid, 0, &its, NULL): Cette fonction configure le timer avec les valeurs de its.

    Boucle principale:
        while (1) { pause(); }: La fonction pause fait dormir le processus jusqu'à ce qu'il reçoive un signal. Ainsi, le processus restera en sommeil jusqu'à ce que le timer expire, déclenchant ainsi le signal SIGRTMIN et l'exécution du timer_handler.

*/