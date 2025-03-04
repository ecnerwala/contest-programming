#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, M; cin >> N >> K >> M;
	std::vector<std::string> S(N);
	for (auto& s : S) cin >> s;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::map<std::string, int> groups;
	for (int k = 0; k < K; k++) {
		int x; cin >> x;
		std::vector<int> inds(x);
		for (auto& i : inds) { cin >> i; i--; }
		int min_cost = A[inds[0]];
		for (auto i : inds) min_cost = std::min(min_cost, A[i]);
		for (auto i : inds) groups[S[i]] = min_cost;
	}

	int64_t ans = 0;
	for (int z = 0; z < M; z++) {
		std::string s; cin >> s;
		ans += groups[s];
	}
	cout << ans << '\n';

	return 0;
}
