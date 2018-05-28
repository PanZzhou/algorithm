// 再编一个同步互斥的Pthreads程序， 在一个队列中（可以用数组）一个线程随机产生正整数放入队列中， 一个线程从队列中读奇数，另一个线程读偶数，
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define Max_size 5
int Cycles = 100;
int k = 0;
int odd_capacity = 0;
int even_capacity = 0;
int buffer[Max_size];
int finish = 0;
pthread_mutex_t mutex;
pthread_cond_t is_full,is_odd,is_even;

void * myproducer(void * rank){
    while(k++<Cycles){
        pthread_mutex_lock(&mutex);
        while(odd_capacity+even_capacity==Max_size){
            pthread_cond_wait(&is_full,&mutex);
        }
        int random = rand()%100+1;
        printf("producer put %d\n",random);
        for(int i=0;i<Max_size;i++){
            if(buffer[i]==0){
                buffer[i] = random;
                break;
            }
        }
        if(random%2==0){
            even_capacity++;
            pthread_cond_signal(&is_even);
            if(k==Cycles){
                finish = 1;
                odd_capacity++;
                even_capacity++;
                pthread_cond_signal(&is_odd);
            }
        }
        if(random%2!=0){
            odd_capacity++;
            pthread_cond_signal(&is_odd);
            if(k==Cycles){
                finish = 1;
                odd_capacity++;
                even_capacity++;
                pthread_cond_signal(&is_even);
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    printf("%s\n", "producer finished");
    return NULL;
}

void* myconsumer_odd(void * rank){
    while(1){
        pthread_mutex_lock(&mutex);
        while(odd_capacity==0){
            pthread_cond_wait(&is_odd,&mutex);
        }
        if(odd_capacity==1&&finish==1){
            pthread_mutex_unlock(&mutex);
            break;
        }
        int index = -1;
        for(int i=0;i<Max_size;i++){
            if(buffer[i]!=0&&buffer[i]%2!=0)
                index = i;
        }
        printf("consumer_odd get %d\n",buffer[index]);
        odd_capacity--;
        buffer[index] = 0;
        pthread_cond_signal(&is_full);
        pthread_mutex_unlock(&mutex);
    }
    printf("%s\n", "odd finished");
    return NULL;
}
void* myconsumer_even(void * rank){
    while(1){
        pthread_mutex_lock(&mutex);
        while(even_capacity==0){
            pthread_cond_wait(&is_even,&mutex);
        }
        if(even_capacity==1&&finish==1){
            pthread_mutex_unlock(&mutex);
            break;
        }
        int index = -1;
        for(int i=0;i<Max_size;i++){
            if(buffer[i]!=0&&buffer[i]%2==0)
                index = i;
        }
        printf("consumer_even get %d\n",buffer[index]);
        even_capacity--;
        buffer[index] = 0;
        pthread_cond_signal(&is_full);
        pthread_mutex_unlock(&mutex);
    }
    printf("%s\n", "even finished");
    return NULL;
}

int main(){
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&is_full,NULL);
    pthread_cond_init(&is_odd,NULL);
    pthread_cond_init(&is_even,NULL);
    pthread_t producer,consumer_odd,consumer_even;
    
    pthread_create(&producer,NULL,myproducer,NULL);
    pthread_create(&consumer_odd,NULL,myconsumer_odd,NULL);
    pthread_create(&consumer_even,NULL,myconsumer_even,NULL);

    pthread_join(producer,NULL);
    pthread_join(consumer_odd,NULL);
    pthread_join(consumer_even,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&is_odd);
    pthread_cond_destroy(&is_even);
    pthread_cond_destroy(&is_full);

    pthread_exit(NULL);
    return 0;
}