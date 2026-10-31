#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	struct pt_t {
		int x, y; int idx;
	};
	int N; cin >> N;
	std::vector<pt_t> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].x >> pts[i].y;
		pts[i].idx = i;
	}

	std::vector<pt_t> orig_pts = pts;

	std::string ans(N, '0');

	auto sq = [](int a) -> int64_t {
		return int64_t(a) * int64_t(a);
	};
	auto dist = [sq](const pt_t& a, const pt_t& b) -> int64_t {
		return sq(a.x - b.x) + sq(a.y - b.y);
	};

	const int64_t INF = 8e18;
	std::vector<int64_t> min_dist(N, INF);
	for (int bits = 0; pts.size() >= 2; bits++) {
		//cerr << "bits == " << bits << '\n';
		assert(bits <= 31);
		std::vector<std::pair<std::array<int, 2>, pt_t>> cur_pts; cur_pts.reserve(pts.size());
		for (auto p : pts) {
			cur_pts.push_back({{p.x >> bits, p.y >> bits}, p});
		}
		std::sort(cur_pts.begin(), cur_pts.end(), [](const auto& a, const auto& b) {
			return a.first < b.first;
		});

		std::vector<std::pair<int64_t, std::array<int, 2>>> cnds; cnds.reserve(cur_pts.size());
		for (int i = 0, j0 = 0, j1 = 0; i < int(cur_pts.size()); i++) {
			auto [kx, ky] = cur_pts[i].first;
			auto check_point = [&](int z) -> void {
				int64_t d = dist(cur_pts[i].second, cur_pts[z].second);
				//cerr << "candidate " << d << ' ' << cur_pts[i].second.idx << ' ' << cur_pts[z].second.idx << '\n';
				if (bits < 30 && d >= (1LL << (2*bits))) {
					//cerr << "bad" << '\n';
					// unfortunately, this might be too big
					return;
				}
				cnds.push_back({d, {cur_pts[i].second.idx, cur_pts[z].second.idx}});
			};

			while (j0 < int(cur_pts.size()) && cur_pts[j0].first < std::array<int, 2>{kx-1, ky-1}) {
				j0++;
			}
			for (int z = j0; z < int(cur_pts.size()) && cur_pts[z].first <= std::array<int, 2>{kx-1, ky+1}; z++) {
				check_point(z);
			}
			while (j1 < int(cur_pts.size()) && cur_pts[j1].first < std::array<int, 2>{kx, ky-1}) {
				j1++;
			}
			for (int z = j1; z < i; z++) {
				check_point(z);
			}
		}
		std::sort(cnds.begin(), cnds.end(), [&](const auto& a, const auto& b) -> bool {
			return a.first < b.first;
		});

		for (int i = 0, j = 0; i < int(cnds.size()); i = j) {
			while (j < int(cnds.size()) && cnds[j].first == cnds[i].first) {
				j++;
			}
			int64_t d = cnds[i].first;
			for (int z = i; z < j; z++) {
				auto [x, y] = cnds[z].second;
				if ((min_dist[x] == INF || min_dist[x] == d) && (min_dist[y] == INF || min_dist[y] == d)) {
					min_dist[x] = d;
					min_dist[y] = d;
					ans[x] = '1';
					ans[y] = '1';
				}
			}
		}

		{
			int l = 0;
			for (int i = 0; i < int(pts.size()); i++) {
				if (ans[pts[i].idx] == '0') {
					pts[l++] = pts[i];
				}
			}
			pts.resize(l);
		}
	}

	cout << ans << '\n';

	return 0;
}
