#include<bits/extc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	using num = modnum<998244353>;

	int N, M; cin >> N >> M;

	std::vector<int> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * i;

	const int V = N * (N-1) / 2;
	auto to_idx = [&](int a, int b) { /*assert(0 <= a && a < b && b < N);*/ return a + (b-1) * b / 2; };
	auto p2 = [](int a) -> uint64_t { return uint64_t(1) << a; };

	vector<uint64_t> pred(V);

	for (int e = 0; e < M; e++) {
		int a, b, c, d; cin >> a >> b >> c >> d; a--, b--, c--, d--;
		int s = to_idx(a, b);
		int t = to_idx(c, d);
		pred[t] |= p2(s);
	}

	std::vector<num> dp(fact[N]);
	dp[0] = num(1);
	std::vector<uint64_t> msk(fact[N]);

	std::vector<int> perm(N); iota(perm.begin(), perm.end(), 0);
	for (int idx = 0; idx < fact[N]-1; idx++, next_permutation(perm.begin(), perm.end())) {
		if (idx && !msk[idx]) continue;
		for (int i = 1; i < N; i++) {
			if (perm[i-1] > perm[i]) continue;
			int a = to_idx(perm[i-1], perm[i]);
			if ((msk[idx] & pred[a]) != pred[a]) continue;
			// idx from swapping these
			int nidx = idx + fact[N-i] + idx / fact[N-i-1] % (N-i+1) * (fact[N-i] - fact[N-i-1]);
			msk[nidx] = msk[idx] | p2(a);
			dp[nidx] += dp[idx];
		}
	}
	cout << dp.back() << '\n';
}
