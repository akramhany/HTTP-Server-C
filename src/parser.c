#include "parser.h"
#include "defines.h"

Request *parse(char *request_info) {
  // Parse request_line
  char *tokens[MAX_BUFFER_SIZE];
  int tokens_count = 0;

  tokens[tokens_count++] = strtok(request_info, CRLF);
  while (tokens[tokens_count - 1] != NULL) {
    tokens[tokens_count++] = strtok(NULL, CRLF);
  }

  // Parse request line
  RequestLine *request_line = parse_request_line(tokens[0]);

  // Parse headers
  Headers *headers = parse_headers(&tokens[1], tokens_count - 2);

  // Parse body
  char *body = tokens[tokens_count - 1];

  // Create and return request
  Request *request = request_constructor(request_line, headers, body);

  return request;
}

RequestLine *parse_request_line(char *request_line) {
  char *method = NULL, *path = NULL, *http_version = NULL;
  char *delim = " ";
  char *token = strtok(request_line, delim);

  if (token) {
    method = token;
  }

  token = strtok(NULL, delim);
  if (token) {
    path = token;
  }

  token = strtok(NULL, delim);
  if (token) {
    http_version = token;
  }

  return request_line_constructor(http_version, method, path);
}

Headers *parse_headers(char *headers[], int size) {
  char *host = NULL, *user_agent = NULL, *accept = NULL;
  char *delim = " ";

  for (int i = 0; i < size; i++) {
    char *key = strtok(headers[i], delim);
    char *value = strtok(NULL, delim);

    if (strcmp(key, "Host:") == 0) {
      host = value;
    } else if (strcmp(key, "User-Agent:") == 0) {
      user_agent = value;
    } else if (strcmp(key, "Accept:") == 0) {
      accept = value;
    }
  }

  return headers_constructor(host, user_agent, accept);
}
