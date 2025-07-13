#ifndef UTILS
#define UTILS

#include "constants.h"
#include "defines.h"
#include <zlib.h>

typedef struct {
  char *data;
  int file_size;

} FileContent;

/**
 * Fire an error.
 *
 * @param msg The message to display as error.
 * @param error_status The status error code to display.
 *
 */
void error(const char *msg, int error_status);

void shift_string_left(char **dest, char *src);

// returns 1 if file exist, 0 if file doesn't exist, -1 if dir doesn't exist
int file_exists_in_dir(const char *dir_path, const char *file_name);

// The created data pointer should be freed after used (not freed inside the function)
FileContent *read_file(const char *file_path);

void write_file(const char *dir_path, const char *file_name, const char *data);

void gzip_compress(char *input, size_t input_len, char *output, size_t *output_len);

#endif
