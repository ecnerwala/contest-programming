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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		using num = modnum<998244353>;
		int N; cin >> N;
		std::vector<num> A(N); for (auto& a : A) cin >> a;

		std::vector<num> fact(N+1); fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(N+1); ifact[N] = inv(fact[N]);
		for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		std::vector<num> pref(N+1);
		std::vector<num> pref_incr(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = pref[i] + A[i];
			pref_incr[i+1] = pref_incr[i+1] + A[i] * num(i);
		}

		num ans = 0;
		// l = 1
		for (int i = 0; i < N; i++) {
			ans += N * A[i];
		}
		for (int l = 2; l <= N; l++) {
			int extras = N - l;
			num mid_weight = 0;
			num edge_weight = 0;

			auto place_ways = [&](int bins, int balls) -> num {
				if (balls < 0) return 0;
				assert(bins >= 1);
				return fact[bins - 1 + balls] * ifact[balls] * ifact[bins - 1];
			};
			auto place_weight = [&](int bins, int balls) -> num {
				if (balls <= 0) return 0;
				assert(bins >= 1);
				return fact[bins - 1 + balls] * ifact[balls - 1] * ifact[bins];
			};

			if (extras & 1) {
				mid_weight = 2 * place_ways(l, (extras-1) / 2) + 2 * place_weight(l, (extras - 1) / 2);
				edge_weight = mid_weight + place_ways(l, (extras - 1) / 2);
			} else {
				mid_weight = place_ways(l, extras / 2) + place_ways(l, extras / 2 - 1) + place_weight(l, extras / 2) + place_weight(l, extras / 2 - 1);
				edge_weight = mid_weight + place_ways(l, extras / 2 - 1);
			}

			cerr << N << ' ' << l << ' ' << mid_weight << ' ' << edge_weight << '\n';

			ans += edge_weight * (pref[N-l+1] - pref[0]);
			ans += edge_weight * (pref[N] - pref[l-1]);
			auto get_incr_weight = [&](int cap) -> num {
				return pref_incr[cap] + cap * (pref[N - cap] - pref[cap]) + (N-1) * (pref[N] - pref[N-cap]) - (pref_incr[N] - pref_incr[N-cap]);
			};
			ans += mid_weight * get_incr_weight(std::min(l-2, N-l+1));
		}
		cout << ans << '\n';
	}

	return 0;
}
