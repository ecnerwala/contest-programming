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
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
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

int main(){
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, D, R;
	cin >> N >> D >> R;

	using num = modnum<998244353>;

	const int V = N + D + 1;
	std::vector<num> fact(V+1);
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(V+1);
	ifact[V] = inv(fact[V]);
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	cerr << "done init" << '\n';

	num ans = 0;

	assert(R >= 1);

	vector<num> ncr_with_N(D+1, 0);
	for(int d = 0; d <= D; d++){
		ncr_with_N[d] = int(fact[N + D - d - 1] * ifact[D - d] * ifact[N - 1]);
	}
	cerr << "done with precomp" << '\n';
	std::vector<int> primes; primes.reserve(D+1);
	std::vector<int> pfac(D+1);
	for (int i = 2; i <= D; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(p) * int64_t(i) > D) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}
	for (int p : primes) {
		for (int i = D/p; i >= 1; i--) {
			ncr_with_N[i] += ncr_with_N[i*p];
		}
	}
	ans += num(N) * fact[N+D] * ifact[D] * ifact[N];
	for (int L = N; L > R; L--) {
		num F = 0;
		F = fact[L-2] * ifact[R-1] * ifact[L-R-1];
		F *= fact[N] * ifact[L] * ifact[N-L];
		if ((L + R) & 1) F = -F;

		num inside = ncr_with_N[0] + (L <= D ? ncr_with_N[L] : 0);
		ans += F * num(inside);
	}

	ans /= ncr_with_N[0];
	cout << ans << '\n';
	return 0;
}
