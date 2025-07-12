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

int file_exists_in_dir(const char *dir_path, const char *file_name) {
  DIR *dir = opendir(dir_path);
  if (!dir) {
    return -1;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, file_name) == 0) {
      closedir(dir);
      return 1;
    }
  }

  closedir(dir);
  return 0;
}

FileContent *read_file(const char *file_path) {
  FILE *fp = fopen(file_path, "r");
  if (!fp) {
    return NULL;
  }

  fseek(fp, 0, SEEK_END); // move pointer to end of file
  int file_size = ftell(fp) - 1;    // get pointer position as it will be file size in bytes, minus one byte for EOF (probably)

  rewind(fp);     // move pointer back to begining of file

  char *buffer = malloc(file_size + 1);
  fread(buffer, 1, file_size, fp);

  buffer[file_size] = '\0';

  fclose(fp);

  FileContent *ptr = malloc(sizeof(FileContent));

  ptr->data = buffer;
  ptr->file_size = file_size;

  return ptr;
}
