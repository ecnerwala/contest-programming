#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, C; cin >> N >> C;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::pair<int64_t, int64_t>> B(N-1);
		for (int i = 1; i < N; i++) {
			B[i-1] = {int64_t(C) * (i+1) - A[i], A[i]};
		}
		std::sort(B.begin(), B.end());
		int64_t v = A[0];
		bool good = true;
		for (auto [thresh, nv] : B) {
			if (v < thresh) {
				good = false;
				break;
			}
			v += nv;
		}
		cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
