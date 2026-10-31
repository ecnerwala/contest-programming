#include<bits/stdc++.h>
using namespace std;

template <typename T> T& setmax(T& a, const T& b) { if (b > a) a = b; return a; }
template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int M, N; cin >> M >> N;
		vector<pair<int, int>> P(N);
		for (int i = 0; i < N; i++) {
			cin >> P[i].first >> P[i].second;
		}
		sort(P.begin(), P.end());

		struct y_range { int lo, hi; };
		struct bound { int x; y_range y; };
		vector<bound> left_bounds; left_bounds.reserve(N+1);
		vector<bound> right_bounds; right_bounds.reserve(N+1);

		auto solve = [&](int qx, int qy) -> int {
			auto update_y_range = [&](y_range& yr, int py) {
				if (py <= qy) {
					setmax(yr.lo, py);
				}
				if (py >= qy) {
					setmin(yr.hi, py);
				}
			};

			auto range_inter_size = [&](y_range a, y_range b) -> int {
				return min(a.hi, b.hi) - max(a.lo, b.lo);
			};

			left_bounds.clear();
			left_bounds.push_back({0, {0, M}});
			for (int z = N-1; z >= 0; z--) {
				auto [px, py] = P[z];
				if (px > qx) continue;

				left_bounds.back().x = px;
				left_bounds.push_back({0, left_bounds.back().y});
				update_y_range(left_bounds.back().y, py);
			}

			right_bounds.clear();
			right_bounds.push_back({M, {0, M}});
			for (int z = 0; z < N; z++) {
				auto [px, py] = P[z];
				if (px < qx) continue;

				right_bounds.back().x = px;
				right_bounds.push_back({M, right_bounds.back().y});
				update_y_range(right_bounds.back().y, py);
			}

			int ans = 0;
			auto check_bounds = [&](bound lbound, bound rbound) -> bool {
				int dx = rbound.x - lbound.x;
				int dy = range_inter_size(lbound.y, rbound.y);
				if (rbound.x == qx || lbound.x == qx || lbound.y.lo == qy || lbound.y.hi == qy || rbound.y.lo == qy || rbound.y.hi == qy) {
					// don't update ans
				} else {
					ans = max(ans, min(dx, dy));
				}
				return dx > dy;
			};

			int l_ind = 0;
			int r_ind = int(right_bounds.size())-1;
			while (l_ind < int(left_bounds.size())) {
				while (r_ind >= 0 && check_bounds(left_bounds[l_ind], right_bounds[r_ind])) {
					r_ind--;
				}
				if (r_ind < int(right_bounds.size())-1) {
					r_ind++;
				}
				l_ind++;
			}
			return ans;
		};


		int Q; cin >> Q;
		while (Q--) {
			int qx, qy; cin >> qx >> qy;
			int ans = solve(qx, qy);
			if (ans <= 1) {
				ans = 0;
			}
			cout << int64_t(ans) * int64_t(ans) << '\n';
		}
	}

	return 0;
}
