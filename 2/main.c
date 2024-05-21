#include <stdio.h>
#include <pthread.h>

int counter = 0; // Shared variable between threads

pthread_mutex_t mutex; // Mutex for critical section

void *increment(void *arg)
{
    for (int i = 0; i < 100000; i++)
    {
        // Lock mutex (wait until available)
        pthread_mutex_lock(&mutex);
        // Critical section (unsafe without mutex)
        counter++;
        // Unlock mutex
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t threads[10];

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < 10; i++)
    {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    // Wait for threads to finish
    for (int i = 0; i < 10; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print final counter value (should be 200000)
    printf("Final counter: %d\n", counter);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
