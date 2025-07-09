#ifndef RESPONSE
#define RESPONSE

#include "constants.h"

typedef struct {

  char status_line[MAX_BUFFER_SIZE];
  char headers[MAX_BUFFER_SIZE];
  char body[MAX_BUFFER_SIZE];

} Response;

Response *response_constructor(char *status_line, char *headers, char *body);
void stringfy_response(Response *response, char buf[], int buffer_size);

#endif
