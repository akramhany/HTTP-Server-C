#ifndef PARSER
#define PARSER

#include "http.h"

Request *parse(char *request_info);
RequestLine *parse_request_line(char *request_line);
Headers *parse_headers(char *headers[], int size);

#endif
