#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(N); for (auto& b : B) cin >> b;
	vector<int> P(N); for (auto& p : P) { cin >> p; p--; }

	for (int i = 0; i < N; i++) {
		if (P[i] == i) continue;
		// this guy is already stuck :(
		if (B[P[i]] >= A[i]) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	vector<pair<int, int>> ans; ans.reserve(N);
	for (int i = 0; i < N; i++) {
		if (P[i] == i) continue;
		int best_j = i;
		for (int cur = P[i]; cur != i; cur = P[cur]) {
			if (B[P[cur]] < B[P[best_j]]) best_j = cur;
		}
		for (int cur = P[best_j]; cur != best_j; cur = P[cur]) {
			ans.push_back({cur, P[cur]});
		}
		for (int cur = best_j; cur != P[cur]; cur = std::exchange(P[cur], cur));
	}
	reverse(ans.begin(), ans.end());
	cout << ans.size() << '\n';
	for (auto [x, y] : ans) {
		cout << x+1 << ' ' << y+1 << '\n';
	}

	return 0;
}
