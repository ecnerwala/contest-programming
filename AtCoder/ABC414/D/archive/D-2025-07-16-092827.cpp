#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int64_t> X(N);
	for (auto& x : X) cin >> x;
	std::sort(X.begin(), X.end());
	std::vector<int64_t> G(N-1);
	for (int i = 0; i+1 < N; i++) {
		G[i] = X[i+1] - X[i];
	}
	std::nth_element(G.begin(), G.end() - (N-M), G.end());
	cout << std::accumulate(G.end() - (N-M), G.end(), int64_t(0)) << '\n';

	return 0;
}
