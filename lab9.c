// Nisal Panditha
// CMPT201 Lab9

// client.c

/*

1. What is the address of the server?
   The IP address is 127.0.0.1 and the port is 8000.

2. Is it UDP or TCP? How do you know?
   It is TCP because socket() uses SOCK_STREAM.

3. Where does the client get its data?
   The client gets data from the keyboard.
   read(STDIN_FILENO, buf, BUF_SIZE) reads user input.
   write(sfd, buf, num_read) sends it to the server.

4. How does the client program end?
   The program ends when read() returns 1 or 0.
   Pressing Enter on an empty line makes read()
   return 1 in the normal terminal setup.
   The loop stops, close(sfd) closes the socket,
   and exit(EXIT_SUCCESS) ends the program.
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  // Create TCP socket
  sfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sfd == -1) {
    handle_error("socket");
  }

  // Set server address
  memset(&addr, 0, sizeof(struct sockaddr_in));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  // Connect to server
  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

  if (res == -1) {
    handle_error("connect");
  }

  // Read keyboard input and send it
  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {

    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }

    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  // Close socket
  close(sfd);

  exit(EXIT_SUCCESS);
}

// server.c

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

// Shared counters
int total_message_count = 0;
int client_id_counter = 1;

// Mutexes to protect shared counters
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

// Client information
struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  // Get client information
  int cfd = client->cfd;
  int client_id = client->client_id;

  free(client);

  char buf[BUF_SIZE + 1];
  ssize_t num_read;

  // Keep receiving messages
  while ((num_read = read(cfd, buf, BUF_SIZE)) > 0) {

    // Make received data a string
    buf[num_read] = '\0';

    // Lock shared message counter
    pthread_mutex_lock(&count_mutex);

    total_message_count++;

    // Print received message
    printf("Msg # %3d; Client ID %d: %s", total_message_count, client_id, buf);

    if (buf[num_read - 1] != '\n') {
      printf("\n");
    }

    fflush(stdout);

    // Unlock counter
    pthread_mutex_unlock(&count_mutex);
  }

  if (num_read == -1) {
    perror("read");
  }

  // Close client socket
  close(cfd);

  printf("Ending thread for client %d\n", client_id);

  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  // Create TCP socket
  sfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sfd == -1) {
    handle_error("socket");
  }

  // Set server address
  memset(&addr, 0, sizeof(struct sockaddr_in));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind socket
  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  // Listen for clients
  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  // Accept clients forever
  for (;;) {

    // Accept new client
    int cfd = accept(sfd, NULL, NULL);

    if (cfd == -1) {
      perror("accept");
      continue;
    }

    // Allocate client information
    struct client_info *client = malloc(sizeof(struct client_info));

    if (client == NULL) {
      perror("malloc");
      close(cfd);
      continue;
    }

    client->cfd = cfd;

    // Assign client ID
    pthread_mutex_lock(&client_id_mutex);

    client->client_id = client_id_counter;
    client_id_counter++;

    pthread_mutex_unlock(&client_id_mutex);

    printf("New client created! ID %d on socket FD %d\n", client->client_id, cfd);

    fflush(stdout);

    // Create new thread
    pthread_t thread;

    int result = pthread_create(&thread, NULL, handle_client, client);

    if (result != 0) {
      fprintf(stderr, "pthread_create: %s\n", strerror(result));
      close(cfd);
      free(client);
      continue;
    }

    // Detach thread
    result = pthread_detach(thread);

    if (result != 0) {
      fprintf(stderr, "pthread_detach: %s\n", strerror(result));
    }
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
