#ifndef RESPONSE
#define RESPONSE

#include "constants.h"

typedef struct {

  char *status_line;
  char *headers;
  char *body;

} Response;

Response *response_constructor(char *status_line, char *headers, char *body);
void response_stringify(Response *response, char buf[], int buffer_size);
void response_free(Response *response);

#endif
