#include <bits/stdc++.h>

int64_t solve(std::vector<std::array<int, 3>> cows) {
	using pt_t = std::array<int, 2>;
	std::vector<pt_t> P; P.reserve(cows.size());
	for (auto [x, y, z] : cows) {
		int a = 2 * x - (y + z);
		int b = 2 * y - (z + x);
		int c = 2 * z - (x + y);
		assert(int64_t(a) + int64_t(b) + int64_t(c) == 0);
		if (a == 0 && b == 0) continue;
		int g = std::gcd(std::abs(a), std::abs(b));
		a /= g, b /= g;
		P.push_back({a, b});
		std::cerr << a << ' ' << b << '\n';
	}

	std::sort(P.begin(), P.end());
	std::vector<std::pair<pt_t, int>> dirs; dirs.reserve(P.size());
	for (int i = 0, j = 0; i < int(P.size()); i = j) {
		while (j < int(P.size()) && P[j] == P[i]) j++;
		dirs.push_back({P[i], j-i});
	}

	// returns -1, 0, +1, or -2 for exactly reflex
	auto get_dir = [&](pt_t a, pt_t b) -> int {
		int64_t r = int64_t(a[0]) * int64_t(b[1]) - int64_t(a[1]) * int64_t(b[0]);
		if (r) return (r > 0) - (r < 0);
		return int64_t(a[0]) * int64_t(b[0]) + int64_t(a[1]) * int64_t(b[1]) > 0 ? 0 : -2;
	};
	pt_t base_dir = {1, 0};
	auto mid = std::partition(dirs.begin(), dirs.end(), [&](auto a) -> bool {
		return get_dir(base_dir, a.first) < 0;
	});
	std::sort(dirs.begin(), mid, [&](auto a, auto b) -> bool { int d = get_dir(a.first, b.first); assert(d != -2); return d == 1; });
	std::sort(mid, dirs.end(), [&](auto a, auto b) -> bool { int d = get_dir(a.first, b.first); assert(d != -2); return d == 1; });

	// Count ways for 3 distinct directions
	int64_t tot = 0;
	{
		int64_t w1 = 0;
		int64_t w2 = 0;
		for (auto [_, cnt] : dirs) {
			tot += w2 * cnt;
			w2 += w1 * cnt;
			w1 += cnt;
		}
	}

	int N = int(dirs.size());
	if (!dirs.empty()) {
		int64_t c1 = 0;
		int64_t c2 = 0;
		for (int i = 0, j = 0; i < N; i++) {
			while (j == i || (j < N+i && get_dir(dirs[j%N].first, dirs[i].first) < 0)) {
				c2 += c1 * dirs[j%N].second;
				c1 += dirs[j%N].second;
				j++;
			}
			assert(j > i);
			c1 -= dirs[i].second;
			c2 -= c1 * dirs[i].second;
			tot -= c2 * dirs[i].second;
		}
	}

	return tot;
}

int64_t slow(std::vector<std::array<int, 3>> cows) {
	int N = int(cows.size());
	auto eval = [&](std::array<int, 3> a, std::array<int, 3> b) -> int {
		int64_t r = 0;
		r += int64_t(a[0]) * int64_t(b[1]) - int64_t(a[1]) * int64_t(b[0]);
		r += int64_t(a[1]) * int64_t(b[2]) - int64_t(a[2]) * int64_t(b[1]);
		r += int64_t(a[2]) * int64_t(b[0]) - int64_t(a[0]) * int64_t(b[2]);
		return (r > 0) - (r < 0);
	};
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			int tgt = eval(cows[i], cows[j]);
			if (tgt == 0) continue;
			for (int k = j+1; k < N; k++) {
				if (eval(cows[j], cows[k]) == tgt && eval(cows[k], cows[i]) == tgt) ans++;
			}
		}
	}
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (true) {
		std::mt19937 mt(48);
		for (int case_num = 0; case_num < 10000; case_num++) {
			int N = std::uniform_int_distribution(1, 4)(mt);
			std::vector<std::array<int, 3>> cows(N);
			for (auto& v : cows) {
				for (auto& c : v) {
					c = std::uniform_int_distribution(1, 10)(mt);
				}
				if (v[0] + v[1] + v[2] == 0) v[0]++;
			}
			cerr << "test " << N << '\n';
			int64_t a = solve(cows);
			int64_t b = slow(cows);
			if (a != b) {
				cerr << N << '\n';
				for (auto [x, y, z] : cows) {
					cerr << x << ' ' << y << ' ' << z << '\n';
				}
				cerr << "expected " << b << '\n';
				cerr << "got " << a << '\n';
				assert(false);
			}
		}
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 3>> pts(N);
		for (auto& [x, y, z] : pts) cin >> x >> y >> z;
		cout << solve(pts) << '\n';
	}

	return 0;
}
