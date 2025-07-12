#include "http.h"
#include "defines.h"
#include "utils.h"

Header *header_constructor(char *key, char *value) {
  Header *header = malloc(sizeof(Header));

  header->key = NULL;
  if (key) {
    header->key = malloc(strlen(key) + 1);
    strcpy(header->key, key);
  }

  header->value = NULL;
  if (value) {
    header->value = malloc(strlen(value) + 1);
    strcpy(header->value, value);
  }

  return header;
}

void header_stringify(Header *header, char buf[]) {
  if (!header)
    return;

  buf[0] = '\0';

  strcat(buf, header->key);
  strcat(buf, ": ");
  strcat(buf, header->value);
}

void header_free(Header *header) {
  if (!header)
    return;

  free(header->key);
  free(header->value);

  free(header);
}

Headers *headers_constructor(Header **header_arr, int headers_count) {
  Headers *headers = malloc(sizeof(Headers));

  headers->headers = NULL;
  if (header_arr) {
    headers->headers = header_arr;
  }

  headers->headers_count = headers_count;

  return headers;
}

void headers_stringify(Headers *headers, char buf[]) {
  int size = headers->headers_count;

  for (int i = 0; i < size; i++) {
    char temp_buf[MAX_BUFFER_SIZE];

    header_stringify(headers->headers[i], temp_buf);
    strcat(buf, temp_buf);
    strcat(buf, CRLF);
  }
}

void headers_free(Headers *headers) {
  if (!headers)
    return;

  int size = headers->headers_count;

  for (int i = 0; i < size; i++) {
    header_free(headers->headers[i]);
  }

  free(headers->headers);
  free(headers);
}

RequestLine *request_line_constructor(char *http_version, char *method,
                                      char *path) {
  RequestLine *request_line = malloc(sizeof(RequestLine));

  request_line->method = NULL;
  if (method) {
    request_line->method = malloc(strlen(method) + 1);
    strcpy(request_line->method, method);
  }

  request_line->path = NULL;
  if (path) {
    request_line->path = malloc(strlen(path) + 1);
    strcpy(request_line->path, path);
  }

  request_line->http_version = NULL;
  if (http_version) {
    request_line->http_version = malloc(strlen(http_version) + 1);
    strcpy(request_line->http_version, http_version);
  }

  return request_line;
}

void request_line_free(RequestLine *request_line) {
  if (!request_line)
    return;

  free(request_line->method);
  request_line->method = NULL;

  free(request_line->path);
  request_line->path = NULL;

  free(request_line->http_version);
  request_line->http_version = NULL;

  free(request_line);
}

Request *request_constructor(RequestLine *request_line, Headers *headers,
                             char *body) {
  Request *request = malloc(sizeof(Request));

  request->request_line = NULL;
  if (request_line) {
    request->request_line = request_line;
  }

  request->headers = NULL;
  if (headers) {
    request->headers = headers;
  }

  request->body = NULL;
  if (body) {
    request->body = malloc(strlen(body) + 1);
    strcpy(request->body, body);
  }

  return request;
}

void request_free(Request *request) {
  if (!request)
    return;

  request_line_free(request->request_line);
  request->request_line = NULL;

  headers_free(request->headers);
  request->headers = NULL;

  free(request->body);
  request->body = NULL;

  free(request);
}

StatusLine *status_line_constructor(char *http_version, int status_code,
                                    char *reason_phrase) {
  StatusLine *status_line = malloc(sizeof(StatusLine));

  status_line->status_code = status_code;

  status_line->http_version = NULL;
  if (http_version) {
    status_line->http_version = malloc(strlen(http_version) + 1);
    strcpy(status_line->http_version, http_version);
  }

  status_line->reason_phrase = NULL;
  if (reason_phrase) {
    status_line->reason_phrase = malloc(strlen(reason_phrase) + 1);
    strcpy(status_line->reason_phrase, reason_phrase);
  }

  return status_line;
}

void status_line_stringify(StatusLine *status_line, char buf[]) {
  char status_code_str[20];
  buf[0] = '\0';

  sprintf(status_code_str, "%d", status_line->status_code);

  strcat(buf, status_line->http_version);
  strcat(buf, " ");
  strcat(buf, status_code_str);
  if (status_line->reason_phrase) {
    strcat(buf, " ");
    strcat(buf, status_line->reason_phrase);
  }
}

void status_line_free(StatusLine *status_line) {
  if (!status_line)
    return;

  free(status_line->http_version);
  status_line->http_version = NULL;

  free(status_line->reason_phrase);
  status_line->reason_phrase = NULL;

  free(status_line);
}

Response *response_constructor(StatusLine *status_line, Headers *headers,
                               char *body) {
  Response *response = malloc(sizeof(Response));

  response->status_line = NULL;
  if (status_line) {
    response->status_line = status_line;
  }

  response->headers = NULL;
  if (headers) {
    response->headers = headers;
  }

  response->body = NULL;
  if (body) {
    response->body = malloc(strlen(body) + 1);
    strcpy(response->body, body);
  }

  return response;
}

void response_stringify(Response *response, char buf[], int buffer_size) {
  buf[0] = '\0';

  if (response->status_line) {
    status_line_stringify(response->status_line, buf);
  }
  strcat(buf, CRLF);

  if (response->headers) {
    headers_stringify(response->headers, buf);
  }
  strcat(buf, CRLF);

  if (response->body) {
    strcat(buf, response->body);
  }
}

void response_free(Response *response) {
  if (!response) {
    return;
  }

  status_line_free(response->status_line);
  response->status_line = NULL;

  headers_free(response->headers);
  response->headers = NULL;

  free(response->body);
  response->body = NULL;

  free(response);
}

char *get_header_value(Request *request, char *header) {
  Header **headers = request->headers->headers;
  int headers_size = request->headers->headers_count;

  for (int i = 0; i < headers_size; i++) {
    if (strcmp(headers[i]->key, header) == 0) {
      return headers[i]->value;
    }
  }

  return NULL;
}
