#ifndef PARSER
#define PARSER

#include "http.h"

/**
 * Parse a raw HTTP request string.
 *
 * @param request_info Raw HTTP request string.
 *
 * Parses the raw HTTP request string into its components: request line, headers, and body. Returns a dynamically allocated Request struct, or NULL on failure.
 */
Request *parse(char *request_info);

/**
 * Parse an HTTP request line.
 *
 * @param request_line HTTP request line string (e.g., "GET / HTTP/1.1").
 *
 * Parses the HTTP request line into method, path, and HTTP version. Returns a dynamically allocated RequestLine struct.
 */
RequestLine *parse_request_line(char *request_line);

/**
 * Parse HTTP headers from an array of strings.
 *
 * @param headers Array of header strings (e.g., "Host: localhost").
 * @param size Number of headers in the array.
 *
 * Parses an array of header strings into Header structs and returns a dynamically allocated Headers struct containing all headers.
 */
Headers *parse_headers(char *headers[], int size);

#endif
