#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<int> P(N); P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

	vector<int> sz(N, 1);
	for (int i = N-1; i; i--) sz[P[i]] += sz[i];

	vector<double> mutiny(N, 0);
	for (int i = N-1; i; i--) {
		if (sz[i] == 1) mutiny[i] = 1;
		mutiny[P[i]] = max(mutiny[P[i]], min(double(sz[i]) / double(sz[P[i]]-1), mutiny[i]));
	}
	double ans = 0;
	for (int i = 0; i < N; i++) {
		if (sz[i] > K) ans = max(ans, mutiny[i]);
	}
	cout << fixed << setprecision(15) << ans << '\n';

	return 0;
}
