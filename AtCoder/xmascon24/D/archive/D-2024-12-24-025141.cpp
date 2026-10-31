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

constexpr int64_t floor_sqrt(int64_t N) {
	assert(N >= 0);
	if (N == 0) return 0;
	int64_t a = N;
	while (true) {
		int64_t b = N/a;
		assert(a >= b);
		if (a-b <= 1) return b;
		a = (a+b+1)>>1;
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, A; cin >> N >> A;
	int64_t rt = floor_sqrt(N);
	int len = int(2 * rt + (rt * (rt+1) <= N));
	// len corresponds to N/0 = infty
	auto get_bucket = [&](int64_t a) -> int {
		return a <= rt ? int(a) : len - int(N/a);
	};

	using num = modnum<998244353>;

	std::vector<int64_t> cnt(len);

	std::vector<int> primes; primes.reserve(rt+1);
	std::vector<int> pfac(rt+1);
	for (int i = 2; i <= rt; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(p) * int64_t(i) > rt) break;
			pfac[p * i] = p;
			if (p == pfac[i]) break;
		}
	}
	std::vector<int> num_primes(rt+1);
	for (int i = 1; i <= rt; i++) num_primes[i] += num_primes[i-1] + (pfac[i] == i);

	for (int p : primes) {
		if (int64_t(p) * p * p >= N) continue;
		for (int x = 1; x <= std::min(N / p / p / p, rt / p); x++) {
			// y > std::max(p, x)
			int min_bucket = get_bucket(std::max(p, x));
			int max_bucket = len - get_bucket(int64_t(p) * p * x);
			assert(max_bucket >= min_bucket);

			// we'll do suffix sums
			cnt[max_bucket]++;
			cnt[min_bucket]--;

			int bucket_x = get_bucket(x);
			int64_t val = N / (int64_t(p) * p * x) - std::max(p, x) + (x > p);
			cnt[bucket_x] += val;
			cnt[bucket_x-1] -= val;
		}
	}

	for (int x = 1; x * x <= rt; x++) {
		for (int y = x; int64_t(x) * y * y * y <= N; y++) {
			// p >= max(x, y)
			int64_t val = num_primes[floor_sqrt(N / x / y)] - num_primes[std::max(x, y) - 1];
			cnt[y] += val;
			cnt[y-1] -= val;
			if (x < y) {
				cnt[x] += val;
				cnt[x-1] -= val;
			}
		}
	}
	for (int a = len-1; a > 0; a--) {
		cnt[a-1] += cnt[a];
	}
	for (int a = 1; a < len; a++) {
		cerr << cnt[a] << '\n';
	}

	return 0;
}
