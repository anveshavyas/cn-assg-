#include <iostream>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

const int INF = numeric_limits<int>::max();

void dijkstra(int n, vector<vector<pair<int, int>>> &graph, int src) {
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[src] = 0;
    pq.push(make_pair(0, src));

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        for (auto &edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    cout << "\nNode\tDistance from Source " << src << "\tPath\n";
    for (int i = 0; i < n; ++i) {
        cout << i << "\t\t";
        if (dist[i] == INF) {
            cout << "∞\t\t\tNo path\n";
            continue;
        }
        cout << dist[i] << "\t\t\t";

        vector<int> path;
        for (int v = i; v != -1; v = parent[v])
            path.push_back(v);
        for (int j = path.size() - 1; j >= 0; --j)
            cout << path[j] << (j ? " -> " : "");
        cout << endl;
    }
}
int main() {
    int n, m;
    cout << "Enter number of nodes: ";
    cin >> n;

    cout << "Enter number of edges: ";
    cin >> m;

    vector<vector<pair<int, int>>> graph(n);
    cout << "Enter edges in the format: from to weight\n";
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
    }
    int source;
    cout << "Enter source node (0 to " << n - 1 << "): ";
    cin >> source;
    dijkstra(n, graph, source);
    return 0;
}
