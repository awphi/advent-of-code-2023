#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_NEWLINE 10
#define CHAR_0 48
#define CHAR_9 57

char* read_file(char* path) {
  FILE* f = fopen(path, "r");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  rewind(f);
  char* string = malloc(fsize + 1);
  fread(string, fsize, 1, f);
  string[fsize] = 0;
  fclose(f);
  return string;
}

int main() {
  char* content = read_file("./day-1-input.txt");
  printf("%s\n", content);
  char buf[2];
  memset(buf, 0, sizeof(buf));

  long sum = 0;
  int i = 0;

  for (;;) {
    char c = content[i];
    if (c == 10 || c == 0) {
      sum += strtol(buf, NULL, 10);
      printf("%c%c, sum: %ld\n", buf[0], buf[1], sum);
      memset(buf, 0, sizeof(buf));

      if (c == 0) {
        break;
      }
    } else if (c >= CHAR_0 && c <= CHAR_9) {
      if (buf[0] == 0) {
        buf[0] = c;
      }

      buf[1] = c;
    }
    i++;
  }

  free(content);

  return 0;
}