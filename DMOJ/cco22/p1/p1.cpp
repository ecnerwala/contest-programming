#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, Q; cin >> N >> K >> Q;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }

	auto is_scc = [&](int x, int y) {
		std::vector<std::vector<int>> outEdges(K);
		std::vector<int> inDeg(K);
		for (int i = x; i+1 <= y; i++) {
			int a = A[i], b = A[i+1];
			if (i & 1) std::swap(a, b);

			outEdges[a].push_back(b);
			inDeg[b]++;
		}
		std::vector<int> q; q.reserve(K);
		for (int i = 0; i < K; i++) if (!inDeg[i]) q.push_back(i);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt : outEdges[cur]) {
				if (!--inDeg[nxt]) q.push_back(nxt);
			}
		}
		return int(q.size()) == K;
	};

	std::vector<int> best(N);
	for (int i = 0; i < N; i++) {
		best[i] = i ? std::max(i, best[i-1]) : 0;
		while (best[i]+1 < N && is_scc(i, best[i]+1)) best[i]++;
	}

	for (int q = 0; q < Q; q++) {
		int x, y; cin >> x >> y; x--, y--;
		cout << (y <= best[x] ? "YES" : "NO") << '\n';
	}

	return 0;
}
