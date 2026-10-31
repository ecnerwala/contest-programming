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

using num = modnum<int(1e9)+7>;
using std::cerr;

num get_ways_slow(int A, int B, int C) {
	if (A < 0 || B < 0 || C < 0) return 0;
	std::vector<int> cur;
	for (int z = 0; z < A; z++) cur.push_back(+1);
	for (int z = 0; z < B; z++) cur.push_back(-1);
	for (int z = 0; z < C; z++) cur.push_back(0);
	std::sort(cur.begin(), cur.end());
	int ans = 0;
	do {
		bool good = true;
		for (int z = 0; z + 1 < int(cur.size()); z++) {
			if (cur[z] * cur[z+1] == 1) good = false;
		}
		ans += good;
	} while (std::next_permutation(cur.begin(), cur.end()));
	return ans;
}

num get_ways(int A, int B, int C) {
	if (A < 0 || B < 0 || C < 0) {
		return 0;
	}


	// what do we do?
	int N = A+B+C;
	std::vector<num> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(N+1);
	ifact[N] = inv(fact[N]);
	for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	auto choose = [&](int n, int r) -> num {
		if (0 <= r && r <= n) return fact[n] * ifact[r] * ifact[n-r];
		else return 0;
	};
	auto stars_and_bars = [&](int buckets, int objs) -> num {
		return choose(buckets + objs - 1, buckets - 1);
	};

	auto stars_and_bars_nonempty = [&](int buckets, int objs) -> num {
		return stars_and_bars(buckets, objs - buckets);
		// obs - 1 choose buckets - 1
	};

	if (A == 0 && B == 0) {
		return 1;
	} else if (A == 0 || B == 0) {
		return stars_and_bars(A+B+1, C - (A+B-1));
	}

	num ans = 0;
	assert(A > 0 && B > 0);
	for (int z = 0; z < 2; z++, std::swap(A, B)) {
		for (int runs = 2; runs <= A + B; runs ++) {
			int A_runs = (runs + 1) / 2;
			int B_runs = runs / 2;
			if (A_runs > A) break;
			if (B_runs > B) break;
			num ways = stars_and_bars_nonempty(A_runs, A) * stars_and_bars_nonempty(B_runs, A);
			ways *= stars_and_bars(A + B + 1, C - (A - A_runs) - (B - B_runs));
			ans += ways;
		}
	}

	return ans;
}

num solve(int A, int B, int C) {
	assert(A >= B);
	return get_ways(A, B, C) - get_ways(B-1, A, C) - get_ways(B-1, A+1, C-1) - get_ways(B-1, A, C-1);
}

num solve_slow(int A, int B, int C) {
	assert(A >= B);
	std::vector<int> cur;
	for (int z = 0; z < A; z++) cur.push_back(+1);
	for (int z = 0; z < B; z++) cur.push_back(-1);
	for (int z = 0; z < C; z++) cur.push_back(0);
	std::sort(cur.begin(), cur.end());
	int ans = 0;
	do {
		bool good = true;
		int bal = 0;
		for (int z = 0; z < int(cur.size()); z++) {
			if (z+1 < int(cur.size()) && cur[z] * cur[z+1] == 1) good = false;
			bal += cur[z];
			if (bal < 0) good = false;
		}
		ans += good;
	} while (std::next_permutation(cur.begin(), cur.end()));
	return ans;
}

void test() {
	for (int N = 2; N <= 10; N++) {
		cerr << "testing " << N << '\n';
		for (int A = 1; A + 1 <= N; A++) {
			for (int B = 1; B <= A && A + B <= N; B++) {
				int C = N - A - B;
				num ans = get_ways_slow(A, B, C);
				num mine = get_ways(A, B, C);
				if (mine != ans) {
					cerr << "bad get_ways" << '\n';
					cerr << A << ' ' << B << ' ' << C << '\n';
					cerr << "ans " << ans << '\n';
					cerr << "mine " << mine << '\n';
				}
			}
		}
	}
	return;
	for (int N = 2; N <= 10; N++) {
		cerr << "testing " << N << '\n';
		for (int A = 1; A + 1 <= N; A++) {
			for (int B = 1; B <= A && A + B <= N; B++) {
				int C = N - A - B;
				num ans = solve_slow(A, B, C);
				num mine = solve(A, B, C);
				if (mine != ans) {
					cerr << "bad" << '\n';
					cerr << A << ' ' << B << ' ' << C << '\n';
					cerr << "ans " << ans << '\n';
					cerr << "mine " << mine << '\n';
				}
			}
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();

	// sequences of x A's and y B's and z draws where no 2 x's in a row and no 2 y's in a row

	// If it crosses the line, it must cross with a B then a draw or a B then an A
	// Should we just casework on where it crosses? Maybe? Or maybe we can just use the reflection == BB thing?
	// # ways to get to (A, B) - (# ways to get to (B-1, A+1) + # ways to get to (B-2, A+1) - (# ways to get to (B-1, A+1) where the first cross is BA))
	// # ways to get to (A, B) - (# ways to get to (B-1, A+1) BB + # ways to get to (B-1, A+1) Btie)
	// # ways to get to (A, B) - (# ways to get to (B-1, A) B + # ways to get to (B-2, A) B + # (B-2, A-1) B)
	//
	// # (A, B, C) - # (B-1, A+1, C) BB - # (B-1, A+1, C) Btie
	// # (A, B, C) - # (B-1, A+1, C) BB - # (B-1, A+1, C) Btie(A/tie/$) - # (B-1, A+1, C) BtieB
	// # (A, B, C) - # (B-1, A, C) B - # (B-1, A+1, C-1) B - # (B-1, A, C-1) B
	
	int N, A, B; cin >> N >> A >> B;
	cout << solve(A, B, N - A - B) << '\n';

	return 0;
}
