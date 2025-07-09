#include "http.h"
#include "defines.h"
#include "utils.h"

Headers *headers_constructor(char *host, char *user_agent, char *accept) {
  Headers *headers = malloc(sizeof(Headers));

  headers->host = NULL;
  if (host) {
    headers->host = malloc(strlen(host) + 1);
    strcpy(headers->host, host);
  }

  headers->user_agent = NULL;
  if (user_agent) {
    headers->user_agent = malloc(strlen(user_agent) + 1);
    strcpy(headers->user_agent, user_agent);
  }

  headers->accept = NULL;
  if (accept) {
    headers->accept = malloc(strlen(accept) + 1);
    strcpy(headers->accept, accept);
  }

  return headers;
}

void headers_stringify(Headers *headers, char buf[]) {
  buf[0] = '\0';

  if (headers->host) {
    strcat(buf, "Host: ");
    strcat(buf, headers->host);
    strcat(buf, CRLF);
  }

  if (headers->user_agent) {
    strcat(buf, "User-Agent: ");
    strcat(buf, headers->user_agent);
    strcat(buf, CRLF);
  }

  if (headers->accept) {
    strcat(buf, "Accept: ");
    strcat(buf, headers->accept);
    strcat(buf, CRLF);
  }
}

void headers_free(Headers *headers) {
  if (!headers)
    return;

  free(headers->host);
  free(headers->user_agent);
  free(headers->accept);

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
  free(request_line->path);
  free(request_line->http_version);

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
  headers_free(request->headers);
  free(request->body);

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
  free(status_line->reason_phrase);
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
  headers_free(response->headers);
  free(response->body);

  free(response);
}
