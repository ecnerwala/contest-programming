#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<bool> has(N+1);
		for (auto a : A) { has[a] = true; }
		int ans = 0;
		while (ans < K && has[ans]) ans++;
		cout << ans << '\n';
	}

	return 0;
}
