#include <bits/stdc++.h>
using namespace std;

template <int MOD> class modnum {
private:
	int v;
public:
	using ll = long long;
	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int () const { return v; }
	operator bool() const { return bool(v); }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << n.v; }
	friend std::istream& operator >> (std::istream& in, modnum& n) { return in >> n.v; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

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
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
};
using num = modnum<int(1e9)+7>;

const int MAXN = 1.1e6;
int N;

const int MAXP = 4;
const int P = 3;
bool nxt[MAXN][MAXP+1];
bool prv[MAXN][MAXP+1];

// Valid states: (note that 3->3 can't be satisfied)
// 1-path states: 1->2, 2->1, 1->3, 3->1, 2->3, 3->2 (only on last iter)
// 2-path states: 1->1 + 2->3, 1->1 + 3->2, 2->2 + 1->3, 2->2 + 3->1
// Special start states:
// start state 1->1, which could lead to 1->1 + 2->2
// Special end states:
// () state
// end state transitions: 2 path immediately dies, 1 path wins if both possible
// that's 13 total states
enum state {
	S11 = 0,
	S11_22,
	S12,
	S21,
	S13,
	S31,
	S23,
	S32,
	S11_23,
	S11_32,
	S22_13,
	S22_31,
	S0,
	NUM_STATES
};

num dp[2][NUM_STATES];
num go() {
	if (N == 1) return 1;
	memset(dp, 0, sizeof(dp));
	dp[(N-1)&1][S11] = 1;
	for (int i = N-2; i >= 0; i--) {
		memset(dp[i&1], 0, sizeof(dp[i&1]));
		for (int s = 0; s < NUM_STATES; s++) {
			num cnt = dp[!(i&1)][s];
			if (!cnt) continue;
			auto& ndp = dp[i&1];
			switch(s) {
				case S11:
				if (nxt[i][1] && prv[i][1]) ndp[S0] += cnt;
				if (nxt[i][1]) ndp[S12] += cnt;
				if (prv[i][1]) ndp[S21] += cnt;
				ndp[S11_22] += cnt;
				break;
				case S11_22:
				// we have to connect to 2, otherwise we always lose
				if (nxt[i][2]) ndp[S22_13] += cnt;
				if (prv[i][2]) ndp[S22_31] += cnt;
				// we can't connect both to 2
				if (prv[i][1] && nxt[i][2]) ndp[S23] += cnt; // 1->1->0->2->2
				if (prv[i][2] && nxt[i][1]) ndp[S32] += cnt; // 2->2->0->1->1
				break;
				case S12:
				// connect to nothing
				ndp[S11_23] += cnt;
				// connect to both
				if (prv[i][2] && nxt[i][1]) ndp[S0] += cnt;
				// connect to only 1
				if (nxt[i][1]) ndp[S13] += cnt;
				// connect to only 2
				if (prv[i][2]) ndp[S21] += cnt;
				break;
				case S21:
				ndp[S11_32] += cnt;
				if (prv[i][1] && nxt[i][2]) ndp[S0] += cnt;
				if (nxt[i][2]) ndp[S12] += cnt;
				if (prv[i][1]) ndp[S31] += cnt;
				break;
				case S13:
				// must connect to 3
				if (prv[i][3]) ndp[S21] += cnt;
				if (prv[i][3] && nxt[i][1]) ndp[S0] += cnt;
				break;
				case S31:
				// must connect to 3
				if (nxt[i][3]) ndp[S12] += cnt;
				if (prv[i][1] && nxt[i][3]) ndp[S0] += cnt;
				break;
				case S23:
				// must connect to 3
				if (prv[i][3]) ndp[S31] += cnt;
				if (prv[i][3] && nxt[i][2]) ndp[S0] += cnt;
				break;
				case S32:
				// must connect to 3
				if (nxt[i][3]) ndp[S13] += cnt;
				if (prv[i][2] && nxt[i][3]) ndp[S0] += cnt;
				break;
				case S11_23:
				// must connect to 3
				if (prv[i][3]) ndp[S22_31] += cnt;
				if (prv[i][3] && nxt[i][1]) ndp[S32] += cnt;
				break;
				case S11_32:
				if (nxt[i][3]) ndp[S22_13] += cnt;
				if (prv[i][1] && nxt[i][3]) ndp[S23] += cnt;
				break;
				case S22_13:
				// must connect to 3 and 2
				if (prv[i][3] && nxt[i][2]) ndp[S23] += cnt;
				break;
				case S22_31:
				if (prv[i][2] && nxt[i][3]) ndp[S32] += cnt;
				break;
				case S0:
				// dead
				break;
				default: assert(false);
			};
		}
	}
	return dp[0][S0];
}

int main() {
	ios_base::sync_with_stdio(0);
	int K, inputP;
	cin >> N >> K >> inputP;
	for (int i = 0; i < N; i++) {
		for (int d = 1; d <= P; d++) {
			if (i+d >= N || d > inputP) {
				nxt[i][d] = prv[i][d] = false;
			} else {
				nxt[i][d] = prv[i][d] = true;
			}
		}
	}
	for (int k = 0; k < K; k++) {
		int a, b; cin >> a >> b; a--, b--;
		assert(a != b);
		if (abs(a-b) > inputP) continue;
		if (b > a) {
			nxt[a][b-a] = false;
		} else {
			prv[b][a-b] = false;
		}
	}

	num res = go();
	cout << res << '\n';
	return 0;
}
