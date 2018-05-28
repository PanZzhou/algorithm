#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

#define thread_count   4
#define fragment 80000000
double sum;
sem_t bin_sem;

void* start_thread(void *rank)
{
    long long i ;
    long my_rank = rank;
    long long my_n = fragment/thread_count ;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i + my_n;
    double my_sum = 0.0;
    double delta = 1.0/fragment;

    for(i = my_first_i;i<my_last_i;i++)
    {
        double x = (double)i/fragment;
        my_sum += delta/(1.0+x*x);
    }
    my_sum*=4;
    sem_wait(&bin_sem);
    sum  += my_sum;
    sem_post(&bin_sem);
    return NULL;
}

int main(void)
{
    long thread;
    pthread_t thread_handles[thread_count];

    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);
    sem_init(&bin_sem,0,1);
    for(thread=0;thread<thread_count;thread++)
    {
        pthread_create(&thread_handles[thread], NULL, start_thread, (void *)thread); 
    }

    printf("main function\n");

    for(thread=0;thread<thread_count;thread++) 
    {
        pthread_join(thread_handles[thread], NULL);
    }
    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;
    printf("sum  is:%lf,and run time is: %fs \n",sum,timeuse);

    sem_destroy(&bin_sem);
    pthread_exit(NULL);

    return 0;
}