#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>

#define delai 399999995
#define DUREE_T2 0.25 
#define PERIODE_T2 2   
#define DUREE_T3 0.75     
#define PERIODE_T3 3   
#define DUREE_T4 1.5     
#define PERIODE_T4 4

void do_work(double duration) {
    clock_t end_time = clock() + (duration * CLOCKS_PER_SEC);

    while (clock() < end_time) {
        asm volatile("nop");
    }
}

void executeTask(const char* taskName, double duration) {
    printf("Exécution de la tâche %s pour %f secondes.\n", taskName, duration);
    do_work(duration);
}

int main() {
    int priority_T2 = 1; 
    int priority_T3 = 2;
    int priority_T4 = 3; 
    struct sched_param param_T2;
    struct sched_param param_T3;
    struct sched_param param_T4;
    param_T2.sched_priority = priority_T2;
    param_T3.sched_priority = priority_T3;
    param_T4.sched_priority = priority_T4;
    if (sched_setparam(0, &param_T2) == -1) {
        perror("Erreur lors de la définition de la priorité pour T2");
    }
    if (sched_setparam(0, &param_T3) == -1) {
        perror("Erreur lors de la définition de la priorité pour T3");
    }
    if (sched_setparam(0, &param_T4) == -1) {
        perror("Erreur lors de la définition de la priorité pour T4");
    }
    time_t startTime = time(NULL);
    while (difftime(time(NULL), startTime) < 12) {
        time_t currentTime = time(NULL);
        if ((currentTime - startTime) % PERIODE_T2 == 0) {
            executeTask("T2", DUREE_T2);
        }
        if ((currentTime - startTime) % PERIODE_T3 == 0) {
            executeTask("T3", DUREE_T3);
        }
        if ((currentTime - startTime) % PERIODE_T4 == 0) {
            executeTask("T4", DUREE_T4);
        }

    }
    printf("Fin de la boucle de 12 secondes.\n");
    return 0;
}
