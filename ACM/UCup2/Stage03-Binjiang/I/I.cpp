#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t M; cin >> N >> M;

	struct line_t {
		int64_t v;
		int64_t d;
	};
	std::vector<std::array<line_t, 2>> lines(N);
	for (auto& [a, b] : lines) {
		cin >> a.v >> a.d >> b.v >> b.d;
	}

	std::vector<int64_t> cutoffs; cutoffs.reserve(N*N + 2);
	cutoffs.push_back(0);
	cutoffs.push_back(M);
	auto check_inter = [&](line_t a, line_t b) {
		// 0 <= k <= M
		if (a.v == b.v) {
			// do nothing, they either intersect at 0 or always
			return;
		}
		if (a.v < b.v) std::swap(a, b);
		assert(a.v > b.v);
		if (a.d >= b.d) {
			// never catch up
			return;
		}
		assert(a.d < b.d);
		int64_t t = (a.v - b.v) / (b.d - a.d);
		if (t < M) cutoffs.push_back(t);
	};
	for (int i = 0; i < N; i++) {
		check_inter(lines[i][0], lines[i][1]);
		for (int j = i+1; j < N; j++) {
			check_inter(lines[i][0], lines[j][0]);
			check_inter(lines[i][1], lines[j][1]);
		}
	}
	std::sort(cutoffs.begin(), cutoffs.end());
	cutoffs.resize(std::unique(cutoffs.begin(), cutoffs.end()) - cutoffs.begin());
	assert(cutoffs.front() == 0);
	assert(cutoffs.back() == M);

	using i128 = __int128_t;
	auto eval_at = [&](line_t a, int64_t t) -> i128 {
		return i128(a.v) + i128(a.d) * i128(t);
	};
	std::vector<std::pair<i128, int>> incr; incr.reserve(N);
	std::vector<std::pair<i128, int>> decr; decr.reserve(N);

	std::vector<line_t> needs; needs.reserve(N);
	std::vector<std::pair<int64_t, line_t>> hull; hull.reserve(N);

	uint64_t ans = 0;

	auto solve_range = [&](int64_t t0, int64_t t1) -> void {
		assert(0 <= t0 && t0 <= t1 && t1 <= M);
		incr.clear(); decr.clear();
		for (int i = 0; i < N; i++) {
			i128 a0 = eval_at(lines[i][0], t0);
			i128 b0 = eval_at(lines[i][1], t0);
			if (a0 <= b0) {
				incr.push_back({a0, i});
			} else {
				decr.push_back({b0, i});
			}
		}
		std::sort(incr.begin(), incr.end());
		std::sort(decr.begin(), decr.end(), std::greater<>());
		incr.insert(incr.end(), decr.begin(), decr.end());

		needs.clear();
		line_t cur{0, 0};
		for (auto [_, i] : incr) {
			cur.v += lines[i][0].v;
			cur.d += lines[i][0].d;
			needs.push_back(cur);
			cur.v -= lines[i][1].v;
			cur.d -= lines[i][1].d;
		}
		// range max query of needs
		std::sort(needs.begin(), needs.end(), [&](line_t a, line_t b) -> bool {
			return a.d != b.d ? a.d < b.d : a.v < b.v;
		});
		// sorted by increasing v
		hull.clear();
		for (line_t l : needs) {
			while (!hull.empty()) {
				if (eval_at(l, hull.back().first) >= eval_at(hull.back().second, hull.back().first)) {
					hull.pop_back();
				} else {
					break;
				}
			}
			if (hull.empty()) hull.push_back({t0, l});
			else {
				assert(l.d >= hull.back().second.d);
				assert(l.v < hull.back().second.v);
				assert(l.d > hull.back().second.d);
				// we want (hull.back().second.v - l.v) < t * (l.d - hull.back().second.d);
				int64_t cut = (hull.back().second.v - l.v) / (l.d - hull.back().second.d) + 1;
				assert(cut > hull.back().first);
				if (cut <= t1) hull.push_back({cut, l});
			}
		}
		assert(!hull.empty());
		for (int z = 0; z < int(hull.size()); z++) {
			int64_t tlo = hull[z].first;
			int64_t thi = z+1 == int(hull.size()) ? t1 + 1 : hull[z+1].first;
			assert(thi > tlo);
			thi--;
			line_t l = hull[z].second;
			//cerr << tlo << ".." << thi << ' ' << l.v << ' ' << l.d << '\n';
			// evaluate l at tlo..thi
			ans += uint64_t((i128(thi - tlo + 1) * i128(l.v)) & uint64_t(-1));
			ans += uint64_t((i128((i128(thi - tlo + 1) * i128(thi + tlo) / 2) & (uint64_t(-1))) * i128(l.d)) & uint64_t(-1));
		}
	};

	for (int i = 0; i < int(cutoffs.size()); i++) {
		solve_range(cutoffs[i], cutoffs[i]);
		if (i+1 < int(cutoffs.size()) && cutoffs[i+1]-1 >= cutoffs[i]+1) {
			solve_range(cutoffs[i]+1, cutoffs[i+1]-1);
		}
	}

	cout << ans << '\n';

	return 0;
}
