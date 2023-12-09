#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEWLINE "\n"
#define MATCH_LEN 2
#define N_COLORS 3

typedef struct {
  char* color;
  int max;
} color;

color colors[N_COLORS] = {{"red", 12}, {"green", 13}, {"blue", 14}};

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

char* substr(int so, int eo, char* input) {
  int len = eo - so;
  if (len <= 0) {
    return NULL;
  }

  char* result = malloc(sizeof(char) * (len + 1));
  for (int i = 0; i < len; i++) {
    result[i] = input[so + i];
  }
  result[len] = 0;
  return result;
}

int max_count_for_color(color* cl, char* input) {
  regex_t re;
  regmatch_t match[2];

  int max = 0;
  char pattern[20] = "([0-9]+) ";

  strcat(pattern, cl->color);
  regcomp(&re, pattern, REG_EXTENDED);

  while (regexec(&re, input, 2, match, 0) == 0) {
    int value = strtol(substr(match[1].rm_so, match[1].rm_eo, input), NULL, 10);
    if (value > max) {
      max = value;
    }
    input = substr(match[0].rm_eo, strlen(input) + 1, input);
  }

  regfree(&re);
  return max <= cl->max;
}

int main() {
  regex_t id_re;
  regmatch_t id_match[2];
  regcomp(&id_re, "Game ([0-9]+):", REG_EXTENDED);

  char* content = read_file("./day-2-input.txt");
  char* line = strtok(content, NEWLINE);
  int sum = 0;

  while (line != NULL) {
    if (regexec(&id_re, line, 2, id_match, 0) == 0) {
      char* game_id_str = substr(id_match[1].rm_so, id_match[1].rm_eo, line);
      long game_id = strtol(game_id_str, NULL, 10);
      free(game_id_str);

      int is_possible = 1;
      for (int i = 0; i < N_COLORS; i++) {
        if (!max_count_for_color(&colors[i], line)) {
          is_possible = 0;
          printf("Game %ld - impossible due to %s\n", game_id, colors[i].color);
          break;
        }
      }

      if (is_possible) {
        sum += game_id;
        printf("Game %ld - possible\n", game_id);
      }
    }

    line = strtok(NULL, NEWLINE);
  }

  printf("%d\n", sum);
  regfree(&id_re);
  return 0;
}