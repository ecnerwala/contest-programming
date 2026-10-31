#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;

		auto solve = [&]() -> bool {
			int64_t tot = std::accumulate(A.begin(), A.end(), int64_t(0));
			if (tot % N) return false;
			int tgt = int(tot / N);

			std::map<int, int> cnt;

			for (auto a : A) {
				if (a == tgt) {
					assert(N >= 2);
					// we're fine
					continue;
				} else if (a > tgt) {
					int d = a - tgt;
					int lo = d & -d;
					int hi = d + lo;
					if (hi & (hi-1)) return false;
					cnt[lo]++;
					cnt[hi]--;
				} else if (a < tgt) {
					int d = tgt - a;
					int lo = d & -d;
					int hi = d + lo;
					if (hi & (hi-1)) return false;
					cnt[lo]--;
					cnt[hi]++;
				} else assert(false);
			}

			for (auto [k, v] : cnt) {
				if (v) return false;
			}
			return true;
		};
		cout << (solve() ? "Yes" : "No") << '\n';
	}

	return 0;
}
