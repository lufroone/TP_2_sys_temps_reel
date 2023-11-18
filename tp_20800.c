#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define delai 399999995

void do_work(double iterations) {
    for (int i = 0; i < iterations; i++) {
        asm volatile ("nop");
    }
}

void t2() {
    printf("t2 lancé\n");
    do_work(delai * 0.666);  // Occupera le CPU pendant 0.333s
    printf("t2 terminé\n");
}

void t3() {
    printf("t3 lancé\n");
    do_work(delai * 2);  // Occupera le CPU pendant 1s
    printf("t3 terminé\n");
}

void t4() {
    printf("t4 lancé\n");
    do_work(delai * 4);  // Occupera le CPU pendant 2s
    printf("t4 terminé\n");
}

int main() {

    printf("ca part\n");
    while (1) {
        t2();
        t3();
        t4();
        t2();
        t3();
        t2();
        t4();
        t2();
        t3();
        t2();
        t4();
        t2();
        t3();
    }

    return 0;
}

