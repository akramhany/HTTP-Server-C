#ifndef UTILS
#define UTILS

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

#endif
