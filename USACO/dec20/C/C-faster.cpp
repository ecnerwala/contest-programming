#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
	static_assert(MOD > 0, "MOD must be positive");
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(std::int64_t v_) : v(int(v_ % MOD)) {}

	explicit operator int() const { return v; }

	friend std::ostream& operator << (std::ostream& o, const modnum& m) {
		return o << int(m);
	}
	friend std::istream& operator >> (std::istream& i, modnum& m) {
		std::int64_t val;
		i >> val;
		m = modnum(val);
		return i;
	}

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum& operator ++ () {
		++v;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum operator ++ (int) {
		modnum r = *this;
		++*this;
		return r;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		--v;
		return *this;
	}
	modnum operator -- (int) {
		modnum r = *this;
		--*this;
		return r;
	}

	modnum neg() const {
		modnum r = *this;
		if (v == 0) r.v = 0;
		else r.v = MOD - v;
		return r;
	}

private:
	static int minv(int a, int m) {
		if (a <= 1) return a;
		return m - int(std::int64_t(minv(m % a, a)) * m / a);
	}
public:

	modnum inv() const {
		assert(v);
		modnum r;
		r.v = minv(v, MOD);
		return r;
	}

	modnum& operator += (const modnum& o) {
		v -= MOD - o.v;
		v = (v < 0 ? v + MOD : v);
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0 ? v + MOD : v);
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(std::int64_t(v) * std::int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return (*this) *= o.inv();
	}
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_INTERVALS, K; cin >> N_INTERVALS >> K;
	K++; // make it exclusive

	vector<pair<int, int>> evts; evts.reserve(N_INTERVALS * 2);
	unordered_map<int, int> evt_map;
	// evts can be +- to increment/decrement coverage
	// evts can also be 0 to toggle on/off
	auto add_cross = [&](int x1, int b1, int x2, int b2) {
		if (b1 > b2) { swap(x1, x2), swap(b1, b2); }
		assert(b1 <= b2);
		int x = (x1 ^ x2) & (~((1<<b2)-1));
		evt_map[x] += (1 << b1);
		evt_map[x + (1 << b2)] -= (1 << b1);
	};
	auto process_range = [&](int lo, int hi) {
		int l = __builtin_ctz(hi-lo);
		for (int v = K; v & (v-1); v &= v-1) {
			add_cross(lo, l, v & (v-1), __builtin_ctz(v));
		}
	};

	// if the Bth bits are 000 or 111, then we can count these specially with choose 3
	// if the Bth bits are 011, then the 11 is already good

	assert(K >= 2);
	int B = 31 - __builtin_clz(K);
	assert(B >= 1);

	using num = modnum<int(1e9)+7>;
	num ans = 0;

	int last_bucket = -1;
	int last_cnt = 0;

	auto choose_3 = [&](int a) {
		return num(a) * num(a-1) * num(a-2) / num(6);
	};

	for (int i = 0; i < N_INTERVALS; i++) {
		int l, r; cin >> l >> r; r++; // make it half-open
		evts.push_back({l, 0});
		evts.push_back({r, 0});

		{
			int lb = l >> B, rb = r >> B;
			if (last_bucket != lb) last_bucket = lb, last_cnt = 0;
			if (lb == rb) {
				// fully inside
				ans -= choose_3(last_cnt);
				last_cnt += (r - l);
				ans += choose_3(last_cnt);
			} else {
				ans -= choose_3(last_cnt);
				last_cnt += ((lb+1) << B) - l;
				ans += choose_3(last_cnt);

				ans += choose_3(1 << B) * num(rb - lb - 1);

				last_bucket = rb, last_cnt = r - (rb << B);
				ans += choose_3(last_cnt);
			}
		}

		while (l < r) {
			if (r == 0 || (l != 0 && (l & -l) < (r & -r))) {
				process_range(l, l + (l & -l));
				l += l & -l;
			} else {
				process_range(r - (r & -r), r);
				r -= r & -r;
			}
		}
	}

	evts.reserve(evts.size() + evt_map.size());
	for (auto it : evt_map) {
		if (it.second == 0) continue;
		evts.push_back({it.first, it.second});
	}
	sort(evts.begin(), evts.end());

	num cur_cnt = 0;
	bool is_on = false;
	int prv = 0;

	for (auto it : evts) {
		int nxt = it.first;
		assert(nxt >= prv);
		if (is_on && prv != nxt) {
			ans += num(nxt - prv) * num(cur_cnt) * num(cur_cnt - 1) / num(2);
		}
		prv = nxt;

		int e = it.second;
		if (e == 0) {
			is_on ^= 1;
		} else {
			cur_cnt += num(e);
		}
	}

	cout << ans << '\n';

	return 0;
}
