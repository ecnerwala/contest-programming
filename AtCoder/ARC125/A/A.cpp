#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<int> S(N); for (auto& s : S) cin >> s;
	vector<int> T(M); for (auto& t : T) cin >> t;

	if (*max_element(T.begin(), T.end()) == *min_element(T.begin(), T.end()) && T[0] == S[0]) {
		cout << M << '\n';
		exit(0);
	}

	int best_dist = N+1;
	for (int i = 0; i < N; i++) {
		if (S[i] != S[0]) best_dist = min(best_dist, min(i, N-i));
	}
	if (best_dist > N) {
		cout << -1 << '\n';
		exit(0);
	}
	assert(best_dist > 0);
	int ans = M + best_dist-1;
	int cur = S[0];
	for (int c : T) {
		if (c != cur) ans++;
		cur = c;
	}
	cout << ans << '\n';

	return 0;
}
