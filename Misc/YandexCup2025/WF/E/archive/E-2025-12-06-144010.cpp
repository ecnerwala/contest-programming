#include <bits/stdc++.h>

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
	std::map<int64_t, std::vector<int>> bucket;
	std::vector<std::vector<int64_t>> by_val(2*Q1+1);
	by_val[0].push_back(0);
	for (int s = 1; s <= 2*Q1; s++) {
		for (int a = 1; a <= s && a < Q1; a++) {
			for (auto v : by_val[s-a]) {
				v *= Q1, v += a;
				if (v <= K) by_val[s].push_back(v);
			}
		}
		for (int z = 0; z < int(by_val[s].size()); z++) {
			int64_t v = by_val[s][z];
			v *= Q1;
			if (v <= K) {
				by_val[s].push_back(v);
			}
		}
		cerr << s << ' ' << int(by_val[s].size()) << '\n';
		const auto& xs = by_val[s];
		[&]() -> bool {
			int c = 1265;
			for (int b = 100; b <= c; b++) {
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
		int c = 1265;
		[&]() -> bool {
			for (int b = 1260; b <= c; b++) {
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
