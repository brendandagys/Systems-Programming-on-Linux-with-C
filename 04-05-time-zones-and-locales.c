/* Simple demo of `strftime` */
#include <stdio.h>
#include <time.h>
#include <locale.h>

int main()
{
  time_t t;
  struct tm *bdtime;
  char buf[100];

  setlocale(LC_ALL, "");
  t = time(NULL);
  bdtime = localtime(&t);
  strftime(buf, 1000, "It is %H:%M on %A %d %B %Y", bdtime);
  puts(buf);
}
