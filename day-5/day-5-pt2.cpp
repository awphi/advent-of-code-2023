#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

typedef struct {
  long dst;
  long src;
  long len;
} range_t;

string read_file(string path) {
  ifstream t(path);
  stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

vector<string> split(string s, string delim) {
  vector<string> sections;
  size_t pos;
  while ((pos = s.find(delim)) != string::npos) {
    string token = s.substr(0, pos);
    sections.push_back(token);
    s.erase(0, pos + delim.length());
  }
  sections.push_back(s);
  return sections;
}

vector<range_t> make_map_section(string s) {
  vector<range_t> result;
  vector<string> lines = split(s, "\n");
  for (auto line_it = lines.begin() + 1; line_it != lines.end(); ++line_it) {
    vector<string> values = split(*line_it, " ");
    range_t r = {stol(values.at(0)), stol(values.at(1)), stol(values.at(2))};
    result.push_back(r);
  }
  return result;
}

bool in_range(long v, range_t r) {
  return v >= r.src && v < r.src + r.len;
}

long map_seed_value(long seed, vector<range_t> map) {
  for (auto it = map.begin(); it != map.end(); it++) {
    range_t r = *it;
    if (in_range(seed, r)) {
      return r.dst + (seed - r.src);
    }
  }

  return seed;
}

vector<range_t> reduce_map_sections(vector<range_t> prev,
                                    vector<range_t> next) {
    // TODO merge all the ranges of next into prev
  return prev;
}

int main() {
  string content = read_file("day-5-input.txt");
  vector<string> sections = split(content, "\n\n");

  // we assume the input file has the map sections in order i.e A->B, B->C, C->D
  // etc.
  vector<vector<range_t>> map_sections;
  for (auto it = sections.begin() + 1; it != sections.end(); it++) {
    map_sections.push_back(make_map_section(*it));
  }

  // reduce all the mapping stages into one
  vector<range_t> map = reduce(map_sections.begin() + 1, map_sections.end(),
                               map_sections.at(0), reduce_map_sections);

  long min = -1;
  vector<string> seed_section = split(sections.at(0), " ");
  for (auto it = seed_section.begin() + 1; it != seed_section.end(); it += 2) {
    vector<long> seeds;
    range_t seed_range = {0, stol(*it), stol(*(it + 1))};

    // we try the first and last seed in the range
    seeds.push_back(seed_range.src);
    seeds.push_back(seed_range.src + seed_range.len - 1);

    // and the start (inclusive) and end (exclusive) of all our mapping ranges
    for (auto range_it = map.begin(); range_it != map.end(); range_it++) {
      range_t r = *range_it;
      if (in_range(r.src, seed_range)) {
        seeds.push_back(r.src);
      }

      if (in_range(r.src + r.len, seed_range)) {
        seeds.push_back(r.src + r.len);
      }
    }

    for (auto seed_it = seeds.begin(); seed_it != seeds.end(); seed_it++) {
      long seed = *seed_it;
      long v = map_seed_value(seed, map);
      cout << seed << "->" << v << endl;
      if (v < min || min == -1) {
        min = v;
      }
    }
  }

  cout << "---" << endl;
  cout << "min final value: " << min << endl;

  return 0;
}