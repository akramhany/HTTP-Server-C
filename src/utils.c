#include "utils.h"
#include "constants.h"
#include "defines.h"

void error(const char *msg, int error_status) {
  fprintf(stderr, "%s", msg);
  exit(error_status);
}

void shift_string_left(char **dest, char *src) {
  int dest_length = strlen(*dest);
  int src_length = strlen(src);

  if (src_length > dest_length) {
    error("Can't shift a string with length greater than itself!\n", -1);
  }

  char buf[MAX_BUFFER_SIZE];

  strcpy(buf, (*dest) + src_length);
  free(*dest);
  (*dest) = malloc(strlen(buf) + 1);
  strcpy((*dest), buf);
  (*dest)[strlen(buf)] = '\0';
}
