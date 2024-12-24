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

using num = modnum<int(1e9)+7>;

using std::cerr;
num slow(int64_t N, int64_t A) {
	int64_t rt = floor_sqrt(N);
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
	std::vector<num> f(N+1);
	for (int p : primes) {
		int q = p * p;
		for (int j = q; j <= N; j += q) f[j]++;
	}
	for (int i = 1; i <= N; i++) {
		f[i] = pow(f[i], A);
		f[i] += f[i-1];
	}
	std::vector<num> res;
	for (int64_t i = 1; ; i = N / (N / (i + 1))) {
		res.push_back(f[i]);
		if (i == N) break;
	}
	std::reverse(res.begin(), res.end());
	num ans = 0;
	for (auto v : res) {
		ans += v;
		ans *= 2024;
	}
	return ans;
}

num solve(int64_t N, int64_t A) {
	int64_t rt = floor_sqrt(N);
	int len = int(2 * rt + (rt * (rt+1) <= N));
	// len corresponds to N/0 = infty
	auto get_bucket = [&](int64_t a) -> int {
		return a <= rt ? int(a) : len - int(N/a);
	};

	std::vector<num> cnt(len);

	std::vector<int> primes; primes.reserve(rt+1);
	std::vector<int> pfac(rt+1);
	std::vector<int> num_fac(rt+1);
	std::vector<int> sqfree(rt+1);
	num_fac[1] = 0;
	sqfree[1] = true;
	for (int i = 2; i <= rt; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
			num_fac[i] = 1;
			sqfree[i] = true;
		}
		for (int p : primes) {
			if (int64_t(p) * int64_t(i) > rt) break;
			pfac[p * i] = p;
			num_fac[p * i] = num_fac[i] + 1;
			sqfree[p * i] = sqfree[i] && (p != pfac[i]);
			if (p == pfac[i]) break;
		}
	}
	for (int i = 1; i <= rt; i++) if (!sqfree[i]) num_fac[i] = -1;

	const int LG = *std::max_element(num_fac.begin(), num_fac.end());
	std::vector<num> fact(LG+1);
	fact[0] = 1;
	for (int i = 1; i <= LG; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(LG+1);
	ifact[LG] = inv(fact[LG]);
	for (int i = LG; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	std::vector<num> weight(LG+1);
	for (int i = 0; i <= LG; i++) {
		weight[i] = pow(num(i), A);
		for (int j = 0; j < i; j++) {
			weight[i] -= fact[i] * ifact[j] * ifact[i-j] * weight[j];
		}
		cerr << i << ' ' << weight[i] << '\n';
	}

	for (int p = 1; int64_t(p) * p * p <= N; p++) {
		if (num_fac[p] == -1) continue;
		num w = weight[num_fac[p]];
		if (w == 0) continue;
		for (int x = 1; x <= std::min(N / p / p / p, rt / p); x++) {
			cerr << x << ' ' << p << '\n';
			// y > std::max(p, x)
			int min_bucket = get_bucket(std::max(p, x));
			int max_bucket = len - get_bucket(int64_t(p) * p * x);
			assert(max_bucket >= min_bucket);

			// we'll do suffix sums
			cnt[max_bucket] += w;
			cnt[min_bucket] -= w;
			cerr << "increment " << min_bucket+1 << '-' << max_bucket << ' ' << w << '\n';

			int bucket_x = get_bucket(x);
			num val = num(N / (int64_t(p) * p * x) - std::max(p, x-1)) * w;
			cnt[bucket_x] += val;
			cnt[bucket_x-1] -= val;
			cerr << "increment " << bucket_x << ' ' << val << '\n';
		}
	}

	std::vector<num> pref_weight(len);
	for (int i = 1; i <= rt; i++) {
		if (num_fac[i] != -1) pref_weight[get_bucket(i * i)] += weight[num_fac[i]];
	}
	for (int i = 1; i < len; i++) pref_weight[i] += pref_weight[i-1];


	for (int x = 1; x * x <= rt; x++) {
		for (int y = x; int64_t(x) * y * y * y <= N; y++) {
			cerr << x << ' ' << y << '\n';
			// p >= max(x, y)
			int max_bucket = len - get_bucket(int64_t(x) * y);
			int min_bucket = get_bucket(int64_t(y) * y) - 1;
			assert(y <= rt);
			assert(max_bucket > min_bucket);
			num val = pref_weight[max_bucket] - pref_weight[min_bucket];
			cerr << min_bucket+1 << '-' << max_bucket << '\n';
			cnt[y] += val;
			cnt[y-1] -= val;
			cerr << "increment " << y << ' ' << val << '\n';
			if (x < y) {
				cnt[x] += val;
				cnt[x-1] -= val;
				cerr << "increment " << x << ' ' << val << '\n';
			}
		}
	}
	for (int a = len-1; a > 0; a--) {
		cnt[a-1] += cnt[a];
	}
	std::reverse(cnt.begin() + 1, cnt.begin() + len);
	for (int a = 1; a < len; a++) {
		cerr << cnt[a] << '\n';
	}
	num ans = 0;
	for (int a = len-1; a >= 1; a--) {
		ans += cnt[a];
		ans *= 2024;
	}
	return ans;
}


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, A; cin >> N >> A;
	cout << slow(N, A) << '\n';
	//cout << solve(N, A) << '\n';

	return 0;
}

// 40 / 4 + 40 / 9 + 40 / 25
