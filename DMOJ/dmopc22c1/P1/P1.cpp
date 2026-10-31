#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		cout << ([&]() -> bool {
			for (int z = 0; z < 2; z++) {
				for (auto& v : A) v = -v;
				bool is_good = true;
				for (int i = 0; i+1 < N; i++) {
					if (A[i] == 0 || A[i+1] == 0) continue;
					if (A[i] == A[i+1]) {
						is_good = false;
						break;
					}
					if ((A[i] > A[i+1]) != (i & 1)) {
						is_good = false;
						break;
					}
				}
				if (is_good) return true;
			}
			return false;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
