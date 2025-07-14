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

/**
 * Shift a string left by the length of another string.
 *
 * @param dest Pointer to a string to be shifted.
 * @param src String whose length determines the shift amount.
 *
 * Shifts the contents of *dest left by the length of src, effectively removing the prefix src from *dest. Allocates new memory for the shifted string.
 */
void shift_string_left(char **dest, char *src);

/**
 * Check if a file exists in a directory.
 *
 * @param dir_path Directory path to search.
 * @param file_name File name to look for.
 *
 * Checks if a file with the given name exists in the specified directory. Returns 1 if found, 0 if not, and -1 if the directory cannot be opened.
 */
int file_exists_in_dir(const char *dir_path, const char *file_name);

/**
 * Read the contents of a file.
 *
 * @param file_path Path to the file to read.
 *
 * Reads the contents of a file into a dynamically allocated buffer and returns a FileContent struct containing the data and file size. Returns NULL if the file cannot be opened.
 */
FileContent *read_file(const char *file_path);

/**
 * Write data to a file in a directory.
 *
 * @param dir_path Directory path to write the file in.
 * @param file_name Name of the file to write.
 * @param data Data to write to the file.
 *
 * Writes the given data to a file in the specified directory. Overwrites the file if it exists. Exits the program with an error if the file cannot be created.
 */
void write_file(const char *dir_path, const char *file_name, const char *data);

/**
 * Compress data using gzip.
 *
 * @param input Input data to compress.
 * @param input_len Length of input data.
 * @param output Buffer to store compressed data.
 * @param output_len Pointer to variable holding output buffer size; set to actual compressed size on return.
 *
 * Compresses the input data using gzip and stores the result in output. Updates output_len to the compressed size. Exits the program with an error if compression fails.
 */
void gzip_compress(char *input, size_t input_len, char *output, size_t *output_len);

#endif
