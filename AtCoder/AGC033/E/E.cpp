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
using num = modnum<int(1e9)+7>;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	string S; cin >> S;
	if (S[0] == 'B') {
		for (char& c : S) {
			c ^= 'R' ^ 'B';
		}
	}
	assert(S[0] == 'R');
	bool hasBlue = false;
	for (char c : S) {
		if (c == 'B') {
			hasBlue = true;
		}
	}
	assert(N >= 2);
	if (!hasBlue) {
		// 1 is red, 0 is blue
		num dp[2][2] = {{1, 0}, {0, 1}};
		for (int i = 1; i < N; i++) {
			num ndp[2][2] = {{0,0}, {0,0}};
			for (int a = 0; a < 2; a++) {
				for (int b = 0; b < 2; b++) {
					for (int c = 0; c < 2; c++) {
						if (b || c) {
							ndp[a][c] += dp[a][b];
						}
					}
				}
			}
			memcpy(dp, ndp, sizeof(ndp));
		}
		num ans = 0;
		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				if (a || b) {
					ans += dp[a][b];
				}
			}
		}
		cout << ans << '\n';
		exit(0);
	}

	assert(hasBlue);
	hasBlue = false;
	int maxRed = N-1;
	int cntRed = 0;
	for (char c : S) {
		if (c == 'B') {
			if (hasBlue) {
				if (cntRed % 2 == 1) {
					maxRed = min(maxRed, cntRed);
				}
			} else {
				maxRed = min(maxRed, cntRed + 1);
			}
			hasBlue = true;
			cntRed = 0;
		} else if (c == 'R') {
			cntRed++;
		} else assert(false);
	}

	assert(maxRed >= 1);

	if (maxRed % 2 == 0) maxRed --;

	assert(maxRed % 2 == 1);

	//cerr << maxRed << '\n';

	if (N % 2 == 1) {
		cout << 0 << '\n';
		exit(0);
	}

	assert(N % 2 == 0);
	N /= 2;
	maxRed ++;
	maxRed /= 2;
	assert(maxRed <= N);

	vector<num> dp;
	dp.push_back(1);

	num sum = 0;
	for (int i = 0; i < N; i++) {
		sum += dp[i];
		if (i >= maxRed) sum -= dp[i - maxRed];
		dp.push_back(sum);
	}

	num ans = 0;
	for (int i = 1; i <= maxRed; i++) {
		ans += num(2 * i) * dp[N - i];
	}
	cout << ans << '\n';

	return 0;
}
