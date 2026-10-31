#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;
	const int L = 500;
	int B = (N + L - 1) / L;
	std::vector<std::pair<int64_t, int>> sorted_bucket(N);
	// lo and hi first, then add
	const int64_t INF = 1e18;
	struct op_t {
		int64_t lo = -INF, hi = INF;
		int64_t add = 0;
	};
	std::vector<op_t> ops(B);

	for (int i = 0; i < N; i++) {
		sorted_bucket[i] = {A[i], i};
	}
	auto my_cmp = [&](auto x, auto y) { return x.first < y.first; };
	for (int b = 0; b*L < N; b ++) {
		std::sort(sorted_bucket.begin()+b*L, sorted_bucket.begin() + std::min((b+1)*L, N), my_cmp);
	}


	std::vector<std::pair<int64_t, int>> buf1; buf1.reserve(L);
	std::vector<std::pair<int64_t, int>> buf2; buf2.reserve(L);

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1 || op == 2 || op == 3) {
			int l, r; int64_t v; cin >> l >> r >> v; l--, r--;
			auto do_bucket_fast = [&](int b) {
				if (op == 3) {
					ops[b].add += v;
				} else {
					int64_t cv = v - ops[b].add;
					if (op == 2) {
						ops[b].lo = std::max(ops[b].lo, cv);
						ops[b].hi = std::max(ops[b].hi, cv);
					} else if (op == 1) {
						ops[b].lo = std::min(ops[b].lo, cv);
						ops[b].hi = std::min(ops[b].hi, cv);
					} else assert(false);
				}
			};
			auto do_bucket_slow = [&](int b) {
				buf1.clear();
				buf2.clear();
				for (int i = b*L; i < std::min((b+1)*L, N); i++) {
					int64_t val = sorted_bucket[i].first;
					val = std::max(val, ops[b].lo);
					val = std::min(val, ops[b].hi);
					val += ops[b].add;

					int idx = sorted_bucket[i].second;
					if (l <= idx && idx <= r) {
						if (op == 1) {
							val = std::min(val, v);
						} else if (op == 2) {
							val = std::max(val, v);
						} else if (op == 3) {
							val += v;
						}
						buf1.push_back({val, idx});
					} else {
						buf2.push_back({val, idx});
					}
				}
				assert(std::is_sorted(buf1.begin(), buf1.end(), my_cmp));
				assert(std::is_sorted(buf2.begin(), buf2.end(), my_cmp));
				std::merge(buf1.begin(), buf1.end(), buf2.begin(), buf2.end(), sorted_bucket.begin()+b*L, my_cmp);
				ops[b].lo = -INF;
				ops[b].hi = +INF;
				ops[b].add = 0;
			};
			if (l / L == r / L) {
				do_bucket_slow(l / L);
			} else {
				do_bucket_slow(l / L);
				do_bucket_slow(r / L);
				for (int b = (l/L)+1; b <= (r/L)-1; b++) {
					do_bucket_fast(b);
				}
			}
		} else if (op == 4) {
			int l, r; cin >> l >> r; l--; r--;
			int64_t lo, hi; cin >> lo >> hi;
			int ans = 0;
			auto do_bucket_fast = [&](int b) {
				int64_t clo = lo - ops[b].add, chi = hi - ops[b].add;
				if (clo <= ops[b].lo) {
					clo = -INF;
				}
				if (chi >= ops[b].hi) {
					chi = +INF;
				}
				auto it = std::lower_bound(sorted_bucket.begin() + (b*L), sorted_bucket.begin() + std::min((b+1)*L, N),
					std::pair<int64_t, int>{clo, -1}, my_cmp);
				auto jt = std::upper_bound(sorted_bucket.begin() + (b*L), sorted_bucket.begin() + std::min((b+1)*L, N),
					std::pair<int64_t, int>{chi, -1}, my_cmp);
				ans += int(jt - it);
			};
			auto do_bucket_slow = [&](int b) {
				int64_t clo = lo - ops[b].add, chi = hi - ops[b].add;
				if (clo <= ops[b].lo) {
					clo = -INF;
				}
				if (chi >= ops[b].hi) {
					chi = +INF;
				}
				for (int i = b*L; i < std::min((b+1)*L, N); i++) {
					int64_t val = sorted_bucket[i].first;
					int idx = sorted_bucket[i].second;
					if (l <= idx && idx <= r && clo <= val && val <= chi) {
						ans++;
					}
				}
			};
			if (l / L == r / L) {
				do_bucket_slow(l / L);
			} else {
				do_bucket_slow(l / L);
				do_bucket_slow(r / L);
				for (int b = (l/L)+1; b <= (r/L)-1; b++) {
					do_bucket_fast(b);
				}
			}
			cout << ans << '\n';
		} else assert(false);
	}

	return 0;
}
