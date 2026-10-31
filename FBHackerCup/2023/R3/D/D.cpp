#include<bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cerr << "Case #" << case_num << ": " << N << '\n';
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) {
			cin >> P[i]; P[i]--;
		}

		std::vector<std::vector<int>> ch(N);
		for (int i = 1; i < N; i++) {
			ch[P[i]].push_back(i);
		}

		std::vector<int> up_height(N);
		std::y_combinator([&](auto self, int cur) -> int {
			int v = 0;
			for (int nxt : ch[cur]) {
				v = std::max(v, self(nxt));
			}
			return up_height[cur] = v + 1;
		})(0);
		std::vector<int> down_height(N);
		std::y_combinator([&](auto self, int cur, int v_par) -> void {
			int v1 = v_par;
			int v2 = 0;
			for (int nxt : ch[cur]) {
				if (up_height[nxt] > v1) {
					v2 = v1;
					v1 = up_height[nxt];
				} else if (up_height[nxt] > v2) {
					v2 = up_height[nxt];
				}
			}
			for (int nxt : ch[cur]) {
				self(nxt, down_height[nxt] = (v1 == up_height[nxt] ? v2 : v1) + 1);
			}
			//cerr << "vert " << cur << ' ' << down_height[cur] << '\n';
		})(0, 0);

		std::vector<std::vector<std::pair<int, int>>> vals(N);
		for (int i = 0; i < N; i++) {
			vals[i].reserve(int(P[i] != -1) + int(ch[i].size()));
			if (P[i] != -1) {
				assert(down_height[i] > 0);
				vals[i].push_back({down_height[i], 1});
			}
			for (int nxt : ch[i]) {
				assert(up_height[i] > 0);
				vals[i].push_back({up_height[nxt], 1});
			}
			std::sort(vals[i].begin(), vals[i].end(), [&](auto a, auto b) { return a.first > b.first; });
			int l = 0;
			int cnt = 0;
			for (int z = 0; z < int(vals[i].size()); z++) {
				cnt += vals[i][z].second;
				assert(cnt == z+1);
				if (l == 0 || vals[i][z].first != vals[i][l - 1].first) {
					vals[i][l++] = vals[i][z];
				}
				vals[i][l-1].second = cnt;
			}
			vals[i].resize(l);
			/*
			int l = 0;
			for (int z = 0; z < int(vals[i].size()); z++) {
				if (l == 0 || vals[i][z].first != vals[i][l - 1].first) {
					vals[i][l++] = vals[i][z];
				} else {
					vals[i][l - 1].second += vals[i][z].second;
				}
			}
			vals[i].resize(l);
			// make them cumulative
			for (int z = 0; z+1 < l; z++) {
				vals[i][z+1].second += vals[i][z].second;
			}
			*/

			/*
			cerr << "vertex " << i << '\n';
			for (auto [v, c] : vals[i]) {
				cerr << v << ' ' << c << '\n';
			}
			*/
		}

		int64_t ans = 0;
		for (int i = 1; i < N; i++) {
			// check i against P[i]
			int j = P[i];
			int cur_x = 1;
			int cur_val = N+1;
			for (int a = 0, b = 0; a < int(vals[i].size()) && b < int(vals[j].size()); ) {
				cur_val = std::min(cur_val, vals[i][a].first);
				cur_val = std::min(cur_val, vals[j][b].first);
				if (vals[i][a].second <= vals[j][b].second) {
					assert(vals[i][a].second >= cur_x);
					ans += int64_t(vals[i][a].second - cur_x) * int64_t(cur_val);
					cur_x = vals[i][a].second;
					a++;
				} else {
					assert(vals[j][b].second >= cur_x);
					ans += int64_t(vals[j][b].second - cur_x) * int64_t(cur_val);
					cur_x = vals[j][b].second;
					b++;
				}
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
