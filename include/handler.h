#ifndef ROUTER
#define ROUTER

#include "http.h"

void handle_connection(int server_fd);

Response *router(Request *request);
Response *handle_echo(Request *request);
Response *handle_invalid_path(Request *request);
Response *handle_default_path(Request *request);

#endif
