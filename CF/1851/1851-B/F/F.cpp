#include <bits/stdc++.h>

struct rng_t {
	int lo, hi;
	friend bool operator == (rng_t a, rng_t b) {
		return a.lo == b.lo && a.hi == b.hi;
	}
	int sz() const { return hi - lo; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<std::pair<int, int>> A(N);
		for (auto& a : A) cin >> a.first;
		for (int i = 0; i < N; i++) A[i].second = i;

		std::vector<std::array<rng_t, 2>> ranges;
		ranges.reserve(N);
		std::array<std::vector<std::array<rng_t, 2>>, 2> n_ranges;
		n_ranges[0].reserve(N);
		n_ranges[1].reserve(N);
		std::vector<std::pair<int, int>> nxt_A(N);

		assert(N >= 2);
		ranges.push_back({rng_t{0, N}, {0, N}});
		for (int b = K-1; b >= 0; b--) {
			auto do_sort = [&](rng_t r) -> std::array<rng_t, 2> {
				int idx = r.lo;
				for (int i = r.lo; i < r.hi; i++) {
					if (((A[i].first >> b) & 1) == 0) {
						nxt_A[idx++] = A[i];
					}
				}
				int m = idx;
				for (int i = r.lo; i < r.hi; i++) {
					if (((A[i].first >> b) & 1) == 1) {
						nxt_A[idx++] = A[i];
					}
				}
				assert(idx == r.hi);
				return {rng_t{r.lo, m}, rng_t{m, r.hi}};
			};
			for (auto [rng_0, rng_1] : ranges) {
				auto [l0, r0] = do_sort(rng_0);
				if (rng_1 == rng_0) {
					if (l0.sz() >= 2) {
						n_ranges[1].push_back({l0, l0});
					}
					if (r0.sz() >= 2) {
						n_ranges[1].push_back({r0, r0});
					}
					if (l0.sz() >= 1 && r0.sz() >= 1) {
						n_ranges[0].push_back({l0, r0});
					}
				} else {
					auto [l1, r1] = do_sort(rng_1);
					if (l0.sz() && l1.sz()) {
						n_ranges[1].push_back({l0, l1});
					}
					if (r0.sz() && r1.sz()) {
						n_ranges[1].push_back({r0, r1});
					}
					if (l0.sz() && r1.sz()) {
						n_ranges[0].push_back({l0, r1});
					}
					if (r0.sz() && l1.sz()) {
						n_ranges[0].push_back({r0, l1});
					}
				}
			}
			std::swap(A, nxt_A);

			for (int z = 1; ; z--) {
				assert(z >= 0);
				if (n_ranges[z].empty()) continue;
				std::swap(ranges, n_ranges[z]);
				break;
			}
			for (auto& v : n_ranges) v.clear();
		}
		assert(!ranges.empty());
		auto i = A[ranges[0][0].lo], j = A[ranges[0][1].hi - 1];
		cout << i.second+1 << ' ' << j.second+1 << ' ' << (1 << K) - 1 - i.first << '\n';
	}

	return 0;
}
