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

// Generate neighbors by substituting each letter in the word.
// This function assumes that all words in the dictionary have the same length.
vector<string> get_neighbors(const string &word, const set<string> &dict) {
    vector<string> neighbors;
    string candidate = word;
    for (size_t i = 0; i < word.size(); ++i) {
        char original = candidate[i];
        for (char c = 'a'; c <= 'z'; ++c) {
            if (c == original) continue; // Skip the same letter
            candidate[i] = c;
            if (dict.find(candidate) != dict.end())
                neighbors.push_back(candidate);
        }
        candidate[i] = original; // Restore original letter
    }
    return neighbors;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string> &word_list) {
    // If the end word isn't in the dictionary, return an empty ladder.
    if (word_list.find(end_word) == word_list.end()) return vector<string>{};

    // Create a mutable copy of the dictionary so we can remove words as we use them.
    set<string> dict = word_list;

    queue<vector<string>> q;
    q.push(vector<string>{begin_word});
    // Remove the begin word to prevent revisiting it.
    dict.erase(begin_word);

    while (!q.empty()) {
        vector<string> ladder = q.front();
        q.pop();
        string last_word = ladder.back();
        if (last_word == end_word)  // If reached the goal, return the ladder.
            return ladder;

        // Generate neighbors using our optimized approach.
        vector<string> neighbors = get_neighbors(last_word, dict);
        for (const string &nbr : neighbors) {
            vector<string> new_ladder = ladder;
            new_ladder.push_back(nbr);
            q.push(new_ladder);
            // Remove the neighbor from the dictionary to avoid revisiting.
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

// verify_word_ladder uses a predefined test case and checks the validity of the resulting ladder.
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