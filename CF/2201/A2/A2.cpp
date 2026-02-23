#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// consider the first biggest guy
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<std::pair<int, int>> vals; vals.reserve(N);
		for (int i = 0; i < N; i++) {
			vals.push_back({A[i], -i});
		}
		std::sort(vals.begin(), vals.end(), std::greater<>());

		std::vector<int> prv(N);
		std::vector<int> nxt(N);
		for (int i = 0; i < N; i++) prv[i] = i-1, nxt[i] = i+1;

		int64_t ans = int64_t(N) * int64_t(N+1) * int64_t(N+2) / 6;
		for (auto [a, i] : vals) {
			i = -i;
			assert(A[i] == a);
			int p = prv[i], n = nxt[i];
			if (p != -1 && A[p] == a-1) {
				ans -= int64_t(p+1) * int64_t(N - i);
				// delete i
				nxt[p] = n;
				if (n != N) prv[n] = p;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
