/* A simple version of ls */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void listfile(char *name) {
  struct stat sb; // The stat buffer
  char *modtime;

  // Array of file types, indexed by the top four bits of `st_mode`
  char *filetype[] = {"?", "p", "c", "?", "d", "?", "b",
                      "?", "-", "?", "l", "?", "s"};

  if (stat(name, &sb) < 0) {
    perror(name);
    exit(2);
  }

  // File type (using same indicator characters as `ls`)
  printf("%s", filetype[(sb.st_mode >> 12) & 017]);

  // Permissions
  printf("%c%c%c%c%c%c%c%c%c", (sb.st_mode & S_IRUSR) ? 'r' : '-',
         (sb.st_mode & S_IWUSR) ? 'w' : '-', (sb.st_mode & S_IXUSR) ? 'x' : '-',
         (sb.st_mode & S_IRGRP) ? 'r' : '-', (sb.st_mode & S_IWGRP) ? 'w' : '-',
         (sb.st_mode & S_IXGRP) ? 'x' : '-', (sb.st_mode & S_IROTH) ? 'r' : '-',
         (sb.st_mode & S_IWOTH) ? 'w' : '-',
         (sb.st_mode & S_IXOTH) ? 'x' : '-');

  printf("%8lld", sb.st_size);

  modtime = ctime(&sb.st_mtime);
  // ctime() string has \n at end; we must remove
  modtime[strlen(modtime) - 1] = '\0';
  printf("  %s  ", modtime);
  printf("%s\n", name);
}

int main(int argc, char *argv[]) {
  DIR *d;
  struct dirent *info;

  if (argc != 2) {
    fprintf(stderr, "Usage: listdir dirname\n");
    exit(1);
  }

  chdir(argv[1]);
  d = opendir(".");

  while ((info = readdir(d)) != NULL) {
    listfile(info->d_name);
  }
}
