#include "utils.h"
#include "defines.h"

void error(const char *msg, int error_status) {
  fprintf(stderr, "%s", msg);
  exit(error_status);
}
