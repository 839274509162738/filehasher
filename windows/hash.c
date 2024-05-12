#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

void printUsage() { printf("Usage: hash.exe -f <filename>\n"); }

int main(int argc, char *argv[]) {
  if (argc != 3 || strcmp(argv[1], "-f") != 0) {
    printUsage();
    return 1;
  }

  char *filename = argv[2];
  FILE *file;
  if (fopen_s(&file, filename, "rb") != 0) {
    fprintf(stderr, "Error opening file\n");
    return 1;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  unsigned char *buffer = malloc(file_size);
  if (!buffer) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(file);
    return 1;
  }

  size_t bytes_read = fread(buffer, 1, file_size, file);
  fclose(file);

  if (bytes_read != (size_t)file_size) {
    fprintf(stderr, "Error reading file\n");
    free(buffer);
    return 1;
  }

  SHA512_CTX sha512;
  if (!SHA512_Init(&sha512)) {
    fprintf(stderr, "Error initializing SHA-512 context\n");
    free(buffer);
    return 1;
  }

  if (!SHA512_Update(&sha512, buffer, file_size)) {
    fprintf(stderr, "Error updating SHA-512 context\n");
    free(buffer);
    return 1;
  }

  unsigned char hash[SHA512_DIGEST_LENGTH];
  if (!SHA512_Final(hash, &sha512)) {
    fprintf(stderr, "Error finalizing SHA-512 hash\n");
    free(buffer);
    return 1;
  }

  printf("Hash of file %s:\n", filename);
  for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
  printf("\n");

  free(buffer);
  return 0;
}
