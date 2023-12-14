#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

typedef struct {
  int score;
  long bid;
  string hand;
} hand_t;

unordered_map<char, int> card_ranks = {
    {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3},  {'6', 4},  {'7', 5}, {'8', 6},
    {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12}};

string read_file(string path) {
  ifstream t(path);
  stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

vector<string> split(string s, const string& delim) {
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

int score_hand(const string& hand) {
  unordered_map<char, int> map;
  for (auto c : hand) {
    // missing keys are added and automatically initialised to 0
    map[c]++;
  }

  switch (map.size()) {
    case 1:
      // five of a kind (TTTTT)
      return 7;
    case 2:
      for (auto c : hand) {
        // four of a kind (TTTTA)
        if (map[c] == 4) {
          return 6;
        }
      }
      // full house (TTTAA)
      return 5;
    case 3:
      for (auto c : hand) {
        // three of a kind (TTTAB)
        if (map[c] == 3) {
          return 4;
        }
      }
      // 2 pair (TTAAB)
      return 3;
    case 4:
      // one pair
      return 2;
  }

  // high card - weakest hand
  return 1;
}

bool compare_hands(const hand_t& h1, const hand_t& h2) {
  if (h1.score == h2.score) {
    for (int i = 0; i < h1.hand.size(); i++) {
      int r1 = card_ranks[h1.hand[i]];
      int r2 = card_ranks[h2.hand[i]];
      if (r1 != r2) {
        return r1 < r2;
      }
    }
  }

  return h1.score < h2.score;
}

int main() {
  string content = read_file("day-7-input.txt");
  vector<string> lines = split(content, "\n");
  vector<hand_t> hands;
  for (auto line : lines) {
    vector<string> parts = split(line, " ");
    hand_t hand;
    hand.hand = parts.at(0);
    hand.score = score_hand(parts.at(0));
    hand.bid = stol(parts.at(1));
    hands.push_back(hand);
  }

  sort(hands.begin(), hands.end(), compare_hands);

  long sum = 0;
  for (int i = 0; i < hands.size(); i++) {
    hand_t hand = hands.at(i);
    cout << hand.hand << " - rank: " << i + 1 << "; score: " << hand.score
         << "; bid: " << hand.bid << endl;
    sum += (i + 1) * hand.bid;
  }
  cout << "sum: " << sum << endl;
  return 0;
}