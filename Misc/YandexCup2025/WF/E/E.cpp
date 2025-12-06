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
	auto solve = [&](auto&& query, int K, int64_t C) -> int64_t {
		if (K == 4) {
			assert(C == int64_t(4e18));
			int64_t a = query(int(2e9) + 1);
			if (a != -1) {
				int64_t b = query(int(2e9) + 2);
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
			assert(false);
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
		return x.mod;
	};

	for (int x = 36; x <= 1000000; x++) {
		cerr << "test " << x << '\n';
		auto query = [&](int64_t b) -> int64_t {
			return fn(b, x);
		};
		int64_t res = solve(query, 3, int(2e9));
		assert(res == x);
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

	auto try_solve_2 = [&](int c) -> bool {
		for (int b = 2; b <= c; b++) {
			std::map<int64_t, std::vector<int>> mp;
			for (int x = 1; x <= c; x++) {
				mp[fn(b, x)].push_back(x);
			}
			bool good = true;
			for (const auto& [_, v] : mp) {
				bool found = false;
				for (int b2 = 2; b2 <= c; b2++) {
					std::set<int64_t> s;
					bool s_good = true;
					for (auto x : v) {
						auto [_, did_insert] = s.insert(fn(b2, x));
						if (!did_insert) { s_good = false; break; }
					}
					if (s_good) { found = true; break; }
				}
				if (!found) { good = false; break; }
			}
			if (good) {
				cerr << "solve " << b << ' ' << c << '\n';
				return true;
			}
		}
		return false;
	};
	{
		int a = 2;
		while (try_solve_2(a)) a++;
	}
	assert(!try_solve_2(36));
	// first query must be <= 36
	// 36^0 + ... + 36^5
	// there are 35 * 6 = 210 possible values
	const int Q1 = 36;
	const int K = int(2e9);
	const int CAP = 7600;
	std::map<int64_t, std::vector<int>> bucket;
	std::vector<std::vector<int64_t>> by_val(2*Q1-1);
	by_val[0].push_back(0);
	for (int s = 1; s <= 2*(Q1-1); s++) {
		for (int a = 1; a <= s && a < Q1; a++) {
			for (auto v : by_val[s-a]) {
				v *= Q1, v += a;
				if (v <= CAP) by_val[s].push_back(v);
			}
		}
		for (int z = 0; z < int(by_val[s].size()); z++) {
			int64_t v = by_val[s][z];
			v *= Q1;
			if (v <= CAP) {
				by_val[s].push_back(v);
			}
		}
		cerr << s << ' ' << int(by_val[s].size()) << '\n';
		std::sort(by_val[s].begin(), by_val[s].end());
		const auto& xs = by_val[s];
		auto try_solve_1 = [&](int64_t c) -> bool {
			if (c <= 1) return true;
			for (int b = 2; b <= c; b++) {
				bool dupe = false;
				std::set<int64_t> st;
				for (auto x : xs) {
					if (x < Q1) continue;
					if (x >= c) continue;
					auto v = fn(b, x);
					auto [_, did_insert] = st.insert(v);
					dupe |= !did_insert;
				}
				if (!dupe) {
					return true;
				}
			}
			return false;
		};
		for (int z = 0; z < int(by_val[s].size()); z++) {
			if (!try_solve_1(by_val[s][z])) {
				cerr << "cap at " << by_val[s][z] << '\n';
				break;
			}
		}
		[&]() -> bool {
			int c = 210;
			for (int b = 200; b <= c; b++) {
				std::map<int64_t, std::vector<int>> mp;
				for (int x : xs) {
					if (x < Q1) continue;
					mp[fn(b, x)].push_back(x);
				}
				bool good = true;
				for (const auto& [_, v] : mp) {
					bool found = false;
					for (int b2 = 2; b2 <= c; b2++) {
						std::set<int64_t> s;
						bool s_good = true;
						for (auto x : v) {
							auto [_, did_insert] = s.insert(fn(b2, x));
							if (!did_insert) { s_good = false; break; }
						}
						if (s_good) { found = true; break; }
					}
					if (!found) { good = false; break; }
				}
				if (good) {
					cerr << "solved with " << b << '\n';
					return true;
				}
			}
			cerr << "could not solve!!!" << '\n';
			return false;
		}();
	}
	exit(0);
	// 35 * 36 = 1260
	// 35 * 1261 * 1262
	for (auto [k, xs] : bucket) {
		cerr << "solving " << k << '\n';
		int c = 7601;
		[&]() -> bool {
			for (int b = 7600; b <= c; b++) {
				std::map<int64_t, std::vector<int>> mp;
				for (int x : xs) {
					mp[fn(b, x)].push_back(x);
				}
				bool good = true;
				for (const auto& [_, v] : mp) {
					bool found = false;
					for (int b2 = 2; b2 <= c; b2++) {
						std::set<int64_t> s;
						bool s_good = true;
						for (auto x : v) {
							auto [_, did_insert] = s.insert(fn(b2, x));
							if (!did_insert) { s_good = false; break; }
						}
						if (s_good) { found = true; break; }
					}
					if (!found) { good = false; break; }
				}
				if (good) {
					cerr << "solved with " << b << '\n';
					return true;
				}
			}
			cerr << "could not solve!!!" << '\n';
			return false;
		}();
	}
	// if we know s > 35 * 2, then x >= 36^2, so we can query 36^3 - O(1); if it's big enough, we query that +1 and CRT to win, otherwise we query 36^2 - O(1)
	// we only have to deal with s <= 70
	/*
	std::map<std::array<int64_t, 2>, int> mp;
	for (int i = 1; i <= 1000; i++) {
		std::array<int64_t, 2> val{fn(2, i), fn(3, i)};
		if (mp.count(val)) {
			cerr << mp[val] << ' ' << i << '\n';
		} else {
			mp[val] = i;
		}
	}
	*/

	return 0;
}
