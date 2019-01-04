#include<bits/stdc++.h>
using namespace std;

using ll = long long;
struct modnum {
	static const int MOD = int(1e9)+7;
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	friend std::ostream& operator << (std::ostream& o, const modnum& m) { return o << m.v; }

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

using num = modnum;

string S;
int L;
const int MAXL = 4e3;
num dp[MAXL];
num dp2[MAXL];
const int MAXH = 7e3;
num dp_h[MAXH];
num dp_h2[MAXH];

int finalHeight = -1;

// start, end, max, (min = 0)
void insertSeg(int st, int en, int ma) {
	for (int i = 0; i <= ma; i++) {
		dp_h[i] = 0;
	}
	dp_h[st] = 1;
	for (int l = 0; l <= L; l++) {
		dp2[l] = dp_h[en];
		dp_h2[ma] = 0;
		for (int i = 0; i < ma; i++) {
			dp_h2[i] = dp_h[i+1];
		}
		for (int i = 1; i <= ma; i++) {
			dp_h2[i] += dp_h[i-1];
		}
		for (int i = 0; i <= ma; i++) {
			dp_h[i] = dp_h2[i];
		}
	}

	for (int l = L; l >= 0; l--) {
		num v = 0;
		for (int k = 0; k <= l; k++) {
			v += dp[k] * dp2[l-k];
		}
		dp[l] = v;
	}
}

vector<pair<int, int>> edges;

void doSide() {
	int curStart = 0; // start of the climb
	int curEnd = 0; // end of the climb
	int curHeight = 0;
	for (char c : S) {
		if (c == '(') {
			curHeight++;
			if (curHeight > curEnd) {
				if (curEnd > curStart) {
					edges.emplace_back(curStart, curEnd);
				}
				curStart = curEnd = curHeight;
			}
		} else if (c == ')'){
			curHeight--;
			if (curHeight < 0) {
				curEnd++;
				curHeight++;
			}
		} else assert(false);
	}
	if (curEnd > curStart) {
		edges.emplace_back(curStart, curEnd);
		curStart = curEnd;
	}
	if (finalHeight != -1) {
		assert(finalHeight == curEnd);
	}
	finalHeight = curEnd;
}

num go() {
	doSide();
	reverse(S.begin(), S.end());
	for (char& c : S) {
		if (c == '(') c = ')';
		else if (c == ')') c = '(';
		else assert(false);
	}
	doSide();

	L -= int(edges.size());
	if (L < 0) return 0;

	dp[0] = 1;

	for (auto it : edges) {
		insertSeg(it.first, it.second-1, it.second-1);
	}
	insertSeg(finalHeight, finalHeight, finalHeight + L);
	return dp[L];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> S >> L;

	num res = go();
	cout << res << '\n';
	return 0;
}
