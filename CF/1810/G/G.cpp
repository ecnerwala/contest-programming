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

	using num = modnum<int(1e9)+7>;

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<num> P(N);
		for (auto& p : P) {
			num x, y; cin >> x >> y;
			p = x / y;
		}
		std::vector<num> H(N+1);
		for (auto& h : H) cin >> h;

		std::vector<num> W(N+1);
		for (int i = 0; i <= N; i++) {
			W[i] = H[i] - (i+1 <= N ? H[i+1] : 0);
		}

		const int V = 2*N+21;
		std::vector<num> dp(V, num(0));
		for (int i = 0; i <= N; i++) {
			dp[i] = W[i];
		}
		std::vector<num> ndp(V);
		for (int k = 1; k <= N; k++) {
			{
				// subtract one with probability P[i]
				num p = P[k-1];
				std::fill(ndp.begin(), ndp.end(), 0);
				for (int i = 0; i <= 2*N; i++) {
					if (i-1 >= 0) ndp[i-1] += dp[i] * p;
					if (i+1 < V) ndp[i+1] += dp[i] * (1 - p);
				}
				std::swap(dp, ndp);
			}
			num res = std::accumulate(dp.begin(), dp.end(), num(0));
			cout << res << " \n"[k==N];
		}
	}

	// we get w0 * p0 + w1 * (p0 + p1) + w2 * (p0 + p1 + p2) + ...
	// we want h0 * p0 + h1 * p1 + h2 * p2 + ...
	// so
	// w0 + w1 + w2 + ... = h0
	// w1 + w2 + w3 + ... = h1
	// w2 + w3 + w4 + ... = h2
	//
	// w0 = h0 + h1

	return 0;
}
