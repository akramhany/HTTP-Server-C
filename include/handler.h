#ifndef ROUTER
#define ROUTER

#include "http.h"

void handle_connection(int server_fd, const char *dir_path);

Response *router(Request *request, const char *dir_path);
Response *handle_echo(Request *request);
Response *handle_user_agent(Request *request);
Response *handle_files(Request *request, const char *dir_path);
Response *handle_invalid_path(Request *request);
Response *handle_default_path(Request *request);

Response *handle_files_get(Request *request, const char *dir_path,
    const char *file_name);
Response *handle_files_post(Request *request, const char *dir_path,
    const char *file_name);

#endif
