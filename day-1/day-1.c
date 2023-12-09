#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_NEWLINE 10
#define CHAR_0 48
#define CHAR_9 57
#define CHAR_SPACE 32

char* number_strings[10] = {"zero", "one", "two",   "three", "four",
                            "five", "six", "seven", "eight", "nine"};

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

void replace_string_numbers(char* content, char replacement) {
  size_t content_len = strlen(content);
  for (int i = 0; i < 10; i++) {
    char* num_string = number_strings[i];
    size_t len = strlen(num_string);
    char buf[len + 1];

    for (int j = 0; j < content_len - len + 1; j++) {
      // copy the next len chars of content into our buffer for comparison
      for (int k = 0; k < len; k++) {
        buf[k] = content[j + k];
      }
      buf[len] = 0;

      // printf("%d, %lu, %s, %s\n", i, len, buf, num_string);
      if (!strcmp(buf, num_string)) {
        // replace the second character in the content with the respective char
        // this abuses the fact that no two pairs of number strings have a
        // common prefix-suffix or suffix-prefix of length > 1 and that all
        // number strings are of length >= 3
        content[j + 1] = CHAR_0 + i;
      }
    }
  }
}

int main() {
  char* content = read_file("./day-1-input.txt");
  // part 2 only:
  replace_string_numbers(content, CHAR_SPACE);
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