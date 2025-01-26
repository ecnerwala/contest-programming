#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		bool good = true;
		for (int i = 0; i < N; i++) {
			if (A[i] <= 2 * std::max(i, N - 1 - i)) {
				good = false;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
