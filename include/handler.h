#ifndef ROUTER
#define ROUTER

#include "http.h"

/**
 * Handle a client connection.
 *
 * @param server_fd Server socket file descriptor.
 * @param dir_path Directory path to serve files from.
 *
 * Reads and processes HTTP requests from a client, sends responses, and manages connection lifecycle.
 */
void handle_connection(int server_fd, const char *dir_path);

/**
 * Route an HTTP request to the appropriate handler.
 *
 * @param request Pointer to Request struct.
 * @param dir_path Directory path to serve files from.
 *
 * Determines the correct handler for the request path and returns the generated Response.
 */
Response *router(Request *request, const char *dir_path);

/**
 * Handle /echo/ requests.
 *
 * @param request Pointer to Request struct.
 *
 * Returns a Response echoing the request path after /echo/.
 */
Response *handle_echo(Request *request);

/**
 * Handle /user-agent requests.
 *
 * @param request Pointer to Request struct.
 *
 * Returns a Response containing the User-Agent header value.
 */
Response *handle_user_agent(Request *request);

/**
 * Handle invalid request paths.
 *
 * @param request Pointer to Request struct.
 *
 * Returns a 404 Not Found Response.
 */
Response *handle_invalid_path(Request *request);

/**
 * Handle the default path (/).
 *
 * @param request Pointer to Request struct.
 *
 * Returns a 200 OK Response for the root path.
 */
Response *handle_default_path(Request *request);

/**
 * Handle /files requests (GET and POST).
 *
 * @param request Pointer to Request struct.
 * @param dir_path Directory path to serve files from.
 *
 * Routes file requests to the appropriate file handler.
 */
Response *handle_files(Request *request, const char *dir_path);

/**
 * Handle GET requests for files.
 *
 * @param request Pointer to Request struct.
 * @param dir_path Directory path to serve files from.
 * @param file_name Name of the file to retrieve.
 *
 * Returns a Response containing the file contents or a 404 if not found.
 */
Response *handle_files_get(Request *request, const char *dir_path, const char *file_name);

/**
 * Handle POST requests for files.
 *
 * @param request Pointer to Request struct.
 * @param dir_path Directory path to write files to.
 * @param file_name Name of the file to create or overwrite.
 *
 * Writes the request body to the specified file and returns a 201 Created Response.
 */
Response *handle_files_post(Request *request, const char *dir_path, const char *file_name);

/**
 * Send a response with headers.
 *
 * @param request Pointer to Request struct.
 * @param content_length Length of the response content.
 * @param status_code HTTP status code.
 * @param reason_phrase Reason phrase string.
 * @param content_type Content-Type header value.
 * @param body Response body data.
 *
 * Constructs and returns a Response with the specified headers and body, optionally compressing the body if requested.
 */
Response *send_response_with_headers(Request *request, int content_length, int status_code, char *reason_phrase, char *content_type, char *body);

#endif
