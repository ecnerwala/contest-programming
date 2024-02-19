#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
	int v;
	modnum() : v(0) {}
	explicit modnum(int v_) : v(v_ % MOD) {
		if (v < 0) v += MOD;
	}
	explicit operator int() const { return v; }
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator += (modnum o) {
		v -= MOD - o.v;
		v += (v < 0) ? MOD : 0;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v += (v < 0) ? MOD : 0;
		return *this;
	}
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }
	friend modnum operator * (modnum a, modnum b) { return a *= b; }
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	auto pow_3 = [&](int64_t a) -> num {
		assert(a >= 0);
		num r = num(1);
		num v = num(3);
		while (a) {
			if (a & 1) r *= v;
			a >>= 1;
			v *= v;
		}
		return r;
	};

	num tot_cost = num(0);
	std::map<int, int64_t> mp;
	auto get_cost = [&](auto a, auto b) -> num {
		int t = b->first - a->first;
		assert(t > 0);
		int64_t v = b->second - a->second;
		if (v < 0) return num(0);
		assert(v >= 0);

		int64_t a_lo = v / t;
		int num_extra = int(v % t);
		if (a_lo == 0) {
			return num(num_extra);
		} else {
			return pow_3(a_lo - 1) * num(t + 2 * num_extra);
		}
	};
	auto add_cost = [&](auto a, auto b) {
		tot_cost += get_cost(a, b);
	};
	auto sub_cost = [&](auto a, auto b) {
		tot_cost -= get_cost(a, b);
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
			if (it != mp.end()) {
				if (it->first >= v) {
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
