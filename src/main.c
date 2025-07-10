#include "defines.h"
#include "http.h"
#include "parser.h"
#include "utils.h"

void handle_connection(int server_fd) {
  /*
   * 1. read request info.
   * 2. parse request.
   * 3. based on the request content do something.
   * 4. construct the response.
   * 5. send the response.
   * */

  const int MAX_REQUEST_INFO_SIZE = 4096;
  char request_info[MAX_REQUEST_INFO_SIZE];

  recv(server_fd, request_info, MAX_REQUEST_INFO_SIZE, 0);

  // Parse request.
  Request *request = parse(request_info);

  char *reason_phrase = NULL;
  int status_code = -1;

  if (strcmp(request->request_line->path, "/") == 0) {
    status_code = 200;
    reason_phrase = "OK";
  } else {
    status_code = 404;
    reason_phrase = "Not Found";
  }

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, status_code, reason_phrase);

  // Create response
  Response *response = response_constructor(status_line, NULL, NULL);

  char msg[MAX_BUFFER_SIZE];
  response_stringify(response, msg, MAX_BUFFER_SIZE);

  send(server_fd, msg, strlen(msg), 0);

  request_free(request);
  response_free(response);
}

int main(int argc, char *argv[]) {
  int server_fd, client_addr_len, new_server_fd, port_no;
  struct sockaddr_in client_addr;

  if (argc != 2) {
    error("Usage: ./program port_number\n", 1);
  }

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

  port_no = atoi(argv[1]);

  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port_no),
      .sin_addr = {htonl(INADDR_ANY)},
  };

  if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("Bind failed: %s \n", strerror(errno));
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    printf("Listen failed: %s \n", strerror(errno));
    return 1;
  }

  printf("Waiting for a client to connect...\n");
  client_addr_len = sizeof(client_addr);

  new_server_fd =
      accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
  printf("Client connected\n");

  handle_connection(new_server_fd);

  close(new_server_fd);
  close(server_fd);

  return 0;
}
