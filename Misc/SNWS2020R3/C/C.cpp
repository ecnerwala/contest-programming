#include<bits/stdc++.h>
using namespace std;

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
using num = modnum<998244353>;

using ll = long long;

const int MAXK = 1e6;
bool isPrime[MAXK];
vector<int> primes;
unordered_map<ll, num> mem[MAXK];
num sumAntiSmooth(ll N, int i) {
	if (N == 0) return 0; // nothing works
	if (i == 0) return num(N) * num(N+1) / num(2); // only 1
	if (mem[i].count(N)) return mem[i][N];
	int p = primes[i-1];
	return mem[i][N] = sumAntiSmooth(N, i-1) - sumAntiSmooth(N/p, i-1) * num(p);
}

// (L, R]
num solve(ll L, ll R, ll K) {
	for (ll i = 2; i * i <= K; i++) {
		if (K % i == 0) {
			// K is not prime
			return 0;
		}
	}

	R /= K;
	L /= K;

	if (R < K) {
		if (L < 1 && 1 <= R) {
			return K;
		} else {
			return 0;
		}
	}

	assert(R >= K);

	for (int v = 2; v < K; v++) isPrime[v] = true;
	for (int p = 2; p < K; p++) {
		if (!isPrime[p]) continue;
		primes.push_back(p);
		for (int j = p+p; j < K; j += p) isPrime[j] = false;
	}

	return (sumAntiSmooth(R, int(primes.size())) - sumAntiSmooth(L, int(primes.size()))) * num(K);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	ll L, R, K; cin >> L >> R >> K; L--;
	cout << solve(L, R, K) << '\n';

	return 0;
}
