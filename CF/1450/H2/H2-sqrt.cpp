#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
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
		v = int(ll(v) * ll(o.v) % MOD);
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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	assert(N % 2 == 0);
	string S; cin >> S;
	for (int i = 1; i < N; i += 2) {
		if (S[i] == 'b' || S[i] == 'w') S[i] ^= 'b' ^ 'w';
	}

	vector<num> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
	vector<num> ifact(N+1);
	ifact[N] = inv(fact[N]);
	for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
	auto choose = [&](int n, int r) -> num {
		if (0 <= r && r <= n) return fact[n] * ifact[r] * ifact[n-r];
		else return 0;
	};
	vector<num> p2(N+1);
	p2[0] = 1;
	for (int i = 1; i <= N; i++) p2[i] = p2[i-1] + p2[i-1];

	int c0 = 0;
	int c1 = 0;
	int cb = 0;
	for (int i = 0; i < N; i++) {
		if (S[i] == 'b') c0++;
		else if (S[i] == 'w') c1++;
		else if (S[i] == '?') cb++;
		else assert(false);
	}

	vector<pair<int, int>> queries(Q+1);
	queries[0] = {cb, N/2-c0};

	for (int q = 1; q <= Q; q++) {
		int i; char c; cin >> i >> c; i--;
		if ((i & 1) && c != '?') c ^= 'b' ^ 'w';

		if (S[i] == 'b') c0--;
		else if (S[i] == 'w') c1--;
		else if (S[i] == '?') cb--;
		else assert(false);

		S[i] = c;

		if (S[i] == 'b') c0++;
		else if (S[i] == 'w') c1++;
		else if (S[i] == '?') cb++;
		else assert(false);

		queries[q] = {cb, N/2-c0};
	}
	Q++;

	auto get_ways = [&](int n, int r) -> num {
		assert(n >= 0);
		if (n == 0) return (r & 1) ? 0 : 1;
		else return p2[n-1];
	};
	auto get_tot = [&](int n, int r) {
		num tot = 0;
		for (int i = 0; i <= n; i++) {
			if ((i - r) & 1) continue;
			tot += choose(n, i) * num(abs(i-r)/2);
		}
		return tot;
	};
	auto solve_slow = [&](int n, int r) {
		return get_tot(n,r) / get_ways(n,r);
	};

	vector<num> ans(Q);

	const int BUCKET = 500;
	const int INF = 1e9;
	for (int lo = 0; lo < Q; lo += BUCKET) {
		int hi = min(Q, lo + BUCKET);

		int n_min = INF;
		int n_max = -1;
		int r_max = -INF;
		int nr_max = -INF;
		for (int q = lo; q < hi; q++) {
			auto [n, r] = queries[q];
			setmin(n_min, n);
			setmax(n_max, n);
			setmax(r_max, r);
			setmax(nr_max, n-r);
		}

		vector<vector<pair<int, int>>> to_solve(n_max - n_min + 1);
		for (int q = lo; q < hi; q++) {
			auto [n, r] = queries[q];
			to_solve[n - n_min].push_back({r, q});
		}

		vector<num> dp;
		for (int n = n_min; n <= n_max; n++) {
			int r_min = n - nr_max;
			assert(r_max >= r_min);

			if (n == n_min) {
				dp.resize(r_max - r_min + 1);
				for (int z = 0; z < 2 && z <= r_max - r_min; z++) {
					dp[z] = get_tot(n, r_min + z);

					num suff_sum = 0;
					for (int i = 0; i <= n; i++) {
						if (i < (r_min + z)) continue;
						if ((i - (r_min + z)) & 1) continue;
						suff_sum += choose(n, i);
					}

					for (int r = r_min + z + 2; r <= r_max; r += 2) {
						suff_sum -= choose(n, r - 2);

						dp[r - r_min] = dp[r - 2 - r_min];
						dp[r - r_min] += (n == 0 ? num((r & 1) ? 0 : 1) : p2[n-1]);
						dp[r - r_min] -= 2 * suff_sum;
					}
				}
			} else {
				// just add adjacent things to get closer
				for (int i = 0; i <= int(dp.size())-2; i++) {
					dp[i] += dp[i+1];
				}
				dp.pop_back();
			}

			assert(int(dp.size()) == r_max - r_min + 1);

			for (auto [r, q] : to_solve[n - n_min]) {
				ans[q] = dp[r - r_min] / get_ways(n, r);
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}

// Flip every other one, it's abs(# b - # w) / 4
//
// bbbwbbwb
