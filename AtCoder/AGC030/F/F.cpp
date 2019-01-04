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

const int MAXN = 400;
int N;
int C[MAXN];
const int MAXM = MAXN * 2;
int M;

num binom[MAXM][MAXM];
num factorial[MAXM];

// number of sequences with i C requests and j other requests
num dp[2][MAXM][MAXM];
int z;

void goM(int i, int k) {
	//cerr << "goM " << i << ' ' << k << '\n';
	for (int a = 0; a <= (N-i); a++) {
		for (int b = 0; b <= (M-k-1); b++) {
			num v = dp[z][a][b];
			dp[z][a][b] = 0;
			dp[!z][a][b+1] += v;
			if (a) {
				dp[!z][a-1][b] += v * num(a);
			}
			// don't multiply by the ordering, because we always choose the biggest request
			if (b) {
				dp[!z][a][b-1] += v;
			}
		}
	}
	z = !z;
}

void goN(int i, int k) {
	//cerr << "goN " << i << ' ' << k << '\n';
	for (int a = 0; a <= (N-i-1); a++) {
		for (int b = 0; b <= (M-k); b++) {
			num v = dp[z][a][b];
			dp[z][a][b] = 0;

			dp[!z][a+1][b] += v;
			if (b) {
				dp[!z][a][b-1] += v;
			}
		}
	}
	z = !z;
}

num go() {
	for (int i = 0; i <= M; i++) {
		binom[i][0] = binom[i][i] = 1;
		for (int j = 1; j < i; j++) {
			binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
		}
	}
	factorial[0] = 1;
	for (int i = 1; i <= M; i++) {
		factorial[i] = factorial[i-1] * num(i);
	}

	C[N] = M;

	z = 0;
	dp[z][0][0] = 1;

	//cerr << N << ' ' << M << '\n';
	for (int i = N-1; i >= 0; i--) {
		for (int k = C[i+1]-1; k >= C[i]; k--) { // O(M) loop
			goM(i+1, k);
		}
		goN(i, C[i]);
	}
	for (int k = C[0]-1; k >= 0; k--) {
		goM(0, k);
	}
	num res = dp[z][0][0];
	assert((M - N) % 2 == 0);
	int numEmpty = (M-N)/2;
	return res * factorial[numEmpty];
}

pair<int, int> A[MAXN];
bool used[MAXM];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i].first >> A[i].second;

		if (A[i].first > 0) {
			A[i].first--;
			used[A[i].first] = true;
		} else {
			assert(A[i].first == -1);
		}

		if (A[i].second > 0) {
			A[i].second--;
			used[A[i].second] = true;
		} else {
			assert(A[i].second == -1);
		}

		if (A[i].first > A[i].second) swap(A[i].first, A[i].second);
		assert(A[i].first <= A[i].second);
	}
	sort(A, A + N);
	//for (int i = 0; i < N; i++) { cerr << A[i].first << ' ' << A[i].second << '\n'; } cerr << '\n';
	int numEmpty = 0;
	int numHalf = 0;
	for (int i = 0; i < N; i++) {
		assert(A[i].first <= A[i].second);
		if (A[i].second == -1) {
			numEmpty++;
		} else if (A[i].first == -1) {
			numHalf++;
			int numSmaller = 0;
			assert(used[A[i].second]);
			for (int v = 0; v < A[i].second; v++) {
				if (!used[v]) numSmaller++;
			}
			C[numHalf-1] = numSmaller;
		}
	}
	// no more A
	M = numEmpty * 2 + numHalf;
	N = numHalf;

	num res = go();
	cout << res << '\n';

	return 0;
}
