#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int, int>> constraints; constraints.reserve(N);
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			constraints.push_back({a, i});
		}
		cout << [&]() -> int {
			int lo = 0;
			int hi = 0;
			int cur_t = N;
			std::sort(constraints.begin(), constraints.end(), std::greater<>());
			for (auto [a, i] : constraints) {
				assert(a <= cur_t);
				hi += (cur_t - a);
				cur_t = a;
				// we need x <= i <= x + a - 1
				// <=> i - (a - 1) <= x <= i
				hi = std::min(hi, i);
				lo = std::max(lo, i - (a - 1));
				if (lo > hi) return 0;
			}
			assert(1 <= cur_t);
			hi += (cur_t - 1);
			cur_t = 1;

			assert(lo <= hi);
			return hi - lo + 1;
		}() << '\n';
	}

	return 0;
}
