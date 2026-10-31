#include <bits/stdc++.h>

struct pt_t {
	int x, y;
	int64_t norm;
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<pt_t> pts(N);
	for (auto& p : pts) {
		cin >> p.x >> p.y;
		p.norm = int64_t(p.x) * int64_t(p.x) + int64_t(p.y) * int64_t(p.y);
	}
	int num_origin = 0;
	std::vector<bool> used(N);
	for (int i = 0; i < N; i++) {
		if (pts[i].norm == 0) {
			num_origin++;
			used[i] = true;
		}
	}

	std::vector<int> pts_sorted(N); std::iota(pts_sorted.begin(), pts_sorted.end(), 0);
	std::sort(pts_sorted.begin(), pts_sorted.end(), [&](int a, int b) -> bool {
		auto v = __int128_t(pts[a].x) * pts[b].norm - __int128_t(pts[b].x) * pts[a].norm;
		if (v != 0) return v < 0;
		return __int128_t(pts[a].y) * pts[b].norm < __int128_t(pts[b].y) * pts[a].norm;
	});


	auto cross = [&](int a, int b) -> int64_t {
		return int64_t(pts[a].x) * int64_t(pts[b].y) - int64_t(pts[b].x) * int64_t(pts[a].y);
	};
	auto dir = [&](int a, int b, int c) -> int {
		auto v = cross(a, b) * __int128_t(pts[c].norm)
		+ cross(b, c) * __int128_t(pts[a].norm)
		+ cross(c, a) * __int128_t(pts[b].norm);
		return (v > 0) - (v < 0);
	};

	std::array<std::vector<int>, 5> lower_hulls;
	std::array<std::vector<int>, 5> upper_hulls;
	for (int k = 0; k < 5; k++) {
		std::vector<int> upper_hull; upper_hull.reserve(N);
		std::vector<int> lower_hull; lower_hull.reserve(N);
		for (int i : pts_sorted) {
			if (used[i]) continue;

			// explicitly drop identical points
			if (!upper_hull.empty() && pts[i].x == pts[upper_hull.back()].x && pts[i].y == pts[upper_hull.back()].y) {
				continue;
			}
			while (upper_hull.size() >= 2 && dir(upper_hull.end()[-2], upper_hull.end()[-1], i) >= 0) upper_hull.pop_back();
			while (lower_hull.size() >= 2 && dir(lower_hull.end()[-2], lower_hull.end()[-1], i) <= 0) lower_hull.pop_back();
			upper_hull.push_back(i);
			lower_hull.push_back(i);
		}
		if (upper_hull.empty()) {
			assert(lower_hull.empty());
		} else if (upper_hull.size() == 1) {
			assert(lower_hull.size() == 1);
			assert(lower_hull[0] == upper_hull[0]);
		} else {
			assert(lower_hull.size() >= 2);
			assert(lower_hull.front() == upper_hull.front());
			assert(lower_hull.back() == upper_hull.back());
		}

		for (int i : lower_hull) used[i] = true;
		for (int i : upper_hull) used[i] = true;

		lower_hulls[k] = std::move(lower_hull);
		upper_hulls[k] = std::move(upper_hull);
	}

	int Q; cin >> Q;
	while (Q--) {
		int x, y, K; cin >> x >> y >> K;
		if (x == 0 && y == 0) {
			cout << -1 << '\n';
			continue;
		}
		K -= num_origin;
		if (K <= 0) {
			cout << 0 << '\n';
			continue;
		}

		bool should_upper = (y > 0) || (y == 0 && x < 0);
		auto& hulls = (should_upper ? upper_hulls : lower_hulls);
		auto& o_hulls = (should_upper ? lower_hulls : upper_hulls);
		std::vector<int> cnds; cnds.reserve(K*K);
		auto is_better = [&](int a, int b) -> bool {
			return (x * int64_t(pts[a].x) + y * int64_t(pts[a].y)) * __int128_t(pts[b].norm) >
			(x * int64_t(pts[b].x) + y * int64_t(pts[b].y)) * __int128_t(pts[a].norm);
		};
		for (int k = 0; k < K; k++) {
			if (hulls[k].empty()) continue;
			if (hulls[k].size() == 1) {
				cnds.push_back(hulls[k].back());
				continue;
			}
			auto mi = -1, ma = int(hulls[k].size()) - 1;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (is_better(hulls[k][md], hulls[k][md+1])) {
					ma = md;
				} else {
					mi = md;
				}
			}
			// take ma as the best
			int tot_l = int(hulls[k].size()) + int(o_hulls[k].size()) - 2;
			for (int z = -(K-k); z <= (K-k); z++) {
				int idx = ma + z;
				idx %= tot_l;
				if (idx < 0) idx += tot_l;
				if (idx < int(hulls[k].size())) {
					cnds.push_back(hulls[k][idx]);
				} else {
					cnds.push_back(o_hulls[k][tot_l - idx]);
				}
			}
		}
		std::sort(cnds.begin(), cnds.end());
		cnds.resize(std::unique(cnds.begin(), cnds.end()) - cnds.begin());
		std::sort(cnds.begin(), cnds.end(), is_better);
		if (int(cnds.size()) < K) {
			cout << -1 << '\n';
			continue;
		}
		int p = cnds[K-1];
		int64_t dot = x * int64_t(pts[p].x) + y * int64_t(pts[p].y);
		if (x * int64_t(pts[p].x) + y * int64_t(pts[p].y) <= 0) {
			cout << -1 << '\n';
			continue;
		}
		using ld = long double;
		long double ans = std::sqrt(ld(int64_t(x) * int64_t(x) + int64_t(y) * int64_t(y))) / ld(dot) * ld(pts[p].norm) / 2;
		cout << fixed << setprecision(20) << ans << '\n';
	}

	return 0;
}
