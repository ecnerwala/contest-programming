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
				cerr << "solve " << b << '\n';
				return true;
			}
		}
		return false;
	};
	try_solve_2(100);
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
