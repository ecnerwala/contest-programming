#include <bits/stdc++.h>

int64_t go(std::vector<std::array<int, 2>> B) {
	int N = int(B.size());
	for (int i = 0; i < N; i++) {
		if (!(B[i][0] <= i && i <= B[i][1])) {
			return -1;
		}
	}
	for (int i = 0; i+1 < N; i++) {
		if (!(B[i][0] <= B[i+1][0])) {
			return -1;
		}
		if (!(B[i][1] <= B[i+1][1])) {
			return -1;
		}

		if ((B[i][1] == i) != (B[i+1][0] == i+1)) {
			return -1;
		}
	}

	std::vector<bool> is_st(N);
	std::vector<bool> is_en(N);

	std::vector<int> best_st(N+1);

	int64_t ans = 0;
	for (int lo = 0, hi; lo < N; lo = hi) {
		hi = lo+1;
		while (hi < N && !(B[hi-1][1] == hi-1 && hi == B[hi][0])) {
			hi++;
		}

		//std::cerr << lo << ' ' << hi << '\n';
		//for (int i = lo; i < hi; i++) { std::cerr << B[i][0] << ' ' << i << ' ' << B[i][1] << '\n'; } std::cerr << '\n';

		if (hi - lo == 1) continue;

		// Solve the range [lo, hi)
		// Each guy in this range is involved in at least 1 crossing (is not just {i})
		std::vector<std::array<int, 2>> l_paths; l_paths.reserve(hi-lo);
		std::vector<std::array<int, 2>> r_paths; r_paths.reserve(hi-lo);
		for (int i = lo; i < hi; i++) {
			if (i+1 == hi || B[i][0] < B[i+1][0]) {
				if (is_st[B[i][0]]) {
					return -1;
				}
				is_st[B[i][0]] = true;
				if (is_en[i]) {
					return -1;
				}
				is_en[i] = true;
				l_paths.push_back({B[i][0], i});
			}
			if (i == lo || B[i][1] > B[i-1][1]) {
				if (is_st[B[i][1]]) {
					return -1;
				}
				is_st[B[i][1]] = true;
				if (is_en[i]) {
					return -1;
				}
				is_en[i] = true;
				r_paths.push_back({i, B[i][1]});
			}
		}

		//for (auto [l, r] : l_paths) { std::cerr << l << ' ' << r << '\n'; } std::cerr << '\n';
		//for (auto [l, r] : r_paths) { std::cerr << l << ' ' << r << '\n'; } std::cerr << '\n';

		for (auto [l, r] : l_paths) ans += r - l;
		for (auto [l, r] : r_paths) ans += r - l;
		for (int st = 0, en = 0, idx = 0; idx < int(r_paths.size()); idx++) {
			while (en < int(l_paths.size()) && l_paths[en][0] < r_paths[idx][1]) en++;
			while (st < int(l_paths.size()) && l_paths[st][1] < r_paths[idx][0]) st++;
			ans -= en - st;
		}

		for (int z = 0; z < 2; z++, std::swap(l_paths, r_paths)) {
			// run the l_paths, and check the r_paths are valid
			for (int i = lo; i < hi; i++) best_st[i] = hi-1;
			for (auto it : r_paths) {
				best_st[it[1]] = it[0];
			}
			for (int i = hi-1; i > lo; i--) {
				best_st[i-1] = std::min(best_st[i-1], best_st[i]);
			}
			assert(best_st[lo] == lo);

			std::deque<std::pair<int, int>> q;
			for (int i = lo+1, l = 0, r = 0; i < hi; i++) {
				if (r < int(l_paths.size()) && l_paths[r][0] < i) {
					int v = l_paths[r][1] - r;
					while (!q.empty() && v <= q.back().first) q.pop_back();
					q.push_back({v, r});
					++r;
				}
				assert(r == int(l_paths.size()) || l_paths[r][0] >= i);
				while (!q.empty() && q.front().first + r - 1 < i) {
					l = q.front().second + 1;
					q.pop_front();
				}
				assert(l <= r);
				if (!(best_st[i] <= i - (r - l))) {
					return -1;
				}
			}
		}
	}

	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> B(N);
	for (auto& b : B) for (auto& v : b) { cin >> v; v--; }

	cout << go(B) << '\n';

	return 0;
}
