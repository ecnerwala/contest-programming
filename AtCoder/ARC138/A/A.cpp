#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::pair<int, int>> bigs; bigs.reserve(N-K);
	for (int i = K; i < N; i++) {
		if (bigs.empty() || A[i] > bigs.back().first) {
			bigs.push_back({A[i], i});
		}
	}

	int ans = -1;
	for (int i = 0; i < K; i++) {
		// find the next thing after K bigger than it
		auto it = std::lower_bound(bigs.begin(), bigs.end(), std::pair<int, int>(A[i], N+1));
		if (it == bigs.end()) continue;
		int j = it->second;
		int cost = j - i;
		if (ans == -1 || cost < ans) ans = cost;
	}
	cout << ans << '\n';

	return 0;
}
