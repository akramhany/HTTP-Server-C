#include "handler.h"
#include "defines.h"
#include "utils.h"
#include "parser.h"

void handle_connection(int server_fd, const char *dir_path) {
  /*
   * 1. read request info.
   * 2. parse request.
   * 3. based on the request content do something.
   * 4. construct the response.
   * 5. send the response.
   * */

  const int MAX_REQUEST_INFO_SIZE = 4096;
  const int MAX_RESPONSE_STR_SIZE = 4096;
  char request_info[MAX_REQUEST_INFO_SIZE];

  recv(server_fd, request_info, MAX_REQUEST_INFO_SIZE, 0);

  // Parse request.
  Request *request = parse(request_info);

  // Create response
  Response *response = router(request, dir_path);

  char msg[MAX_RESPONSE_STR_SIZE];
  response_stringify(response, msg, MAX_RESPONSE_STR_SIZE);

  send(server_fd, msg, strlen(msg), 0);

  request_free(request);
  response_free(response);
}

Response *router(Request *request, const char *dir_path) {
  const char *default_path = "/";
  const char *echo_path = "/echo/";
  const char *user_agent_path = "/user-agent";
  const char *files_path = "/files";

  if (strncmp(echo_path, request->request_line->path, strlen(echo_path)) == 0) {
    return handle_echo(request);
  } else if (strncmp(user_agent_path, request->request_line->path, strlen(user_agent_path)) == 0) {
    return handle_user_agent(request);
  } else if (strncmp(files_path, request->request_line->path, strlen(files_path)) == 0) {
    return handle_files(request, dir_path);
  } else if (strcmp(default_path, request->request_line->path) == 0) {
    return handle_default_path(request);
  }

  return handle_invalid_path(request);
}

Response *handle_echo(Request *request) {
  shift_string_left(&(request->request_line->path), "/echo/");

  char content_length[16];
  sprintf(content_length, "%ld", strlen(request->request_line->path));

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, 200, "OK");

  Header **header_arr = malloc(2 * sizeof(Header *));
  header_arr[0] = header_constructor("Content-Type", "text/plain");
  header_arr[1] = header_constructor("Content-Length", content_length);
  Headers *headers = headers_constructor(header_arr, 2);

  return response_constructor(status_line, headers, request->request_line->path);
}

Response *handle_user_agent(Request *request) {
  char *value = get_header_value(request, "User-Agent");
  if (!value) {
    error("Couldn't find header!\n", -1);
  }

  char content_length[16];
  sprintf(content_length, "%ld", strlen(value));

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, 200, "OK");

  Header **header_arr = malloc(2 * sizeof(Header *));
  header_arr[0] = header_constructor("Content-Type", "text/plain");
  header_arr[1] = header_constructor("Content-Length", content_length);
  Headers *headers = headers_constructor(header_arr, 2);

  return response_constructor(status_line, headers, value);
}

Response *handle_files(Request *request, const char *dir_path) {
  shift_string_left(&(request->request_line->path), "/files/");

  if (strcmp(request->request_line->method, "GET") == 0) {    // handle get request
    return handle_files_get(request, dir_path, request->request_line->path);
  } else {    // handle post request
    return handle_files_post(request, dir_path,request->request_line->path );
  }
}

Response *handle_invalid_path(Request *request) {
  int status_code = 404;
  char *reason_phrase = "Not Found";

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, status_code, reason_phrase);

  return response_constructor(status_line, NULL, NULL);
}

Response *handle_default_path(Request *request) {
  int status_code = 200;
  char *reason_phrase = "OK";

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, status_code, reason_phrase);

  return response_constructor(status_line, NULL, NULL);
}

Response *handle_files_get(Request *request, const char *dir_path,
                           const char *file_name) {
  char file_path[MAX_BUFFER_SIZE];
  snprintf(file_path, sizeof(file_path), "%s%s", dir_path, file_name);

  if (file_exists_in_dir(dir_path, file_name) != 1) {
    return handle_invalid_path(request);
  }

  FileContent *file_content = read_file(file_path);

  char content_length[16];
  sprintf(content_length, "%d", file_content->file_size);

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, 200, "OK");

  Header **header_arr = malloc(2 * sizeof(Header *));
  header_arr[0] = header_constructor("Content-Type", "application/octet-stream");
  header_arr[1] = header_constructor("Content-Length", content_length);
  Headers *headers = headers_constructor(header_arr, 2);

  return response_constructor(status_line, headers, file_content->data);
}

Response *handle_files_post(Request *request, const char *dir_path,
                            const char *file_name) {
  write_file(dir_path, file_name, request->body);

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, 201, "Created");

  return response_constructor(status_line, NULL, NULL);
}
