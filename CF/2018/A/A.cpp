#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t K; cin >> N >> K;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		cout << [&]() -> int64_t {
			int64_t mx = *std::max_element(A.begin(), A.end());
			int64_t tot = std::accumulate(A.begin(), A.end(), int64_t(0));
			for (int d = N; d >= 1; d--) {
				if (mx <= (tot + K) / d) return d;
			}
			assert(false);
		}() << '\n';
	}

	return 0;
}
