// 再编一个同步互斥的Pthreads程序， 在一个队列中（可以用数组）一个线程随机产生正整数放入队列中， 一个线程从队列中读奇数，另一个线程读偶数，
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define Max_size 5
int Cycles = 100;
int k = 0;
int odd_capacity = 0;
int even_capacity = 0;
int buffer[Max_size];
int finish = 0;
pthread_mutex_t mutex;
sem_t empty,odd_full,even_full;

void * myproducer(void * rank){
    while(k++<Cycles){
        sem_wait(&empty);
        int random = rand()%100+1;
        pthread_mutex_lock(&mutex);
        printf("producer put %d\n",random);
        for(int i=0;i<Max_size;i++){
            if(buffer[i]==0){
                buffer[i] = random;
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        if(random%2!=0){
            odd_capacity++;
            sem_post(&odd_full);
        }
        if(random%2==0){
            even_capacity++;
            sem_post(&even_full);
        }
        if(k==Cycles){
                finish = 1;
                even_capacity++;
                odd_capacity++;
                sem_post(&odd_full);
                sem_post(&even_full);
            }
    }
    printf("%s\n", "producer finished");
    return NULL;
}

void* myconsumer_odd(void * rank){
    while(1){
        sem_wait(&odd_full);
        if(finish==1&&odd_capacity==1){
            break;
        }
        pthread_mutex_lock(&mutex);
        int index = -1;
        for(int i=0;i<Max_size;i++){
            if(buffer[i]!=0&&buffer[i]%2!=0)
                index = i;
        }
        printf("consumer_odd get %d\n",buffer[index]);
        buffer[index] = 0;
        odd_capacity--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    printf("%s\n", "odd finished");
    return NULL;
}
void* myconsumer_even(void * rank){
    while(1){
        sem_wait(&even_full);
        if(finish==1&&even_capacity==1)
            break;
        pthread_mutex_lock(&mutex);
        int index = -1;
        for(int i=0;i<Max_size;i++){
            if(buffer[i]!=0&&buffer[i]%2==0)
                index = i;
        }
        printf("consumer_even get %d\n",buffer[index]);
        buffer[index] = 0;
        even_capacity--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    printf("%s\n", "even finished");
    return NULL;
}

int main(){
    pthread_mutex_init(&mutex,NULL);
    sem_init(&empty,0,Max_size);
    sem_init(&odd_full,0,0);
    sem_init(&even_full,0,0);
    pthread_t producer,consumer_odd,consumer_even;
    
    pthread_create(&producer,NULL,myproducer,NULL);
    pthread_create(&consumer_odd,NULL,myconsumer_odd,NULL);
    pthread_create(&consumer_even,NULL,myconsumer_even,NULL);

    pthread_join(producer,NULL);
    pthread_join(consumer_odd,NULL);
    pthread_join(consumer_even,NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&odd_full);
    sem_destroy(&even_full);

    pthread_exit(NULL);
    return 0;
}