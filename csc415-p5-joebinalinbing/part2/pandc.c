#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


//global args
int n;
int p;
int c;
int x;
int pTime;
int cTime;
int totalPro;
int totalCon;

//Over Consumed variables
int setOverConsumedID;
int amountOfOverconsumed;
int overConsumed;
int isOverconsumed;

//arrays
int *buffer;
int *pArray;
int *cArray;


//semaphores
sem_t fullCount;
sem_t emptyCount;


//mutex
pthread_mutex_t mutex;


//Counters
int pCount;   //for arrays
int cCount;   //for arrays
int amountProd; //for threads
int amountCons;  //for threads
int items;    //buffer


//rand
int randC;  //randomize consumer id who consumes the extra item



/*
 * Function to add item.
 * Item added is returned.
 * It is up to you to determine
 * how to use the ruturn value.
 * If you decide to not use it, then ignore
 * the return value, do not change the
 * return type to void.
 */


int enqueue_item(int item)
{

    if (amountProd < n) {
        buffer[amountProd] = item;
        amountProd = (amountProd+1);
    }
    else {
        amountProd = 0;
        buffer[amountProd] = item;
        amountProd = (amountProd+1);
    }

    return item; //Doesnt do anything
}

void *producer(void *index){
    int id = *((int*)index);
    for(int i = 0; i < x; i++){
        sem_wait(&emptyCount);
        pthread_mutex_lock(&mutex);
        items++;
        enqueue_item(items);
        printf("Item %d was produced by producer-> ID #%d\n", items, id);
        pArray[pCount++] = items;
        pthread_mutex_unlock(&mutex);
        sem_post(&fullCount);
        sleep(pTime);
    }
}

/*
 * Function to remove item.
 * Item removed is returned
 */
int dequeue_item()
{
int item;
    if (amountCons < n) {
       item = buffer[amountCons];
        amountCons = (amountCons+1);
    }
    else {
        amountCons = 0;
        item = buffer[amountCons];
        amountCons = (amountCons+1);
    }
    return item;
}

void *consumer(void *index){
    int id = *((int*)index);
    if(amountOfOverconsumed != 0 && id == setOverConsumedID){
        for(int i = 0; i < overConsumed; i++){
            sem_wait(&fullCount);
            pthread_mutex_lock(&mutex);
            int temp = dequeue_item();
            printf("Item %d was consumed by consumer-> ID #%d\n", temp, id);
            cArray[cCount++] = temp;
            pthread_mutex_unlock(&mutex);
            sem_post(&emptyCount);
            sleep(cTime);
        }
    }else{
        for(int i = 0; i < totalCon; i++){
            sem_wait(&fullCount);
            pthread_mutex_lock(&mutex);
            int temp = dequeue_item();
            printf("Item %d was consumed by consumer-> ID #%d\n", temp, id);
            cArray[cCount++] = temp;
            pthread_mutex_unlock(&mutex);
            sem_post(&emptyCount);
            sleep(cTime);
        }
    }
}


void initializeBuffer() {
    pArray = (int*)malloc(sizeof(int)*p*x);
    cArray = (int*)malloc(sizeof(int)*p*x);
    buffer = malloc(n*sizeof(int)); //allocate memory for the buffer
    pCount = 0;
    cCount = 0;
    amountProd = 0;
    amountCons = 0;
    items=0 ;
    totalPro=p*x;
    totalCon = (p*x)/c;
    pthread_mutex_init(&mutex,NULL);
    sem_init(&fullCount,0,0);
    sem_init(&emptyCount,0,n);

    //rand
    srand(time(NULL));
    int randC = ( rand() % c ) + 1;

    setOverConsumedID = 0;
    amountOfOverconsumed = (p*x)%c;
    isOverconsumed=0;
    if(amountOfOverconsumed != 0){
        setOverConsumedID = randC;
        overConsumed = totalCon + amountOfOverconsumed;
        isOverconsumed=1;
    }else{
        overConsumed = 0;
    }

}

int main(int argc, char** argv) {

    if (argc != 7) {
        //if we don't receive 6 args, exit
        printf("Six arguments required.\n");
        return 0;
    }
    //collect command line arguments
    n = atoi(argv[1]);
    p = atoi(argv[2]);
    c = atoi(argv[3]);
    x = atoi(argv[4]);
    pTime = atoi(argv[5]);
    cTime = atoi(argv[6]);

    initializeBuffer();
    time_t currtime = time(NULL);


    printf("Current Time: %s\n",ctime(&currtime));

    printf("\n");
    printf("%42s %4d\n","Number of Buffers:", n);
    printf("%42s %4d\n","Number of Producers:", p);
    printf("%42s %4d\n","Number of Consumers:", c);
    printf("%42s %4d\n","Number of items Produced by each producer:", x);
    printf("%42s %4d\n","Number of items consumed by each consumer:", totalCon);
    printf("%42s %4d\n","Over consume on?:", isOverconsumed);
    printf("%42s %4d\n","Over consume amount:", amountOfOverconsumed);
    printf("%42s %4d\n","Time each Producer Sleeps (seconds):", pTime);
    printf("%42s %4d\n","Time each Consumer Sleeps (seconds):", cTime);
    printf("\n");

///THREADS
    int i;
    pthread_t prothreads[p];
    pthread_t conthreads[c];
    int prargs[p];
    int coargs[c];

    for (i = 0; i < p; i++) {
        prargs[i] = i+1;
        pthread_create(&prothreads[i], NULL, producer, &prargs[i]);
    }

    for (i = 0; i < c; i++) {
        coargs[i] = i+1;
        pthread_create(&conthreads[i], NULL, consumer, &coargs[i]);
    }

    for (i = 0; i < p; i++) {
        pthread_join(prothreads[i], NULL);
        printf("Producer %d joined\n",i+1);
    }

    for (i = 0; i < c; i++) {
        pthread_join(conthreads[i], NULL);
        printf("Consumer %d joined\n",i+1);
    }
    time_t currtime2 = time(NULL);
    printf("Current Time: %s\n",ctime(&currtime2));

    int match = 1;

    printf("\n%-10s| CONSUMER\n","PRODUCER");

    for (i = 0; i < totalPro; i++) {
        printf("%-10d| %d\n",pArray[i],cArray[i]);
        if (pArray[i] != cArray[i]) {
            match = 0;
        }
    }
    if (match) {
        printf("Arrays match!\n");
    } else { printf("Arrays don't match!\n"); }


    time_t endtime = time(NULL);

    double diff = difftime(endtime,currtime);

    printf("Total time elapsed: %g seconds\n",diff);
    //free data structures to prevent memory leak
    pthread_mutex_destroy(&mutex);
    free(buffer);
    free(pArray);
    free(cArray);
    sem_destroy(&fullCount);
    sem_destroy(&emptyCount);
}