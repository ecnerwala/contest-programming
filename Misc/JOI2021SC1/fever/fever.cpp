#include <bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

template <typename T>
using min_priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> pts(N); for (auto& p : pts) cin >> p[0] >> p[1];

	{ // Recenter them
		auto v = pts[0];
		for (auto& p : pts) p[0] -= v[0], p[1] -= v[1];
	}
	assert(pts[0][0] == 0 && pts[0][1] == 0);

	int actual_ans = 0;

	for (int start_dir = 0; start_dir < 4; start_dir++) {
		// rotate by 90
		for (auto& p : pts) p = {-p[1], p[0]};

		const int INF = (1 << 30) - 2;
		array<hash_map<int, array<vector<tuple<int, int, int>>, 2>>, 4> lines;
		for (int i = 0; i < N; i++) {
			auto [x, y] = pts[i];
			lines[0][x+y][(x-y)>0].push_back({abs(x-y), i, INF});
			lines[1][x-y][(x+y)>0].push_back({abs(x+y), i, INF});
			if ((x > y) == (x > -y)) {
				lines[2][y][x>0].push_back({abs(x), i, INF});
			} else {
				lines[3][x][y>0].push_back({abs(y), i, INF});
			}
		}
		for (int d = 0; d < 4; d++) {
			for (auto& [_, l] : lines[d]) {
				sort(l[0].begin(), l[0].end());
				sort(l[1].begin(), l[1].end());
			}
		}
		min_priority_queue<pair<int, pair<vector<tuple<int, int, int>>*, int>>> pq;
		vector<int> dist(N, INF);
		pq.push({dist[0] = 0, {nullptr, 0}});

		int ans = 0;

		while (!pq.empty()) {
			int d = pq.top().first;
			auto [_l, z] = pq.top().second;
			pq.pop();
			if (_l == nullptr) {
				// process this as a point
				if (d != dist[z]) continue;
				ans++;

				auto check_line = [&](array<vector<tuple<int, int, int>>, 2>& line_pair, int val) -> void {
					auto& line = line_pair[!(val > 0)];
					val = -abs(val);
					int p = int(lower_bound(line.begin(), line.end(), tuple<int, int, int>{val+d, -1, -1}) - line.begin());
					if (p < int(line.size())) {
						int cnd = get<0>(line[p]) - val;
						assert(cnd >= d);
						if (cnd < get<2>(line[p])) {
							pq.push({get<2>(line[p]) = cnd, {&line, p}});
						}
					}
				};

				auto [x, y] = pts[z];
				check_line(lines[0][x+y], x-y);
				check_line(lines[1][x-y], x+y);
				if ((x>y) == (x>-y)) {
					check_line(lines[2][y], x);
				} else {
					check_line(lines[3][x], y);
				}
			} else {
				auto& line = *_l;
				if (d != get<2>(line[z])) continue;

				{
					int i = get<1>(line[z]);
					if (d < dist[i]) {
						pq.push({dist[i] = d, {nullptr, i}});
					}
				}
				if (z+1 < int(line.size())) {
					int cnd = d + get<0>(line[z+1]) - get<0>(line[z]);
					if (cnd < get<2>(line[z+1])) {
						pq.push({get<2>(line[z+1]) = cnd, {&line, z+1}});
					}
				}
			}
		}

		actual_ans = max(actual_ans, ans);
	}

	cout << actual_ans << '\n';

	return 0;
}
