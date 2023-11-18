#include <stdio.h>
#include <time.h>

void do_work(unsigned int duration) {
    printf("debut\n");
    clock_t start_time = clock();
    clock_t end_time = start_time + (duration * CLOCKS_PER_SEC) / 1000;
    while (clock() < end_time) {
        asm volatile("nop");
    }
    printf("Fin du temps de 500ms\n");
}

int main() {
    unsigned int duration = 500;  // 500 millisecondes
    do_work(duration);
    return 0;
}

