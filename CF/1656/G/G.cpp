#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; a--; }

		std::vector<int> P(N);
		auto solve = [&]() -> bool {
			std::vector<int> cnt(N);
			for (auto a : A) cnt[a]++;
			int mid = 0;
			if (N & 1) {
				for (auto a : A) mid ^= a;
			}
			for (int i = 0; i < N; i++) {
				if ((cnt[i] - (mid == i)) & 1) {
				}
			}
		};
		if (solve()) {
			cout << "YES" << '\n';
			for (int i = 0; i < N; i++) {
				cout << P[i]+1 << " \n"[i+1==N];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
