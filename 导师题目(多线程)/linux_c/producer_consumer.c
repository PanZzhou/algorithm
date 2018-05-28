#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define Max_size 5
int Cycles = 8;
int k = 0;
int consumer_num = 2;
int capacity = 0;
int buffer[Max_size];
pthread_mutex_t mutex;
pthread_cond_t is_full,is_empty;

void * myproducer(void * rank){
    while(k<Cycles){
        pthread_mutex_lock(&mutex);
        while(capacity==Max_size){
            pthread_cond_wait(&is_full,&mutex);
        }
        int random = rand()%100+1;
        printf("producer put %d\n",random);
        for(int i=0;i<Max_size;i++){
            if(buffer[i]==0){
                buffer[i] = random;
                capacity++;
                break;
            }
        }
        k++;
        if(k==Cycles)
            capacity+=2;
        pthread_cond_signal(&is_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void * myconsumer(void * rank){
    int myrank = *((int *)rank);
    printf("%d\n",myrank);
    while(1){
        pthread_mutex_lock(&mutex);
        if(k==Cycles&&finished(myrank)){
            pthread_mutex_unlock(&mutex);
            break;
        }
        while(capacity==0){
            pthread_cond_wait(&is_empty,&mutex);
        }
        int index = -1;
        for(int i=0;i<Max_size;i++){
            if(buffer[i]!=0&&(myrank+buffer[i])%2==0)
                index = i;
        }
        if(index!=-1){
            printf("consumer %d get %d\n",myrank,buffer[index]);
            buffer[index] = 0;
            capacity--;
        }
        pthread_cond_signal(&is_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int finished(int rank){
    for(int i=0;i<Cycles;i++)
        if(buffer[i]!=0&&(rank+buffer[i])%2==0)
            return 0;
    return 1;
}

int main(){
    int a=1,b=2;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&is_full,NULL);
    pthread_cond_init(&is_empty,NULL);
    pthread_t producer,consumer_1,consumer_2;
    
    pthread_create(&producer,NULL,myproducer,NULL);
    pthread_create(&consumer_1,NULL,myconsumer,&a);
    pthread_create(&consumer_2,NULL,myconsumer,&b);

    pthread_join(producer,NULL);
    pthread_join(consumer_1,NULL);
    pthread_join(consumer_2,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&is_empty);
    pthread_cond_destroy(&is_full);

    pthread_exit(NULL);
    return 0;
}