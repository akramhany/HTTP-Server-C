#include "defines.h"
#include "http.h"
#include "parser.h"
#include "utils.h"
#include "handler.h"

int main(int argc, char *argv[]) {
  int server_fd, client_addr_len, new_server_fd, port_no;
  struct sockaddr_in client_addr;
  char *dir_path = NULL;

  if (argc != 4) {
    error("Usage: ./program --directory dir_path port_number\n", 1);
  }

  port_no = atoi(argv[3]);
  dir_path = argv[2];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    printf("Socket creation failed: %s...\n", strerror(errno));
    return 1;
  }

  // Since the tester restarts your program quite often, setting
  SO_REUSEADDR;
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    printf("SO_REUSEADDR failed: %s \n", strerror(errno));
    return 1;
  }

  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port_no),
      .sin_addr = {htonl(INADDR_ANY)},
  };

  if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("Bind failed: %s \n", strerror(errno));
    return 1;
  }

  int connection_backlog = 10;
  if (listen(server_fd, connection_backlog) != 0) {
    printf("Listen failed: %s \n", strerror(errno));
    return 1;
  }

  client_addr_len = sizeof(client_addr);

  while (1) {
    printf("Waiting for a client to connect...\n");
    new_server_fd =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (new_server_fd < 0) {
      error("accept failed", -1);
      continue;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        close(new_server_fd);
        continue;
    }

    if (pid == 0) {
        // Child process
        close(server_fd);  // child doesn't need the listening socket
        handle_connection(new_server_fd, dir_path);
        close(new_server_fd);
        exit(0);  // child exits after handling
    } else {
        // Parent process
        close(new_server_fd);  // parent doesn't need the client socket
    }
  }

  close(server_fd);

  return 0;
}
