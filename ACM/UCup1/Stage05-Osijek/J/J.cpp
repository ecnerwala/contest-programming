#include <bits/stdc++.h>

std::vector<std::array<int, 2>> solve(int N) {
	if (N == 1) {
		return std::vector<std::array<int, 2>>({{0, 0}});
	}
	if (N == 2 || N == 3 || N == 4) return {};
	std::vector<std::array<int, 2>> r; r.reserve(N+1);
	int cur = N;
	while (cur >= 11) {
		cur -= 6;

		cur--;
		r.push_back({cur + 4, cur + 6});
		r.push_back({cur + 3, cur + 3});
		r.push_back({cur + 6, cur + 2});
		r.push_back({cur + 5, cur + 5});
		r.push_back({cur + 2, cur + 4});
		r.push_back({cur + 1, cur + 1});
		cur++;

	}
	if (cur >= 9) {
		cur -= 10;

		cur--;
		r.push_back({cur + 10, cur + 8});
		r.push_back({cur + 9, cur + 5});
		r.push_back({cur + 8, cur + 2});
		r.push_back({cur + 5, cur + 3});
		r.push_back({cur + 6, cur + 6});
		r.push_back({cur + 7, cur + 9});
		r.push_back({cur + 4, cur + 10});
		r.push_back({cur + 3, cur + 7});
		r.push_back({cur + 2, cur + 4});
		r.push_back({cur + 1, cur + 1});
		cur++;
	} else if (cur >= 7) {
		cur -= 8;

		cur--;
		r.push_back({cur + 8, cur + 6});
		r.push_back({cur + 7, cur + 3});
		r.push_back({cur + 4, cur + 2});
		r.push_back({cur + 5, cur + 5});
		r.push_back({cur + 6, cur + 8});
		r.push_back({cur + 3, cur + 7});
		r.push_back({cur + 2, cur + 4});
		r.push_back({cur + 1, cur + 1});

		cur++;
	} else if (cur >= 5) {
		cur -= 6;

		cur--;
		r.push_back({cur + 4, cur + 6});
		r.push_back({cur + 3, cur + 3});
		r.push_back({cur + 6, cur + 2});
		r.push_back({cur + 5, cur + 5});
		r.push_back({cur + 2, cur + 4});
		r.push_back({cur + 1, cur + 1});
		cur++;
	} else assert(false);
	if (cur == -1) {
		assert((r.back() == std::array<int, 2>({-1, -1})));
		r.pop_back();
		cur++;
	}
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (false) {
		for (int N = 1; N <= 200; N++) {
			auto r = solve(N);
			if (N == 2 || N == 3 || N == 4) {
				assert(r.empty());
			} else {
				assert(int(r.size()) == N);
				std::set<int> xs, ys;
				for (auto [x, y] : r) {
					assert(0 <= x && x < N);
					xs.insert(x);
					assert(0 <= y && y < N);
					ys.insert(y);
				}
				assert(int(xs.size()) == N);
				assert(int(ys.size()) == N);
				for (int i = 0; i+1 < N; i++) {
					int dx = abs(r[i+1][0] - r[i][0]);
					int dy = abs(r[i+1][1] - r[i][1]);
					assert(std::min(dx, dy) == 1 && std::max(dx, dy) == 3);
				}

			}
		}
	}

	int N; cin >> N;
	auto r = solve(N);
	if (int(r.size()) == N) {
		cout << "POSSIBLE" << '\n';
		for (int i = 0; i < N; i++) {
			cout << r[i][0]+1 << ' ' << r[i][1]+1 << '\n';
		}
	} else {
		cout << "IMPOSSIBLE" << '\n';
	}

	return 0;
}
