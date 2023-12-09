#include <algorithm>
#include <fstream>
#include <iostream>
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
  for (auto line_it = lines.begin() + 1; line_it != lines.end(); line_it++) {
    vector<string> values = split(*line_it, " ");
    range_t r = {stol(values.at(0)), stol(values.at(1)), stol(values.at(2))};
    result.push_back(r);
  }
  return result;
}

bool in_range(long v, range_t r) {
  return v >= r.dst && v < r.dst + r.len;
}

long location_to_seed(long seed, vector<vector<range_t>>* maps) {
  long current = seed;

  for (auto section : *maps) {
    for (auto r : section) {
      if (in_range(current, r)) {
        current = r.src + (current - r.dst);
        break;
      }
    }
  }

  return current;
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

  reverse(map_sections.begin(), map_sections.end());

  vector<string> seed_section = split(sections.at(0), " ");
  vector<range_t> seed_ranges;
  for (auto it = seed_section.begin() + 1; it != seed_section.end(); it += 2) {
    range_t seed_range = {0, stol(*it), stol(*(it + 1))};
    seed_ranges.push_back(seed_range);
    cout << "Seed range: " << seed_range.src << "->"
         << seed_range.src + seed_range.len << endl;
  }

  long i = 0;
  bool done = false;
  while (!done) {
    long v = location_to_seed(i, &map_sections);
    // cout << i << "->" << v << endl;
    for (auto range : seed_ranges) {
      if (v >= range.src && v < range.src + range.len) {
        // cout << "---" << endl;
        cout << "min final value: " << i << endl;
        return 0;
      }
    }
    i++;
  }

  return 0;
}