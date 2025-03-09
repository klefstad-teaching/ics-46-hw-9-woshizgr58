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
    return edit_distance_within(word1, word2, 1);
}

// Generate all neighbors of 'word' that are in 'dict'.
vector<string> get_neighbors(const string &word, const set<string> &dict) {
    set<string> nbrs;

    for (size_t i = 0; i < word.size(); ++i) {
        string candidate = word;
        for (char c = 'a'; c <= 'z'; ++c) {
            if (candidate[i] == c) continue; // Skip same letter
            candidate[i] = c;
            if (dict.find(candidate) != dict.end()) {
                nbrs.insert(candidate);
            }
        }
    }

    for (size_t i = 0; i <= word.size(); ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            string candidate = word;
            candidate.insert(candidate.begin() + i, c);
            if (dict.find(candidate) != dict.end()) {
                nbrs.insert(candidate);
            }
        }
    }

    if (word.size() > 1) { 
        for (size_t i = 0; i < word.size(); ++i) {
            string candidate = word;
            candidate.erase(candidate.begin() + i);
            if (dict.find(candidate) != dict.end()) {
                nbrs.insert(candidate);
            }
        }
    }
    
    return vector<string>(nbrs.begin(), nbrs.end());
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string> &word_list) {
    if (begin_word == end_word)
        return vector<string>{};


    if (word_list.find(end_word) == word_list.end())
        return vector<string>{};


    set<string> dict = word_list;
    
    queue<vector<string>> q;
    q.push(vector<string>{begin_word});
    dict.erase(begin_word);
    
    while (!q.empty()) {
        vector<string> ladder = q.front();
        q.pop();
        string last_word = ladder.back();
        
        vector<string> neighbors = get_neighbors(last_word, dict);
        for (const string &nbr : neighbors) {
            vector<string> new_ladder = ladder;
            new_ladder.push_back(nbr);
            if (nbr == end_word)
                return new_ladder;
            q.push(new_ladder);
            dict.erase(nbr);
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
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found: ";
    for (const string &word : ladder)
        cout << word << " ";
    cout << endl;
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
void verify_word_ladder() {
    set<string> word_list;

    load_words(word_list, "src/words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);

    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);

    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);

    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);

    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);

    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}