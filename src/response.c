#include "response.h"
#include "defines.h"
#include "utils.h"

Response *response_constructor(char *status_line, char *headers, char *body) {
  Response *response = malloc(sizeof(Response));

  response->status_line = malloc(strlen(status_line) + strlen(CRLF) + 1);
  strcpy(response->status_line, status_line);
  strcat(response->status_line, CRLF);

  response->headers = malloc(strlen(headers) + strlen(CRLF) + 1);
  strcpy(response->headers, headers);
  strcat(response->headers, CRLF);

  response->body = malloc(strlen(body) + 1);
  strcpy(response->body, body);

  return response;
}

void response_stringify(Response *response, char buf[], int buffer_size) {
  if (strlen(response->status_line) + strlen(response->headers) +
          strlen(response->body) >=
      buffer_size) {
    error("Failed to stringfy response, destination length is less than source "
          "lengths\n",
          -1);
  }

  strcpy(buf, response->status_line);
  strcat(buf, response->headers);
  strcat(buf, response->body);
}

void response_free(Response *response) {

  free(response->status_line);
  free(response->headers);
  free(response->body);

  free(response);
}
