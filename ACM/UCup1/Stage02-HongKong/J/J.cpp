#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<998244353>;

num solve(int N) {
	assert(1 <= N && N < num::MOD);

	std::array<int64_t, 30> cnt_set; cnt_set.fill(0);
	for (int v = N; v; v &= (v-1)) {
		int b = __builtin_ctz(v);
		for (int i = 0; i < b; i++) {
			cnt_set[i] += (1 << (b-1));
		}
		for (int x = v ^ (1 << b); x; x &= (x-1)) {
			cnt_set[__builtin_ctz(x)] += (1 << b);
		}
	}
	for (int i = 0; i < 30; i++) {
		cnt_set[i] <<= i;
	}
	std::array<int64_t, 31> prefs;
	prefs[0] = 0;
	for (int i = 0; i < 30; i++) prefs[i+1] = prefs[i] + cnt_set[i];

	int64_t pref_val = 0;

	std::vector<std::pair<int64_t, num>> dp;
	num ans = 0;

	for (int b = 29; b >= 0; b--) {
		// for each thing in dp, either + or -
		std::vector<std::pair<int64_t, num>> ndp;
		ndp.reserve(2 * dp.size());
		for (auto [v, cnt] : dp) {
			ndp.push_back({v + cnt_set[b], cnt});
			ndp.push_back({v - cnt_set[b], cnt});
		}

		if (N & (1 << b)) {
			ndp.push_back({pref_val + cnt_set[b], 1});
			pref_val -= cnt_set[b];
		} else {
			pref_val += cnt_set[b];
		}

		std::sort(ndp.begin(), ndp.end(), [&](auto x, auto y) { return x.first < y.first; });
		// coalesce
		int L = 0;
		for (int i = 0; i < int(ndp.size()); i++) {
			auto [v, cnt] = ndp[i];
			if (v >= prefs[b]) {
				// always good
				ans += num(v) * cnt * num(1 << b);
			} else if (v <= -prefs[b]) {
				// always bad
			} else if (false && L > 0 && v == ndp[L-1].first) {
				ndp[L-1].second += cnt;
			} else {
				ndp[L++] = {v, cnt};
			}
		}
		ndp.resize(L);
		dp = std::move(ndp);
	}
	assert(dp.empty());
	ans += num(N) * num(N-1) / num(2) * num(N);
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		cout << solve(N) / (num(N) * num(N)) << '\n';
	}

	return 0;
}
