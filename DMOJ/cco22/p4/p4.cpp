#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> roots(K);
	for (auto& r : roots) { cin >> r; r--; }

	std::vector<int> cnts(N);
	for (int r : roots) cnts[r]++;

	std::vector<std::array<int, 2>> ch(N);
	for (auto& r : ch) { for (auto& v : r) { cin >> v; v--; } }

	std::vector<int> par(N, -1);
	std::vector<int> dir(N);
	for (int i = 0; i < N; i++) {
		for (int z = 0; z < 2; z++) {
			if (ch[i][z] == -1) continue;
			par[ch[i][z]] = i;
			dir[ch[i][z]] = z;
		}
	}

	std::vector<int> sz(N);
	std::vector<int> extra_cap(N);
	std::y_combinator([&](auto&& self, int i) -> void {
		sz[i] = 1;
		extra_cap[i] = 1 - cnts[i];
		for (int z = 0; z < 2; z++) {
			if (ch[i][z] == -1) continue;
			self(ch[i][z]);
			sz[i] += sz[ch[i][z]];
			extra_cap[i] += extra_cap[ch[i][z]];
		}
	})(0);

	for (int i = 0; i < N; i++) {
		if (extra_cap[i] < 0) {
			cout << 0 << '\n';
			exit(0);
		}
	}
	assert(K <= N);

	using num = modnum<int(1e9)+7>;


	std::vector<num> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(N+1);
	ifact[N] = inv(fact[N]);
	for (int i = N; i > 0; i--) ifact[i-1] = ifact[i] * num(i);

	cout << std::y_combinator([&](auto&& self, int cur) -> std::vector<num> {
		if (cur == -1) {
			return std::vector<num>({num(1)});
		}
		auto pref_ways = self(ch[cur][dir[cur]]);
		auto oth_ways = self(ch[cur][!dir[cur]]);

		int max_pref_cnt = int(pref_ways.size()) - 1;
		int max_oth_cnt = int(oth_ways.size()) - 1;

		int own_cnt = cnts[cur];

		std::vector<num> res(extra_cap[cur] + 1);
		for (int c = 0; c <= extra_cap[cur]; c++) {
			if (c == extra_cap[cur]) max_oth_cnt++;
			for (int o_pref = std::max(0, own_cnt - max_oth_cnt), o_oth = std::min(own_cnt, max_oth_cnt); o_pref <= max_pref_cnt && o_oth >= 0; o_pref++, o_oth--) {
				int c_pref = std::min(c, max_pref_cnt - o_pref);
				int c_oth = c - c_pref;
				num ways = fact[own_cnt];
				ways *= pref_ways[o_pref + c_pref] * fact[o_pref + c_pref] * ifact[o_pref] * ifact[c_pref];
				ways *= (c == extra_cap[cur] ? oth_ways[o_oth + c_oth - 1] : oth_ways[o_oth + c_oth]) * fact[o_oth + c_oth] * ifact[o_oth] * ifact[c_oth];
				res[c] += ways;
			}
		}
		return res;
	})(0)[0] << '\n';

	return 0;
}
