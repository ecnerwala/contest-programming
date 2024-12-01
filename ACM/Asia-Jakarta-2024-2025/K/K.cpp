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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> freq(N+1);
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		freq[a]++;
	}

	std::vector<int> freq_mult(N+1);
	for (int i = 1; i <= N; i++) {
		for (int j = i; j <= N; j += i) {
			freq_mult[i] += freq[j];
		}
	}

	std::vector<int> mu(N+1);
	mu[1] = 1;
	std::vector<int> phi(N+1);
	std::iota(phi.begin(), phi.end(), 0);
	for (int i = 1; i <= N; i++) {
		for (int j = i; j <= N; j += i) {
			mu[j] -= mu[i];
			phi[j] -= phi[i];
		}
	}

	using num = modnum<998244353>;

	std::vector<num> p2(N+1);
	p2[0] = 1;
	for (int i = 1; i <= N; i++) p2[i] = p2[i-1] * 2;

	std::vector<num> p34(N+1);
	p34[0] = 1;
	for (int i = 1; i <= N; i++) p34[i] = p34[i-1] * num(3) / num(4);

	num ways_1_deck = 0;
	for (int i = 1; i <= N; i++) {
		ways_1_deck += phi[i] * p2[freq_mult[i]];
	}
	num ways_0_deck = num(N) * num(N+1) / num(2);

	num ways_2_deck = 0;

	for (int d = 1; d <= N; d++) {
		num cnt = 0;
		for (int a = 1; a * d <= N; a++) {
			cnt += mu[a] * p2[freq_mult[a * d]];
		}
		ways_2_deck *= num(d) * cnt * cnt;
	}

	for (int a = 1; a <= N; a++) {
		for (int b = 1; a * b <= N; b++) {
			if (std::gcd(a, b) != 1) continue;
			for (int g = 1; a * b * g <= N; g++) {
				for (int d = 1; a * b * g * d <= N; d++) {
					ways_2_deck -= num(d) * mu[a * g] * mu[b * g] * p2[freq_mult[a * g * d]] * p2[freq_mult[b * g * d]] * (1 - p34[freq_mult[a * b * g * d]]);
				}
			}
		}
	}

	cout << ways_2_deck - 2 * ways_1_deck + ways_0_deck << '\n';

	return 0;
}

// a' * g, b' * g
// sum_{d | g} d * mu[a' g / d] * mu[b' g / d] * 2^cnt(a' g) * 2^cnt(b' g) * (3/4)^cnt(a' b' g)
