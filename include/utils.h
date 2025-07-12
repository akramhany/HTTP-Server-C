#ifndef UTILS
#define UTILS

/**
 * Fire an error.
 *
 * @param msg The message to display as error.
 * @param error_status The status error code to display.
 *
 */
void error(const char *msg, int error_status);

void shift_string_left(char **dest, char *src);

#endif
