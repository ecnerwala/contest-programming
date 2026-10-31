#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		cout << (K >= 2 || std::is_sorted(A.begin(), A.end()) ? "YES" : "NO") << '\n';
	}

	return 0;
}
