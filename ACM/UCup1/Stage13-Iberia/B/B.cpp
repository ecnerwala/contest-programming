#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [](int N, auto query) -> std::vector<int> {
		std::vector<int> hull;
		hull.reserve(N);
		for (int i = 0; i < N; i++) {
			if (hull.size() < 2) {
				hull.push_back(i);
				continue;
			}
			int mi, ma;
			int r = query(hull[0], i, hull[hull.size() / 2]);
			if (r == 1) {
				mi = 0;
				ma = int(hull.size()) / 2;
			} else {
				mi = int(hull.size()) / 2;
				ma = int(hull.size());
			}

			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (query(hull[mi % int(hull.size())], i, hull[md % int(hull.size())]) == 1) {
					ma = md;
				} else if (query(hull[md % int(hull.size())], i, hull[ma % int(hull.size())]) == 1) {
					mi = md;
				} else {
					goto bad;
				}
			}
			assert(ma - mi == 1);
			std::rotate(hull.begin(), hull.begin() + ma, hull.end());
			while (hull.size() > 2 && query(hull.end()[-2], hull.end()[-1], i) == -1) {
				hull.pop_back();
			}
			while (hull.size() > 2 && query(i, hull.begin()[0], hull.begin()[1]) == -1) {
				hull.erase(hull.begin());
			}
			hull.push_back(i);
			continue;
bad:
			continue;
		}
		return hull;
	};

	std::mt19937_64 mt(48);
	auto test = [&](int N) -> void {
		std::vector<std::array<int, 2>> pts(N);
		auto dist = std::uniform_int_distribution<int>(1, int(1e9));
		for (auto& [x, y] : pts) {
			x = dist(mt);
			y = dist(mt);
		}
		int num_queries = 0;
		auto query = [&](int i, int j, int k) -> int {
			num_queries++;
			assert(i != j && j != k && k != i);
			int64_t v = int64_t(pts[j][0] - pts[i][0]) * int64_t(pts[k][1] - pts[i][1]) - int64_t(pts[k][0] - pts[i][0]) * int64_t(pts[j][1] - pts[i][1]);
			return (v > 0) - (v < 0);
		};
		auto r = solve(N, query);
		cerr << "solved in " << num_queries << " queries\n";
		assert(num_queries <= 30000);
	};
	for (int z = 0; z < 100; z++) {
		test(5000);
	}

	auto query = [&](int i, int j, int k) -> int {
		cout << '?' << ' ' << i+1 << ' ' << j+1 << ' ' << k+1 << '\n' << flush;
		int s; cin >> s;
		assert(s == 1 || s == -1);
		return s;
	};

	int N; cin >> N;
	auto res = solve(N, query);
	cout << '!' << ' ' << res.size();
	for (int i : res) { cout << ' ' << i+1; }
	cout << '\n' << flush;

	return 0;
}
