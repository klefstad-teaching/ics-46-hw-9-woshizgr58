#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <cmath>
#include "ladder.h"

using namespace std;

void error(string word1, string word2, string msg) {
    cerr << "Error: " << word1 << " " << word2 << " " << msg << endl;
}

bool edit_distance_within(const string &s1, const string &s2, int threshold) {
    int m = s1.size();
    int n = s2.size();
    
    if (abs(m - n) > threshold)
        return false;
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]);
        }
    }
    
    return dp[m][n] <= threshold;
}

bool is_adjacent(const string &word1, const string &word2) {
    if (word1 == word2) return true;
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string> &word_list) {
    queue<vector<string>> q;
    q.push(vector<string>{begin_word});
    set<string> visited;
    visited.insert(begin_word);
    
    if (word_list.find(end_word) == word_list.end()) return vector<string> {}; // if end_word not in word list
    
    while (!q.empty()) { 
        vector<string> ladder = q.front();
        q.pop();
        string last_word = ladder.back();
        for (const string &word : word_list) {
            if (is_adjacent(last_word, word)) {
                if (visited.find(word) == visited.end()) {
                    visited.insert(word);
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);
                    if (word == end_word)
                        return new_ladder;
                    q.push(new_ladder);
                }
            }
        }
    }
    
    error(begin_word, end_word, "no ladder found");
    return vector<string>{};
}

void load_words(set<string> &word_list, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: can't open file " << filename << endl;
        return;
    }
    string word;
    while (file >> word) {
        word_list.insert(word);
    }
    file.close();
}

void print_word_ladder(const vector<string> &ladder) {
    if (ladder.empty()) cout << "No word ladder found.";
    cout << "Word ladder found: ";
    for (const string &word : ladder) {
        cout << word << " ";
    }
    cout << endl;
}

// This version of verify_word_ladder takes no parameters as specified in the header.
// It generates a test ladder using predefined inputs and prints whether it is valid.
void verify_word_ladder() {
    string begin_word = "hit";
    string end_word = "cog";
    set<string> word_list = {"hot", "dot", "dog", "lot", "log", "cog"};
    
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    if (ladder.empty()) {
        cout << "Word ladder verification failed: Ladder is empty." << endl;
        return;
    }
    
    for (size_t i = 1; i < ladder.size(); i++) {
        if (!is_adjacent(ladder[i - 1], ladder[i])) {
            cout << "Invalid ladder: Words '" << ladder[i - 1]
                 << "' and '" << ladder[i] << "' are not adjacent." << endl;
            return;
        }
    }
    cout << "Valid word ladder!" << endl;
}