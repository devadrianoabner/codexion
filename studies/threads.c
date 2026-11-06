#include <stdio.h>
#include <pthread.h>

typedef struct s_data
{
    int counter;
    pthread_mutex_t lock;
} t_data;

void    *routine(void *arg)
{
    t_data *data;
    int i;

    data = (t_data *)arg;
    i = 0;
    while (i < 100000)
    {
        pthreade_mutex_lock(&data->lock);
        data->counter++;
        pthread_mutex_unlock(&data->lock);
        i++;
    }
    return (NULL);
}

int main(void)
{
    pthread_t thread_1;
    pthread_t thread_2;
    t_data     data;

    data.counter = 0;
    pthread_mutex_init(&data.lock, NULL);
    pthread_create(&thread_1, NULL, &routine, &data);
    pthread_create(&thread_2, NULL, &routine, &data);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_mutex_destroy(&data.lock);
    printf("Final counter value: %d\n", data.counter);
    return (0);
}