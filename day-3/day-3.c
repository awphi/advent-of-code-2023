#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_NEWLINE 10
#define CHAR_0 48
#define CHAR_9 57
#define CHAR_DOT 46
#define N_DIRECTIONS 8

typedef struct {
  int x;
  int y;
} ivec2_t;

typedef struct {
  int start;
  int end;
} imatch_t;

ivec2_t directions[N_DIRECTIONS] = {{1, 0},  {-1, 0}, {0, 1},  {1, 1},
                                    {-1, 1}, {1, -1}, {0, -1}, {-1, -1}};

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

int is_number(char c) {
  return c >= CHAR_0 && c <= CHAR_9;
}

int is_symbol(char c) {
  return c != CHAR_DOT && !is_number(c);
}

int vec_to_index(ivec2_t* vec, int w) {
  return (vec->y * w) + vec->x;
}

ivec2_t index_to_vec(int idx, int w) {
  int y = (int)floor((double)idx / w);
  int x = idx - (w * y);
  ivec2_t result = {x, y};
  return result;
}

ivec2_t get_dimensions(char* content) {
  int i = 0;
  int w = 0;
  int h = 0;
  for (;;) {
    char c = content[i];
    if (c == CHAR_NEWLINE) {
      if (w == 0) {
        w = i;
      }
      h++;
    } else if (c == 0) {
      break;
    }
    i++;
  }
  ivec2_t result = {w, h + 1};
  return result;
}

char* flatten_str(char* in) {
  int count = 0;
  int i = 0;
  // count all the newlines so we know how big the resultant string should be
  for (;;) {
    char c = in[i];
    if (c == CHAR_NEWLINE) {
      count++;
    } else if (c == 0) {
      break;
    }
    i++;
  }

  int len = strlen(in) - count + 1;
  char* result = malloc(len);
  // maintain two 'pointers', where we're reading from the input string and
  // where we're writing to the output string
  int j = 0;
  i = 0;
  for (;;) {
    char c = in[i];
    if (c != CHAR_NEWLINE) {
      result[j] = c;
      j++;
      if (c == 0) {
        break;
      }
    }

    i++;
  }

  return result;
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

int main() {
  char* content = read_file("./day-3-input.txt");
  // remove newlines from the content so our indexing functions work as expected
  // not super efficient but it's okay
  char* flattened_content = flatten_str(content);
  ivec2_t dimensions = get_dimensions(content);

  int w = dimensions.x;
  int h = dimensions.y;
  size_t len = strlen(flattened_content);
  printf("%s\n\nwidth: %d\nheight: %d\nflat length: %ld\n\n", content, w, h,
         len);

  free(content);

  int i = 0;
  long sum = 0;

  for (;;) {
    char c = flattened_content[i];
    if (c == 0) {
      break;
    }

    if (is_symbol(c)) {
      ivec2_t v = index_to_vec(i, w);
      for (int j = 0; j < N_DIRECTIONS; j++) {
        // search in all 8 directions around the symbol, looking for a number in
        // a valid grid position
        ivec2_t dir = directions[j];
        ivec2_t pos;
        pos.x = dir.x + v.x;
        pos.y = dir.y + v.y;
        if (pos.x >= 0 && pos.x < w && pos.y >= 0 && pos.y < h) {
          int idx = vec_to_index(&pos, w);
          char c2 = flattened_content[idx];
          if (is_number(c2)) {
            // found a number, scan left and right for contiguous digits
            int start = idx;
            int end = idx;
            while (index_to_vec(end, w).y == pos.y &&
                   is_number(flattened_content[end])) {
              end++;
            }

            while (index_to_vec(start, w).y == pos.y &&
                   is_number(flattened_content[start])) {
              start--;
            }

            // extract substring, add to sum and blot it out so we don't double
            // count it
            char* match_str = substr(start + 1, end, flattened_content);
            sum += strtol(match_str, NULL, 10);
            for (int k = start + 1; k < end; k++) {
              flattened_content[k] = ' ';
            }
            // printf("%d -> (%d, %d): %s\n", i, dir.x, dir.y, match_str);
          }
        }
      }
    }

    i++;
  }

  printf("sum: %ld\n", sum);
  free(flattened_content);
  return 0;
}