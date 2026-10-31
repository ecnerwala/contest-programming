#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int M; cin >> M;
	std::vector<std::vector<int>> adj(N+1);
	for (int e = 0; e < M; e++) {
		int l, r; cin >> l >> r; l--;
		assert((l & 1) == (r & 1));
		adj[l].push_back(r);
		adj[r].push_back(l);
	}

	std::vector<int> V(N+1, N+3);

	std::vector<std::pair<int, int>> q(4*(N+1));
	int lo = 2*(N+1), hi = 2*(N+1);

	V[0] = 0;
	q[hi++] = {V[0], 0};

	while (lo < hi) {
		auto [d, cur] = q[lo++];
		if (d != V[cur]) continue;
		for (int nxt : adj[cur]) {
			if (d < V[nxt]) {
				q[--lo] = {V[nxt] = d, nxt};
			}
		}
		if (cur-1 >= 0) {
			if (d+1 < V[cur-1]) {
				q[hi++] = {V[cur-1] = d+1, cur-1};
			}
		}
		if (cur+1 <= N) {
			if (d+1 < V[cur+1]) {
				q[hi++] = {V[cur+1] = d+1, cur+1};
			}
		}
	}

	for (int i = 0; i < N; i++) {
		assert(std::abs(V[i] - V[i+1]) == 1);
		cout << int(V[i] > V[i+1]);
	}
	cout << '\n';

	return 0;
}
