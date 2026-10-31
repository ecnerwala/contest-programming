#include<bits/stdc++.h>
using namespace std;

template <int& MOD_> struct modnum {
	static constexpr int& MOD = MOD_;
	//static_assert(MOD_ > 0, "MOD must be positive");

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

const int MAXN = 5100;
int N, M;
using num = modnum<M>;

num dp[MAXN][MAXN];
num ndp[MAXN][MAXN];

num pref[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;

	int C = (N-1) / 2;

	int curL;
	int curRng;
	if ((N-1) % 2 == 0) {
		curL = 2;
		curRng = N / C;
		for (int i = 0; i <= curRng; i++) {
			dp[i][i] = 1;
		}
	} else {
		curL = 1;
		curRng = 0;
		dp[0][0] = 1;
	}

	while (curL < N-1) {
		int nxtL = curL + 2;
		int nxtRng = N / ((N + 1 - nxtL) / 2);

		assert(nxtRng >= curRng);

		for (int d = 0; d <= N; d++) {
			for (int i = 0; i <= nxtRng; i++) {
				ndp[d][i] = 0;
			}
		}
		for (int d = 0; d <= N; d++) {
			pref[0] = 0;
			for (int j = 0; j <= curRng; j++) {
				pref[j+1] = pref[j] + dp[d][j];
			}
			num v = 0;
			for (int i = 0; i <= nxtRng && i+d <= N; i++) {
				v += pref[min(i+1, curRng+1)];
				ndp[d+i][i] = v;
			}
		}

		for (int d = 0; d <= N; d++) {
			for (int i = 0; i <= nxtRng; i++) {
				dp[d][i] = ndp[d][i];
			}
		}
		curL = nxtL;
		curRng = nxtRng;
	}

	num ans = 0;
	for (int d = 0; d < N; d++) {
		pref[0] = 0;
		for (int j = 0; j <= curRng; j++) {
			pref[j+1] = pref[j] + dp[d][j];
		}
		int maxRng = (N - (d+1));
		num v = 0;
		for (int i = 0; i <= maxRng; i++) {
			v += pref[min(i+1, curRng + 1)];
			ans += v;
		}
	}
	cout << ans << '\n';

	return 0;
}
