#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char *src, *dst;
  int fin, fout;
  size_t size;

  fin = open("foo", O_RDONLY);
  if (fin < 0) {
    perror("foo");
    exit(1);
  }

  // Get size of file. Use to specify the size of the two mappings
  size = lseek(fin, 0, SEEK_END);

  src = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fin, 0);
  if (src == MAP_FAILED) {
    perror("mmap");
    exit(2);
  }

  fout = open("bar", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fout < 0) {
    perror("bar");
    exit(1);
  }

  if (ftruncate(fout, size) == -1) {
    perror("ftruncate");
    exit(3);
  }

  dst = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fout, 0);
  if (dst == MAP_FAILED) {
    perror("mmap");
    exit(4);
  }

  memcpy(dst, src, size); // Copy bytes between mappings

  // Push the destination buffer back to the file
  if (msync(dst, size, MS_SYNC) == -1) {
    perror("msync");
    exit(5);
  }

  exit(0);
}
