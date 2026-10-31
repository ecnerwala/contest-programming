#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	const int B = 1000;
	struct stock_data { int64_t market, face; };
	vector<stock_data> stocks(N);
	for (auto& v : stocks) cin >> v.market >> v.face;

	struct bucket_data { vector<int> inds; vector<pair<int64_t, int64_t>> hull; int64_t lazy_add; };
	auto compute_hull = [&](bucket_data& b) {
		b.hull.clear();
		for (int i : b.inds) {
			pair<int64_t, int64_t> cnd(stocks[i].face - stocks[i].market, stocks[i].face);
			if (!b.hull.empty() && b.hull.back().second - b.hull.back().first <= cnd.second - cnd.first) {
				// do nothing
			} else {
				b.hull.push_back(cnd);
			}
		}
	};
	const int64_t INF = 1e18;
	auto query_hull = [&](const bucket_data& b) -> int64_t {
		auto it = lower_bound(b.hull.begin(), b.hull.end(), pair<int64_t, int64_t>{b.lazy_add, -INF});
		int64_t ans = INF;
		if (it != b.hull.end()) {
			ans = min(ans, it->second);
		}
		if (it != b.hull.begin()) {
			--it;
			ans = min(ans, it->second - it->first + b.lazy_add);
		}
		return ans;
	};
	vector<bucket_data> buckets((N+B-1) / B);
	auto query_stock = [&](int i) -> int64_t {
		return max(stocks[i].market + buckets[i/B].lazy_add, stocks[i].face);
	};
	for (int b = 0; b*B < N; b++) {
		auto& dat = buckets[b];
		dat.inds.resize(min(B, N-b*B));
		iota(dat.inds.begin(), dat.inds.end(), b*B);
		sort(dat.inds.begin(), dat.inds.end(), [&](int i, int j) {
			return std::tie(stocks[i].face, stocks[i].market) < std::tie(stocks[j].face, stocks[j].market);
		});
		dat.hull.reserve(dat.inds.size());
		compute_hull(dat);
		dat.lazy_add = 0;
	}

	for (int q = 0; q < Q; q++) {
		int t; cin >> t;
		if (t == 1) {
			int l, r; cin >> l >> r; l--;
			assert(l < r);
			int64_t x; cin >> x;
			if (l / B == r / B) {
				while (l < r) {
					stocks[l].market += x;
					l++;
				}
				compute_hull(buckets[r/B]);
				continue;
			}

			if (l % B != 0) {
				while (l % B != 0) {
					stocks[l].market += x;
					l++;
				}
				compute_hull(buckets[l / B - 1]);
			}
			if (r % B != 0) {
				// decrease r until it hits
				while (r % B != 0) {
					--r;
					stocks[r].market += x;
				}
				compute_hull(buckets[r / B]);
			}
			while (l < r) {
				buckets[l/B].lazy_add += x;
				l += B;
			}
		} else if (t == 2) {
			int l, r; cin >> l >> r; l--;
			assert(l < r);
			int64_t ans = INF;
			if (l / B == r / B) {
				while (l < r) {
					ans = min(ans, query_stock(l));
					l++;
				}
			} else {
				while (l % B != 0) {
					ans = min(ans, query_stock(l));
					l++;
				}
				while (r % B != 0) {
					--r;
					ans = min(ans, query_stock(r));
				}
				while (l < r) {
					ans = min(ans, query_hull(buckets[l/B]));
					l += B;
				}
			}
			cout << ans << '\n';
		} else assert(false);
	}

	return 0;
}
