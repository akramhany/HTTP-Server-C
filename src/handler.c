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
  int msg_len = response_stringify(response, msg, MAX_RESPONSE_STR_SIZE);

  send(server_fd, msg, msg_len, 0);

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

  return send_response_with_headers(request, strlen(request->request_line->path),
                                    200, "OK",
                                    "text/plain", request->request_line->path);
}

Response *handle_user_agent(Request *request) {
  char *value = get_header_value(request, "User-Agent");
  if (!value) {
    error("Couldn't find header!\n", -1);
  }

  return send_response_with_headers(request, strlen(value),
                                    200, "OK",
                                    "text/plain", value);
}

Response *handle_invalid_path(Request *request) {
  int status_code = 404;
  char *reason_phrase = "Not Found";

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, status_code, reason_phrase);

  return response_constructor(status_line, NULL, NULL, 0);
}

Response *handle_default_path(Request *request) {
  int status_code = 200;
  char *reason_phrase = "OK";

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, status_code, reason_phrase);

  return response_constructor(status_line, NULL, NULL, 0);
}


////////////////////////////////////////////////////
///////////////////// FILES ////////////////////////
////////////////////////////////////////////////////
Response *handle_files(Request *request, const char *dir_path) {
  shift_string_left(&(request->request_line->path), "/files/");

  if (strcmp(request->request_line->method, "GET") == 0) {    // handle get request
    return handle_files_get(request, dir_path, request->request_line->path);
  } else {    // handle post request
    return handle_files_post(request, dir_path,request->request_line->path );
  }
}

Response *handle_files_get(Request *request, const char *dir_path,
                           const char *file_name) {
  char file_path[MAX_BUFFER_SIZE];
  snprintf(file_path, sizeof(file_path), "%s%s", dir_path, file_name);

  if (file_exists_in_dir(dir_path, file_name) != 1) {
    return handle_invalid_path(request);
  }

  FileContent *file_content = read_file(file_path);

  return send_response_with_headers(request, file_content->file_size,
                                    200, "OK",
                                    "application/octet-stream", file_content->data);
}

Response *handle_files_post(Request *request, const char *dir_path,
                            const char *file_name) {
  write_file(dir_path, file_name, request->body);

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, 201, "Created");

  return response_constructor(status_line, NULL, NULL, 0);
}


////////////////////////////////////////////////////
////////////////// GENERAL UTILS ///////////////////
////////////////////////////////////////////////////
Response *send_response_with_headers(Request *request, int content_length,
                                     int status_code, char *reason_phrase,
                                     char *content_type, char *body) {
  char content_length_str[16];

  StatusLine *status_line = status_line_constructor(
      request->request_line->http_version, status_code, reason_phrase);

  char *compression = get_header_value(request, "Accept-Encoding");
  int header_arr_size = 2;

  if (compression && strstr(compression, "gzip") != NULL) {
    header_arr_size = 3;
  }

  Header **header_arr = malloc(header_arr_size * sizeof(Header *));
  header_arr[0] = header_constructor("Content-Type", content_type);

  // If no compression is required, return normal response
  if (header_arr_size == 2) {
    sprintf(content_length_str, "%d", content_length);
    header_arr[1] = header_constructor("Content-Length", content_length_str);
    Headers *headers = headers_constructor(header_arr, header_arr_size);
    return response_constructor(status_line, headers, body, strlen(body) + 1);
  }

  header_arr[1] = header_constructor("Content-Encoding", "gzip");

  const int MAX_COMPRESSED_LEN = 4096;
  char compressed_body[MAX_COMPRESSED_LEN];
  content_length = MAX_COMPRESSED_LEN;

  gzip_compress(body, strlen(body), compressed_body, (size_t *)&content_length);

  sprintf(content_length_str, "%d", content_length);
  header_arr[2] = header_constructor("Content-Length", content_length_str);

  Headers *headers = headers_constructor(header_arr, header_arr_size);
  return response_constructor(status_line, headers, compressed_body, content_length);
}
