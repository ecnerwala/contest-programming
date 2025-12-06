#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <typename T> struct mod_constraint {
	T v, mod;

	friend mod_constraint operator & (mod_constraint a, mod_constraint b) {
		if (a.mod < b.mod) std::swap(a, b);
		if (b.mod == 1) return a;

		extended_gcd_result<T> egcd = extended_gcd<T>(a.mod, b.mod);
		assert(a.v % egcd.gcd == b.v % egcd.gcd);

		T extra = b.v - a.v % b.mod;
		extra /= egcd.gcd;

		extra *= egcd.coeff_a;
		extra %= b.mod / egcd.gcd;
		extra += (extra < 0) ? b.mod / egcd.gcd : 0;

		return mod_constraint{
			a.v + extra * a.mod,
			a.mod * (b.mod / egcd.gcd)
		};
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto fn = [&](int64_t b, int64_t x) -> int64_t {
		assert(b >= 2);
		if (x >= b) {
			int64_t r = 0;
			for (; x; x /= b) r += x % b;
			return r;
		} else {
			return -1;
		}
	};
	std::map<int64_t, std::vector<int>> v4;
	for (int i = 1; i <= 35; i++) {
		v4[fn(4, i)].push_back(i);
	}
	std::map<int64_t, int> b4;
	std::map<int64_t, std::map<int64_t, int>> r4;
	for (auto [k, v] : v4) {
		for (int b = 2; true; b++) {
			std::map<int64_t, int> s;
			for (auto x : v) s[fn(b, x)] = x;
			if (int(s.size()) == int(v.size())) {
				b4[k] = b;
				r4[k] = s;
				break;
			}
		}
	}
	auto solve = [&](auto&& query, int K, int64_t C) -> int64_t {
		if (K == 4) {
			assert(C == int64_t(4e18));
			int64_t a = query(int(2e9) + 1);
			if (a != -1) {
				int64_t b = query(int(2e9) + 2);
				if (b == -1) {
					return int(2e9) + 1;
				}
				mod_constraint<int64_t> x{a % int(2e9), int(2e9)};
				mod_constraint<int64_t> y{b % (int(2e9)+1), int(2e9)+1};
				auto res = x & y;
				assert(res.mod > C);
				return res.v;
			}
		}
		// otherwise, we have 3 queries
		int64_t s1 = query(36);
		mod_constraint<int64_t> x{s1 % 35, 35};
		if (s1 == -1) {
			// 1..35
			int64_t s2 = query(4);
			return r4[s2][query(b4[s2])];
		}
		int64_t s2 = query(2485);
		if (s2 == -1) {
			// 36..2484
			int64_t s3 = query(72);
			if (s3 == -1) {
				// 36..71
				return 36 + (s1 - 1);
			}
			x = x & mod_constraint<int64_t>{s3 % 71, 71};
			assert(x.mod > 2484);
			return x.v;
		}
		x = x & mod_constraint<int64_t>{s2 % 2484, 2484};
		assert(x.mod == 86940);
		int64_t s3 = query(86940);
		if (s3 == -1) {
			return x.v;
		}
		x = x & mod_constraint<int64_t>{s3 % 86939, 86939};
		assert(x.mod > C);
		return x.v;
	};

	auto test = [&](int64_t x) {
		cerr << "test " << x << '\n';
		auto query = [&](int64_t b) -> int64_t {
			return fn(b, x);
		};
		int64_t res = solve(query, 4, int64_t(4e18));
		assert(res == x);
	};
	for (int64_t x = int64_t(2e9) - 200; x <= int64_t(2e9) + 200; x++) {
		test(x);
	}

	int T, K; int64_t C; cin >> T >> K >> C;
	while (T--) {
		auto query = [&](int64_t b) -> int64_t {
			cout << '?' << ' ' << b << '\n' << flush;
			int64_t a; cin >> a;
			if (a == -2) exit(0);
			return a;
		};
		int64_t ans = solve(query, K, C);
		cout << '!' << ' ' << ans << '\n' << flush;
		int resp; cin >> resp;
		if (resp == 0) exit(0);
		else if (resp == 1) continue;
		else assert(false);
	}

	return 0;
}
