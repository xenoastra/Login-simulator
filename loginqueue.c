#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

sem_t semlog; // semaphore that keeps track of who is logged in
sem_t semusr; // semaphore that lets one user log in at a time
int expired;

void *queue(void *args) {
  
  char username[20];
  char password[20];
  char log[20];
  int id = *(int *)args;
  sem_wait(&semlog);
  sem_wait(&semusr);
  printf("\nLogin \n\n", *(int *)args);
  printf("User name: ", *(int *)args); // add the scanf
  scanf("%s", username);
  printf("Password: ", *(int *)args);
  scanf("%s", password);
  printf("\nUser %d Successfully Logged in!\n", *(int *)args);// add the username
  printf("Waiting in the login queue (Position #1)\n", *(int *)args); // add position in the login queue
  // printf("Type something to logout\n\n");
  // scanf("%s", log);
  sem_post(&semusr);
  if (expired == id) {
  printf("Oldest user has been logged out.\n", id);
  expired = -1;
  }
  else if (expired == -1) {
    expired = id;
  }
  
  sem_post(&semlog);

  free(args);

}

int main(int argc, char *argv[]) {
  pthread_t th[15]; // max 15 threads
  sem_init(&semlog, 0, 2); // 2 people logged in at once
  sem_init(&semusr, 0, 1); // allows one user to log in at a time
  for (int i = 0; i < 15; i++) {
    int *a = malloc(sizeof(int));
    *a = i;
    if (pthread_create(&th[i], NULL, &queue, a) != 0) {
      perror("Failed to create thread\n");
    }
  }

  for (int i = 0; i < 15; i++) {
    if (pthread_join(th[i], NULL) != 0) {
      perror("Failed to join thread\n");
    }
  }
  sem_destroy(&semlog);
  return 0;
}
//sem_wait is similar to a lock
//sem_post is like an unlock