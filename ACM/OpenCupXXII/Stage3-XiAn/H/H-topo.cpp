#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<std::vector<pair<int, int>>, 3> P;
		for (int z = 0; z < 3; z++) {
			P[z].resize(N);
		}
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 3; z++) {
				int v; cin >> v;
				P[z][i] = {v, i};
			}
		}
		for (int z = 0; z < 3; z++) {
			std::sort(P[z].begin(), P[z].end());
		}

		// if we can toposort the people obeying
		auto is_good = [&](int k) -> bool {
			int V = N * 4;
			std::vector<std::array<int, 3>> outEdges(V, {-1,-1,-1});
			std::vector<int> inDeg(V);
			auto addEdge = [&](int a, int b, int idx) -> void {
				inDeg[b]++;
				outEdges[a][idx] = b;
			};
			for (int z = 0; z < 3; z++) {
				for (int i = 0; i < N; i++) {
					addEdge((z+1)*N+i, P[z][i].second, 0);
					if (i) {
						addEdge((z+1)*N+i, (z+1)*N+(i-1), 1);
					}
				}
				for (int i = 0, j = 0; i < N; i++) {
					while (j < N && P[z][i].first - P[z][j].first > 2 * k) {
						j++;
					}
					assert(j <= i);
					if (j > 0) {
						addEdge(P[z][i].second, (z+1)*N+(j-1), z);
					}
				}
			}

			std::vector<int> q; q.reserve(V);
			for (int i = 0; i < V; i++) {
				if (!inDeg[i]) q.push_back(i);
			}
			for (int z = 0; z < int(q.size()); z++) {
				int cur = q[z];
				for (int nxt : outEdges[cur]) {
					if (nxt == -1) continue;
					if (!--inDeg[nxt]) q.push_back(nxt);
				}
			}
			return int(q.size()) == V;
		};

		int mi = -1;
		int ma = int(1e9) + 100;
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			if (is_good(md)) ma = md;
			else mi = md;
		}
		cout << ma << '\n';
	}

	return 0;
}
