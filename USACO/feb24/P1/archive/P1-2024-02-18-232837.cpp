#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MOD = int(1e9)+7;
	auto day_cost = [&](int64_t a) -> int {
		assert(a >= 0);
		if (a == 0) return 0;
		a--;
		int r = 1;
		int v = 3;
		while (a) {
			if (a & 1) r = int(int64_t(r) * int64_t(v) % MOD);
			a >>= 1;
			v = int(int64_t(v) * int64_t(v) % MOD);
		}
		return r;
	};

	int tot_cost = 0;
	std::map<int, int64_t> mp;
	auto get_cost = [&](auto a, auto b) -> int {
		int t = b->first - a->first;
		assert(t > 0);
		int64_t v = b->second - a->second;
		if (v < 0) return 0;
		assert(v >= 0);

		int64_t a_lo = v / t;
		int num_extra = int(v % t);
		if (a_lo == 0) {
			return num_extra % MOD;
		} else {
			return int(int64_t(day_cost(a_lo)) * int64_t(t + 2 * num_extra) % MOD);
		}
	};
	auto add_cost = [&](auto a, auto b) {
		tot_cost += get_cost(a, b);
		tot_cost -= (tot_cost >= MOD) ? MOD : 0;
	};
	auto sub_cost = [&](auto a, auto b) {
		tot_cost -= get_cost(a, b);
		tot_cost += (tot_cost < 0) ? MOD : 0;
	};

	mp.insert({0, 0});

	auto is_bad = [&](auto a, auto b, auto c) -> bool {
		int x1 = b->first - a->first;
		assert(x1 > 0);
		int64_t y1 = b->second - a->second;
		int x2 = c->first - b->first;
		assert(x2 > 0);
		int64_t y2 = c->second - b->second;
		return x1 * y2 >= x2 * y1;
	};

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int k; int64_t v; cin >> k >> v;
		{
			auto it = mp.find(k);
			if (it != mp.end() && it->first >= v) {
				goto done;
			}
			// Otherwise just erase the existing one
			assert(it != mp.begin());
			sub_cost(std::prev(it), it);
			if (std::next(it) != mp.end()) {
				sub_cost(it, std::next(it));
				add_cost(std::prev(it), std::next(it));
			}
			mp.erase(it);
		}

		{
			auto [it, did_insert] = mp.insert({k, v});
			assert(did_insert);
			assert(it != mp.begin());
			if (std::next(it) != mp.end()) {
				if (is_bad(std::prev(it), it, std::next(it))) {
					mp.erase(it);
					goto done;
				}
				sub_cost(std::prev(it), std::next(it));
			}
			// Now, clean previous and next
			while (it != mp.begin() && std::prev(it) != mp.begin() && is_bad(std::prev(std::prev(it)), std::prev(it), it)) {
				sub_cost(std::prev(std::prev(it)), std::prev(it));
				mp.erase(std::prev(it));
			}
			while (std::next(it) != mp.end() && std::next(std::next(it)) != mp.end() && is_bad(it, std::next(it), std::next(std::next(it)))) {
				sub_cost(std::next(it), std::next(std::next(it)));
				mp.erase(std::next(it));
			}
			assert(it != mp.begin());
			add_cost(std::prev(it), it);
			if (std::next(it) != mp.end()) {
				add_cost(it, std::next(it));
			}
		}
done:
		cout << tot_cost << '\n';
	}

	return 0;
}
