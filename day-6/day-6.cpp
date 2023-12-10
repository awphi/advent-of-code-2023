#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>
using namespace std;

#define EPS 0.001

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

vector<long> collect_numbers(const string& s) {
  vector<long> result;
  regex re{"(\\d+)"};
  smatch sm;

  auto it = sregex_token_iterator(s.begin(), s.end(), re);
  auto end = sregex_token_iterator();
  while (it != end) {
    result.push_back(stol(*it++));
  }

  return result;
}

int sign(float x) {
  return x >= 0 ? 1 : -1;
}

long solutions_to_race(long time, long dis) {
  // y= -x^{2} + xt - d
  // solve for x to get roots, all integers between roots = winning moves
  double a = -1.0;
  double b = time;
  double c = (double)-dis;
  double temp = -0.5 * (b + sign(b) * sqrt(b * b - 4 * a * c));
  double x1 = c / temp;
  double x2 = temp / a;
  assert(x1 < x2);
  return (long)(floor(x2 - EPS) - ceil(x1 + EPS)) + 1;
}

// solves both part 1 and 2 - just manually modify your input text to remove
// spaces between the digits for part 2 :)
int main() {
  string content = read_file("day-6-input.txt");
  vector<string> sections = split(content, "\n");
  auto times = collect_numbers(sections.at(0));
  auto distances = collect_numbers(sections.at(1));

  long total = 1;
  for (int i = 0; i < times.size(); i++) {
    long time = times.at(i);
    long dis = distances.at(i);
    long s = solutions_to_race(time, dis);
    total *= s;
    cout << "r: " << i + 1 << ", t: " << time << ", d: " << dis << ", s: " << s
         << endl;
  }

  cout << "total: " << total << endl;

  return 0;
}