#include "parser.h"
#include "defines.h"

Request *parse(char *request_info) {
  char *header_part = strstr(request_info, "\r\n\r\n");
  char *body_part = NULL;

  if (header_part) {
    *header_part = '\0';
    body_part = header_part + 4;
    header_part = request_info;
  } else {
    return NULL;
  }

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
  char *body = body_part;

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
  char *delim = ": ";
  Header **header_arr = malloc(size * sizeof(Header *));

  for (int i = 0; i < size; i++) {
    char *ptr = strstr(headers[i], delim);
    ptr[0] = '\0';

    char *key = headers[i];
    char *value = ptr + 2;      // skip ": " chars

    Header *header = header_constructor(key, value);

    header_arr[i] = header;
  }

  return headers_constructor(header_arr, size);
}
