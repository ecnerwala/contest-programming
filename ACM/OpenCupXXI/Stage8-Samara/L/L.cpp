#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<int> A(N); for (int& a : A) { cin >> a; a--; }
	vector<int> next_remove(N);
	vector<pair<int, int>> best_remove(K+1, {0, N});
	for (int i = N-1; i >= 0; i--) {
		int v = A[i];
		next_remove[i] = best_remove[v].second;
		best_remove[v].first++;
		best_remove[v].second = i;
		if (v < K-1) {
			best_remove[v] = min(best_remove[v], best_remove[v+1]);
		}
	}
	cout << best_remove[0].first << '\n';
	vector<int> to_remove; to_remove.reserve(best_remove[0].first);
	for (int cur = best_remove[0].second; cur != N; cur = next_remove[cur]) {
		to_remove.push_back(cur);
	}

	for (int i = 0; i < int(to_remove.size()); i++) {
		cout << to_remove[i]+1 << " ";
	}
	cout << '\n';

	return 0;
}
