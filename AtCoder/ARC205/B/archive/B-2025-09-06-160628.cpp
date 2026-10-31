#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> deg(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		deg[u] ^= 1;
		deg[v] ^= 1;
	}
	for (int i = 0; i < N; i++) {
		deg[i] ^= (N-1) & 1;
	}
	int ans = 0;
	for (int i = 0; i < N; i++) {
		ans += deg[i];
	}
	cout << int64_t(N) * int64_t(N-1) / 2 - ans / 2 << '\n';

	return 0;
}
