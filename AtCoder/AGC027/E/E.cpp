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
using num = modnum<int(1e9)+7>;

const int MAXN = 2e5;
int N;
int A[MAXN];

int main() {
	string S;
	cin >> S;
	N = int(S.size());
	bool alternating = true;
	for (int i = 1; i < N; i++) {
		if (S[i] == S[i-1]) alternating = false;
	}
	if (alternating) {
		cout << 1 << '\n';
		return 0;
	}
	A[0] = 0;
	for (int i = 0; i < N; i++) {
		char c = S[size_t(i)];
		assert(c == 'a' || c == 'b');
		A[i+1] = A[i] + (c - 'a' + 1);
		A[i+1] %= 3;
	}

	num dp[3][3];
	memset(dp, 0, sizeof(dp));
	assert(A[0] == 0);
	dp[0][1] = dp[0][2] = 1;
	num res = 0;
	for (int i = 1; i <= N; i++) {
		int a = A[i];
		int x = (A[i] + 1) % 3, y = (A[i] + 2) % 3;

		num n = dp[x][a] + dp[y][a];
		dp[x][a] = 0;
		dp[y][a] = 0;
		if (a == A[N]) res += n;
		dp[a][x] += n;
		dp[a][y] += n;
	}

	cout << res << '\n';

	return 0;
}
