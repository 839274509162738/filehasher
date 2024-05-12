#include <openssl/evp.h>
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
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Error: failed opening file");
    return 1;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  unsigned char *buffer = malloc(file_size);
  if (!buffer) {
    fprintf(stderr, "Error: failed memory allocation\n");
    fclose(file);
    return 1;
  }

  size_t bytes_read = fread(buffer, 1, file_size, file);
  fclose(file);

  if (bytes_read != (size_t)file_size) {
    fprintf(stderr, "Error: failed reading file\n");
    free(buffer);
    return 1;
  }

  EVP_MD_CTX *mdctx;
  const EVP_MD *md;
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hash_len;

  md = EVP_sha512();
  mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    fprintf(stderr, "Error: failed creating message digest context\n");
    free(buffer);
    return 1;
  }

  if (1 != EVP_DigestInit_ex(mdctx, md, NULL)) {
    fprintf(stderr, "Error: failed initializing message digest\n");
    free(buffer);
    EVP_MD_CTX_free(mdctx);
    return 1;
  }

  if (1 != EVP_DigestUpdate(mdctx, buffer, file_size)) {
    fprintf(stderr, "Error: failed updating message digest\n");
    free(buffer);
    EVP_MD_CTX_free(mdctx);
    return 1;
  }

  if (1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) {
    fprintf(stderr, "Error: failed finalizing message digest\n");
    free(buffer);
    EVP_MD_CTX_free(mdctx);
    return 1;
  }

  printf("Hash of file %s: ", filename);
  for (unsigned int i = 0; i < hash_len; i++) {
    printf("%02x", hash[i]);
  }
  printf("\n");

  free(buffer);
  EVP_MD_CTX_free(mdctx);
  return 0;
}
