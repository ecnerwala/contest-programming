#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		std::vector<int> in_deg(N);
		for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; in_deg[P[i]]++; }

		std::vector<int> q; q.reserve(N);
		for (int i = 0; i < N; i++) if (!in_deg[i]) q.push_back(i);
		std::vector<int> bound(N, 0);
		std::vector<int> sz(N, 1);
		for (int z = 0; z < N; z++) {
			int cur = q[z];
			bound[cur] = std::max(bound[cur], 1 + (sz[cur] - 1 + 1) / 2);

			int p = P[cur];
			if (p != -1) {
				bound[p] = std::max(bound[p], bound[cur] + 1);
				sz[p] += sz[cur];
				if (!--in_deg[p]) q.push_back(p);
			}
		}
		cout << N - bound[0] << '\n';
	}

	return 0;
}
