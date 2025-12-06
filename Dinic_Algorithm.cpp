#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

struct Edge {
    int to, rev;
    long long flow, cap;
};

class Graph {
public:
    int n; // Number of vertices
    vector<vector<Edge>> adj; // Adjacency list

    Graph(int n) : n(n), adj(n) {}

    // Add edges to the graph
    void addEdge(int from, int to, long long cap) {
        Edge a{to, (int)adj[to].size(), 0, cap}; // forward edge
        Edge b{from, (int)adj[from].size(), 0, 0}; // backward edge
        adj[from].push_back(a);
        adj[to].push_back(b);
    }

    // BFS to construct level graph
    bool bfs(int source, int sink, vector<int>& level) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(source);
        level[source] = 0;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto& edge : adj[u]) {
                if (level[edge.to] < 0 && edge.flow < edge.cap) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                    if (edge.to == sink) return true;
                }
            }
        }
        return false;
    }

    // DFS to send flow
    long long dfs(int u, long long flow, int sink, vector<int>& level, vector<int>& ptr) {
        if (u == sink) return flow;
        for (; ptr[u] < adj[u].size(); ptr[u]++) {
            Edge& edge = adj[u][ptr[u]];
            if (level[edge.to] == level[u] + 1 && edge.flow < edge.cap) {
                long long newFlow = min(flow, edge.cap - edge.flow);
                long long pushed = dfs(edge.to, newFlow, sink, level, ptr);
                if (pushed > 0) {
                    edge.flow += pushed;
                    adj[edge.to][edge.rev].flow -= pushed; // Update backward edge
                    return pushed;
                }
            }
        }
        return 0; // No valid flow
    }

    // The main function that returns the maximum flow
    long long maxFlow(int source, int sink) {
        long long totalFlow = 0;
        vector<int> level(n);
        while (bfs(source, sink, level)) {
            vector<int> ptr(n, 0);
            while (long long flow = dfs(source, numeric_limits<long long>::max(), sink, level, ptr)) {
                totalFlow += flow;
            }
        }
        return totalFlow;
    }
};

int main() {
    Graph g(6);
    g.addEdge(0, 1, 16);
    g.addEdge(0, 2, 13);
    g.addEdge(1, 2, 10);
    g.addEdge(1, 3, 12);
    g.addEdge(2, 1, 4);
    g.addEdge(2, 4, 14);
    g.addEdge(3, 2, 9);
    g.addEdge(3, 5, 20);
    g.addEdge(4, 3, 7);
    g.addEdge(4, 5, 4);

    cout << "The maximum possible flow is " << g.maxFlow(0, 5) << endl;

    return 0;
}