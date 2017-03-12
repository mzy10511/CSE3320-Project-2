#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 1024
#define BUF_SIZE 10

// the buffer works like a stack for
// the sake of simplicity, if needed
// we may implement a queue
typedef struct {
    char buf[BUF_SIZE]; // the buffer
    size_t len; // number of items in the buffer
    pthread_mutex_t mutex; // needed to add/remove data from the buffer
    pthread_cond_t can_produce; // signaled when items are removed
    pthread_cond_t can_consume; // signaled when items are added
} buffer_t;

int index = 0;
int written = 0;
int n1;
char *s1;
FILE *fp ;

int readf(FILE * fp){
    if((fp=fopen("messages.txt","r"))==NULL){
        printf("ERROR: cant open messages.txt!\n");
        return 0;
    }
    s1=(char *)malloc(sizeof(char)*MAX);
    if(s1==NULL){
        printf("ERROR: Out of memory\n");
        return -1;
    }
    //printf("File Opened!\n");//test

    //fgets(s1, MAX, fp);
    //n1=strlen(s1);

    //test hardcode
    s1 = "hello world";
    n1=strlen(s1);


    if(s1==NULL){
        return -1;
    }
    return 1;
}


// produce random numbers
void* producer(void *arg) {
    buffer_t *buffer = (buffer_t*)arg;

    while(index < n1) {
        //printf("Producing!\n");//test
        pthread_mutex_lock(&buffer->mutex);

        if(buffer->len == BUF_SIZE) { // full
            // wait until some elements are consumed
            pthread_cond_wait(&buffer->can_produce, &buffer->mutex);
        }

        // in real life it may be some data fetched from
        // sensors, the web, or just some I/O
        char t = s1[index];
        index++;
        //printf("%c", t);//test

        // append data to the buffer
        buffer->buf[buffer->len] = t;
        ++buffer->len;

        // signal the fact that new items may be consumed
        pthread_cond_signal(&buffer->can_consume);
        pthread_mutex_unlock(&buffer->mutex);
    }

    // never reached
    return NULL;
}

// consume random numbers
void* consumer(void *arg) {
    //printf("Consuming!\n");//test
    buffer_t *buffer = (buffer_t*)arg;

    while(written <= index + 1) {
        pthread_mutex_lock(&buffer->mutex);

        if(buffer->len == 0) { // empty
            // wait for new items to be appended to the buffer
            pthread_cond_wait(&buffer->can_consume, &buffer->mutex);
        }

        // grab data
        --buffer->len;
        printf("%c", buffer->buf[written]);
        written++;

        // signal the fact that new items may be produced
        pthread_cond_signal(&buffer->can_produce);
        pthread_mutex_unlock(&buffer->mutex);
    }

    // never reached
    return NULL;
}

int main(int argc, char *argv[]) {
    int count;
    int i, rc;
    readf(fp);

    buffer_t buffer = {
        .len = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .can_produce = PTHREAD_COND_INITIALIZER,
        .can_consume = PTHREAD_COND_INITIALIZER
    };

    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer, (void*)&buffer);
    pthread_create(&cons, NULL, consumer, (void*)&buffer);

    pthread_join(prod, NULL); // will wait forever
    pthread_join(cons, NULL);

    return 1;
}
