#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
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

long map_seed_value_back(long seed, vector<vector<range_t>> maps) {
  long current = seed;

  for (auto section_it = maps.begin(); section_it != maps.end(); ++section_it) {
    auto section = *section_it;

    for (auto it = section.begin(); it != section.end(); ++it) {
      range_t r = *it;
      // map the value if it's in range and break
      // if no ranges match then current will stay unchanged, effectively
      // mapping it to itself
      if (in_range(current, r)) {
        current = r.dst + (current - r.src);
        break;
      }
    }
  }

  return current;
}

int main() {
  string content = read_file("day-5-input.txt");
  vector<string> sections = split(content, "\n\n");
  vector<string> seed_section = split(sections.at(0), " ");

  // gather up all the seeds as integers
  vector<long> seeds;
  for (auto it = seed_section.begin() + 1; it != seed_section.end(); ++it) {
    seeds.push_back(stol(*it));
  }

  // we assume the input file has the map sections in order i.e A->B, B->C, C->D
  // etc.
  vector<vector<range_t>> map_sections;
  for (auto it = sections.begin() + 1; it != sections.end(); ++it) {
    map_sections.push_back(make_map_section(*it));
  }

  // now finally run each seed through the map sections in order and find its
  // final mapped value
  long min = -1;
  for (auto it = seeds.begin(); it != seeds.end(); ++it) {
    long seed = *it;
    long v = map_seed_value_back(*it, map_sections);
    cout << seed << "->" << v << endl;
    if (v < min || min == -1) {
      min = v;
    }
  }

  cout << "---" << endl;
  cout << "min final value: " << min << endl;

  return 0;
}