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
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
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

const int MAXK = 1.1e5;
int K;
int A[MAXK];
int cnt[MAXK];

const int MAXV = 1.1e6;
const int V = int(1e6) + 100;
num fact[MAXV];
num ifact[MAXV];

void precomp() {
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	ifact[V] = fact[V].inv();
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
}

num choose(int n, int r) {
	if (0 <= r && r <= n) return fact[n] * ifact[r] * ifact[n - r];
	else return 0;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();

	cin >> K;
	for (int i = 0; i < K; i++) cin >> A[i];
	sort(A, A + K);

	bool canCycle = true;
	for (int i = 1, j = 0; i < K; i++) {
		while (j < K && A[j] < i) {
			j++;
		}
		if (j > i) {
			// this one is bad
			canCycle = false;
			break;
		}
	}

	if (canCycle) {
		for (int i = 0; i < K; i++) cnt[A[i] % K] ++;

		int extra = 0;
		for (int i = 0; i < K; i++) extra += A[i] / K;

		num ans = 0;
		for (int i = 0; i < K; i++) {
			// if we add i to everything
			extra += cnt[K - i]; // we get extras for each thing

			ans += choose(extra + K-1, K-1);

			// costs one to add 1 to everything
			extra --;
		}
		cout << ans << '\n';
	} else {
		num ans = 0;
		for (int i = 0, j = 0; true; i++) {
			assert(i < K);
			while (j < K && A[j] < i) {
				j++;
			}
			if (j > i) {
				break;
			}
			ans += choose((i - j) + K - 1, K - 1);
		}
		cout << ans << '\n';
	}

	return 0;
}
