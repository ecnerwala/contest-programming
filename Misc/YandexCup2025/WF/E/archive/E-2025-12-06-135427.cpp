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
	std::map<std::array<int64_t, 2>, int> mp;
	for (int i = 1; i <= 1000; i++) {
		std::array<int64_t, 2> val{fn(2, i), fn(4, i)};
		if (mp.count(val)) {
			cerr << mp[val] << ' ' << i << '\n';
		} else {
			mp[val] = i;
		}
	}

	return 0;
}
