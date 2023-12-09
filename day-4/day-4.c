#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_NEWLINE 10
#define TOKENIZER " "
#define MAX_WINNING_NUMBER 99

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

long score_card(char* card) {
  char* pipe_ptr;
  char* space_ptr;
  char* winners_str = __strtok_r(card, "|", &pipe_ptr);
  char* ours_str = __strtok_r(NULL, "|", &pipe_ptr);
  char* winner_tok = __strtok_r(winners_str, " ", &space_ptr);
  winner_tok = __strtok_r(NULL, " ", &space_ptr);
  winner_tok = __strtok_r(NULL, " ", &space_ptr);
  bool winning_numbers[MAX_WINNING_NUMBER + 1] = {false};

  while (winner_tok != NULL) {
    int v = (int)strtol(winner_tok, NULL, 10);
    if (v > MAX_WINNING_NUMBER || v < 0) {
      return -1;
    }

    winning_numbers[v] = true;
    winner_tok = __strtok_r(NULL, " ", &space_ptr);
  }

  char* our_tok = __strtok_r(ours_str, " ", &space_ptr);
  long score = 0;
  while (our_tok != NULL) {
    int v = (int)strtol(our_tok, NULL, 10);
    if (winning_numbers[v]) {
      if (score == 0) {
        score = 1;
      } else {
        score *= 2;
      }
    }
    our_tok = __strtok_r(NULL, " ", &space_ptr);
  }

  return score;
}

int main() {
  char* content = read_file("./day-4-input.txt");
  char* line = strtok(content, "\n");
  long sum = 0;

  while (line != NULL) {
    char* space_ptr;
    printf("%s; len: %ld\n", line, strlen(line));
    long v = score_card(line);
    // verrrry basic error handling
    if (v == -1) {
      printf("Failed to score a card!\n");
    } else {
      sum += v;
    }
    line = strtok(NULL, "\n");
  }

  printf("score: %ld\n", sum);
  free(content);
  return 0;
}