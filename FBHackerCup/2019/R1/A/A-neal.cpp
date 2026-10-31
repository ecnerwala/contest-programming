#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const string IMPOSSIBLE = "Impossible";
const int INF = 1e9 + 5;

int N, M;
vector<vector<int>> edge;
vector<vector<int>> dist;

void solve_case() {
    cin >> N >> M;
    edge.assign(N, vector<int>(N, INF));

    for (int i = 0; i < N; i++)
        edge[i][i] = 0;

    bool answer = true;

    for (int i = 0; i < M; i++) {
        int x, y, weight;
        cin >> x >> y >> weight;
        x--; y--;

        if (edge[x][y] < INF && edge[x][y] != weight)
            answer = false;

        edge[x][y] = edge[y][x] = weight;
    }

    if (!answer) {
        cout << IMPOSSIBLE << '\n';
        return;
    }

    dist = edge;

    for (int k = 0; k < N; k++)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (edge[i][j] < INF)
                answer = answer && edge[i][j] <= dist[i][j];

    if (!answer) {
        cout << IMPOSSIBLE << '\n';
        return;
    }

    int E = 0;

    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
            if (dist[i][j] < INF)
                E++;

    cout << E << '\n';

    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
            if (dist[i][j] < INF)
                cout << i + 1 << ' ' << j + 1 << ' ' << dist[i][j] << '\n';
}

int main() {
    int T;
    cin >> T;

    for (int tc = 1; tc <= T; tc++) {
        cout << "Case #" << tc << ": ";
        solve_case();
        cout << flush;
    }
}
