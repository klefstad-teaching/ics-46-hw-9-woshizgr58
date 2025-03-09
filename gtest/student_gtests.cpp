#include <gtest/gtest.h>

#include "/home/guorenz/ICS46/ics-46-hw-9-woshizgr58/src/dijkstras.h"
#include "/home/guorenz/ICS46/ics-46-hw-9-woshizgr58/src/ladder.h"


TEST(DijkstrasTest, ShortestPath) {
    // Create a small graph with 4 vertices.
    Graph G;
    G.numVertices = 4;
    G.resize(G.numVertices);
    // Add edges: 0->1 (1), 0->2 (4), 1->2 (2), 1->3 (5), 2->3 (1)
    G[0].push_back(Edge(0, 1, 1));
    G[0].push_back(Edge(0, 2, 4));
    G[1].push_back(Edge(1, 2, 2));
    G[1].push_back(Edge(1, 3, 5));
    G[2].push_back(Edge(2, 3, 1));

    int source = 0;
    int destination = 3;
    vector<int> previous;
    
    // Run Dijkstra's algorithm from source.
    vector<int> distances = dijkstra_shortest_path(G, source, previous);
    // Extract the shortest path from source to destination.
    vector<int> path = extract_shortest_path(distances, previous, destination);
    
    // Expected path: 0 -> 1 -> 2 -> 3 with a total cost of 4.
    vector<int> expected_path = {0, 1, 2, 3};
    EXPECT_EQ(path, expected_path);
    EXPECT_EQ(distances[destination], 4);
}

TEST(LadderTest, GenerateWordLadder) {
    // Using a simple dictionary for a 3-letter word ladder.
    set<string> word_list = {"hot", "dot", "dog", "lot", "log", "cog"};
    string begin_word = "hit";
    string end_word = "cog";
    
    // Generate the ladder.
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    
    // One expected valid ladder is: hit -> hot -> dot -> dog -> cog.
    // (Depending on implementation details, another valid ladder might be acceptable.)
    vector<string> expected_ladder = {"hit", "hot", "dot", "dog", "cog"};
    EXPECT_EQ(ladder, expected_ladder);
}
