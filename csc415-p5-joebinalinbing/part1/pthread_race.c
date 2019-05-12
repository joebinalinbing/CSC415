#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
/**
 * THESE DEFINE VALUES CANNOT BE CHANGED.
 * DOING SO WILL CAUSE POINTS TO BE DEDUCTED
 * FROM YOUR GRADE
 */
/** BEGIN VALUES THAT CANNOT BE CHANGED */
#define MAX_THREADS 16
#define MAX_ITERATIONS 40
/** END VALUES THAT CANNOT BE CHANGED */


/**
 * use this struct as a parameter for the function
 * nanosleep.
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {0, 123456};
int global = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *adder_func(void *arg) {
    int temp;
    int idx = *((int *) arg);
    int i;
    for (i = 0; i < MAX_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        temp = global;
        temp += 10;
        global = temp;
        nanosleep(&ts, NULL);
        pthread_mutex_unlock(&mutex);
        fprintf(stderr, "Current Value written to GLobal Variables by ADDER thread id: %d is %d\n", idx, temp);

    }
    return NULL;
}

void *sub_func(void *arg) {
    int temp;
    int idx = *((int *) arg);
    int i;
    for (i = 0; i < MAX_ITERATIONS; i++) {
//        pthread_mutex_lock(&mutex);
        temp = global;
        nanosleep(&ts, NULL);
        temp -= 10;
        global = temp;
        nanosleep(&ts, NULL);
//        pthread_mutex_unlock(&mutex);
        fprintf(stderr, "Current Value written to GLobal Variables by SUBTRACTER thread id :%d is %d\n", idx,
                temp);

    }
    return NULL;
}

int
main(int argc, char **argv) {

    pthread_t threads[MAX_THREADS];
    int idx[MAX_THREADS];
    int result1, result2, result3;
    int i;

    for (i = 0; i < MAX_THREADS; i++) {
        idx[i] = i;

        if (i % 2 == 0) {
            result1 = pthread_create(&threads[i], NULL, adder_func, &idx[i]);
            //   fprintf(stderr, "Current Value written to GLobal Variables by result 1 thread id #%d:\n", result1);
            if (result1 != 0) {
                perror("\ncreate thread adder erroer"); // Display error message
                return -9;
            }
        }
        if (i % 2 != 0) {
            result2 = pthread_create(&threads[i], NULL, sub_func, &idx[i]);
            //  fprintf(stderr, "Current Value written to GLobal Variables by result 2 thread id #%d:\n", result2);
            if (result2 != 0) {
                perror("\ncreate subtrader thread erroer"); // Display error message
                return -5;

            }

        }
    }

    for (i = 0; i < MAX_THREADS; i++) {
        result3 = pthread_join(threads[i], NULL);
        //  fprintf(stderr, "Current Value written to GLobal Variables by result 3 thread id #%d:\n", result3);
        if (result3 != 0) {
            perror("\njoin thread erroer"); // Display error message
            return -5;

        }
    }

    fprintf(stderr, "Final Value of Shared Variable: %d\n", global);

    return 0;
}

