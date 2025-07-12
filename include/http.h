#ifndef HTTP
#define HTTP

#include "constants.h"

typedef struct {
  char *key;
  char *value;

} Header;

Header *header_constructor(char *key, char *value);
void header_stringify(Header *header, char buf[]);
void header_free(Header *header);

typedef struct {
  Header **headers;
  int headers_count;

} Headers;

Headers *headers_constructor(Header **header_arr, int headers_count);
void headers_stringify(Headers *headers, char buf[]);
void headers_free(Headers *headers);

//////////////////////////////////////////////////////
/////////////////// Request Data /////////////////////
//////////////////////////////////////////////////////

typedef struct {
  char *method;
  char *path;
  char *http_version;

} RequestLine;

RequestLine *request_line_constructor(char *http_version, char *method,
                                      char *path);
void request_line_free(RequestLine *request_line);

typedef struct {

  RequestLine *request_line;
  Headers *headers;
  char *body;

} Request;

Request *request_constructor(RequestLine *request_line, Headers *headers,
                             char *body);
void request_free(Request *request);

//////////////////////////////////////////////////////
/////////////////// Response Data ////////////////////
//////////////////////////////////////////////////////

typedef struct {
  char *http_version;
  int status_code;
  char *reason_phrase;

} StatusLine;

StatusLine *status_line_constructor(char *http_version, int status_code,
                                    char *reason_phrase);
void status_line_stringify(StatusLine *status_line, char buf[]);
void status_line_free(StatusLine *status_line);

typedef struct {

  StatusLine *status_line;
  Headers *headers;
  char *body;

} Response;

Response *response_constructor(StatusLine *status_line, Headers *headers,
                               char *body);
void response_stringify(Response *response, char buf[], int buffer_size);
void response_free(Response *response);

//////////////////////////////////////////////////////
/////////////////// HTTP Helpers /////////////////////
//////////////////////////////////////////////////////

char *get_header_value(Request *request, char *header);

#endif
