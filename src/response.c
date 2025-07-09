#include "response.h"
#include "defines.h"
#include "utils.h"

Response *response_constructor(char *status_line, char *headers, char *body) {
  Response *response = malloc(sizeof(Response));

  if (status_line) {
    if (strlen(status_line) + strlen(CRLF) >= MAX_BUFFER_SIZE) {
      error("Failed to copy string, destination length is less than source "
            "length\n",
            -1);
    }
    strcpy(response->status_line, status_line);
  }
  strcat(response->status_line, CRLF);

  if (headers) {
    if (strlen(headers) + strlen(CRLF) >= MAX_BUFFER_SIZE) {
      error("Failed to copy string, destination length is less than source "
            "length\n",
            -1);
    }
    strcpy(response->headers, headers);
  }
  strcat(response->headers, CRLF);

  if (body) {
    if (strlen(body) >= MAX_BUFFER_SIZE) {
      error("Failed to copy string, destination length is less than source "
            "length\n",
            -1);
    }
    strcpy(response->body, body);
  }

  return response;
}

void stringfy_response(Response *response, char buf[], int buffer_size) {
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
