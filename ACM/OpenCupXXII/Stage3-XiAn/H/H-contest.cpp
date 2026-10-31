#include <bits/stdc++.h>

const int MAXN = int(5e5) + 100;
std::pair<int, int> P[3][MAXN];
const int MAXV = MAXN * 4;
int outEdges[MAXV][3];
int inDeg[MAXV];
int q[MAXV];

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 3; z++) {
				int v; cin >> v;
				P[z][i] = {v, i};
			}
		}
		for (int z = 0; z < 3; z++) {
			std::sort(P[z], P[z]+N);
		}

		// if we can toposort the people obeying 
		auto is_good = [&](int64_t k) -> bool {
			int V = N * 4;
			memset(outEdges, -1, sizeof(outEdges[0]) * V);
			memset(inDeg, 0, sizeof(inDeg[0]) * V);
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
					while (j < N && P[z][j].first < P[z][i].first - 2 * k) {
						j++;
					}
					assert(j <= i);
					if (j > 0) {
						addEdge(P[z][i].second, (z+1)*N+(j-1), z);
					}
				}
			}

			int len = 0;
			for (int i = 0; i < V; i++) {
				if (!inDeg[i]) q[len++] = i;
			}
			for (int z = 0; z < len; z++) {
				int cur = q[z];
				for (int nxt : outEdges[cur]) {
					if (nxt == -1) continue;
					if (!--inDeg[nxt]) q[len++] = nxt;
				}
			}
			return len == V;
		};

		int64_t mi = -1;
		int64_t ma = int64_t(2e9) + 100;
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			if (is_good(md)) ma = md;
			else mi = md;
		}
		cout << ma << '\n';
	}

	return 0;
}
