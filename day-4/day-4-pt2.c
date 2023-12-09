#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_NEWLINE 10
#define TOKENIZER " "
#define MAX_WINNING_NUMBER 99
#define INVENTORY_SIZE 200

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

int score_card(char* card) {
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
  int score = 0;
  while (our_tok != NULL) {
    int v = (int)strtol(our_tok, NULL, 10);
    if (winning_numbers[v]) {
      score++;
    }
    our_tok = __strtok_r(NULL, " ", &space_ptr);
  }

  return score;
}

int main() {
  char* content = read_file("./day-4-pt2-input.txt");
  char* line = strtok(content, "\n");
  int inventory[INVENTORY_SIZE] = {0};
  int scores[INVENTORY_SIZE] = {0};
  int i = 0;

  // add initial scratchcards to inventory and cache their scores
  while (line != NULL) {
    long v = score_card(line);
    printf("Card %d: %ld\n", i + 1, v);
    // verrrry basic error handling
    if (v == -1) {
      printf("Failed to score a card!\n");
    } else {
      scores[i] = v;
    }
    inventory[i] = 1;
    i++;
    line = strtok(NULL, "\n");
  }

  // now process inventory until it's empty
  long sum = i;
  bool done = false;

  while (!done) {
    done = true;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
      if (inventory[i] > 0) {
        int amount = inventory[i];
        int score = scores[i];

        if (score > 0) {
          for (int j = i + 1; j < i + 1 + score; j++) {
            inventory[j] += amount;
          }
          sum += score * amount;
          done = false;
        }

        inventory[i] = 0;
      }
    }
  }

  printf("score: %ld\n", sum);
  free(content);
  return 0;
}