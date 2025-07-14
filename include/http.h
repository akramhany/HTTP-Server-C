#ifndef HTTP
#define HTTP

#include "constants.h"

typedef struct {
  char *key;
  char *value;

} Header;

/**
 * Construct a Header struct.
 *
 * @param key Header key string.
 * @param value Header value string.
 *
 * Allocates and initializes a Header struct with the given key and value.
 */
Header *header_constructor(char *key, char *value);
void header_stringify(Header *header, char buf[]);
void header_free(Header *header);

typedef struct {
  Header **headers;
  int headers_count;

} Headers;

/**
 * Construct a Headers struct.
 *
 * @param header_arr Array of pointers to Header structs.
 * @param headers_count Number of headers in the array.
 *
 * Allocates and initializes a Headers struct containing an array of Header pointers.
 */
Headers *headers_constructor(Header **header_arr, int headers_count);

/**
 * Convert all headers to a string.
 *
 * @param headers Pointer to Headers struct.
 * @param buf Output buffer to store the stringified headers.
 *
 * Converts all headers in a Headers struct into a single string, each followed by CRLF, and stores it in buf.
 */
void headers_stringify(Headers *headers, char buf[]);

/**
 * Free a Headers struct.
 *
 * @param headers Pointer to Headers struct to free.
 *
 * Frees the memory allocated for a Headers struct and all contained Header structs.
 */
void headers_free(Headers *headers);

//////////////////////////////////////////////////////
/////////////////// Request Data /////////////////////
//////////////////////////////////////////////////////

typedef struct {
  char *method;
  char *path;
  char *http_version;

} RequestLine;

/**
 * Construct a RequestLine struct.
 *
 * @param http_version HTTP version string.
 * @param method HTTP method string.
 * @param path Request path string.
 *
 * Allocates and initializes a RequestLine struct with the given HTTP version, method, and path.
 */
RequestLine *request_line_constructor(char *http_version, char *method, char *path);
void request_line_free(RequestLine *request_line);

typedef struct {

  RequestLine *request_line;
  Headers *headers;
  char *body;

} Request;

/**
 * Construct a Request struct.
 *
 * @param request_line Pointer to RequestLine struct.
 * @param headers Pointer to Headers struct.
 * @param body Request body string.
 *
 * Allocates and initializes a Request struct with the given request line, headers, and body.
 */
Request *request_constructor(RequestLine *request_line, Headers *headers, char *body);
void request_free(Request *request);

//////////////////////////////////////////////////////
/////////////////// Response Data ////////////////////
//////////////////////////////////////////////////////

typedef struct {
  char *http_version;
  int status_code;
  char *reason_phrase;

} StatusLine;

/**
 * Construct a StatusLine struct.
 *
 * @param http_version HTTP version string.
 * @param status_code HTTP status code.
 * @param reason_phrase Reason phrase string.
 *
 * Allocates and initializes a StatusLine struct with the given HTTP version, status code, and reason phrase.
 */
StatusLine *status_line_constructor(char *http_version, int status_code, char *reason_phrase);
void status_line_stringify(StatusLine *status_line, char buf[]);
void status_line_free(StatusLine *status_line);

typedef struct {

  StatusLine *status_line;
  Headers *headers;
  char *body;
  int body_len;

} Response;

/**
 * Construct a Response struct.
 *
 * @param status_line Pointer to StatusLine struct.
 * @param headers Pointer to Headers struct.
 * @param body Response body data.
 * @param body_len Length of the response body.
 *
 * Allocates and initializes a Response struct with the given status line, headers, and body.
 */
Response *response_constructor(StatusLine *status_line, Headers *headers, char *body, int body_len);

/**
 * Convert a Response struct to a string.
 *
 * @param response Pointer to Response struct.
 * @param buf Output buffer to store the stringified response.
 * @param buffer_size Size of the output buffer.
 *
 * Converts a Response struct into a string suitable for sending over the network, including status line, headers, and body. Returns the total size of the stringified response (including body).
 */
int response_stringify(Response *response, char buf[], int buffer_size);

/**
 * Free a Response struct.
 *
 * @param response Pointer to Response struct to free.
 *
 * Frees the memory allocated for a Response struct and its fields.
 */
void response_free(Response *response);

//////////////////////////////////////////////////////
/////////////////// HTTP Helpers /////////////////////
//////////////////////////////////////////////////////

/**
 * Get the value of a header from a Request.
 *
 * @param request Pointer to Request struct.
 * @param header Header key to search for.
 *
 * Searches for a header in the request and returns its value if found, or NULL if not found.
 */
char *get_header_value(Request *request, char *header);

#endif
