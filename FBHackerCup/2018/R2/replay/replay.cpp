#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

struct num {
	static const int MOD = int(1e9) + 7;
	int v;
	num() : v(0) {}
	num(ll v_) : v(int((v_ % MOD + MOD) % MOD)) {}
	explicit operator int () const {
		return v;
	}
	num& operator += (const num& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	num& operator -= (const num& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	num& operator *= (const num& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	num operator - () const {
		num res(*this);
		if (res.v) res.v = MOD-res.v;
		return res;
	}
};
num operator + (const num& a, const num& b) { return num(a) += b; }
num operator - (const num& a, const num& b) { return num(a) -= b; }
num operator * (const num& a, const num& b) { return num(a) *= b; }
bool operator == (const num& a, const num& b) { return a.v == b.v; }
bool operator != (const num& a, const num& b) { return a.v != b.v; }
std::ostream& operator << (std::ostream& out, const num &n) {
	return out << int(n);
}

const int MAXN = 60;
int N;
typedef pair<int, int> pii;
pair<int, int> pts[MAXN];
int Y[MAXN];

num dp[2][MAXN][MAXN][MAXN][MAXN]; // minup, maxdown, minabove, maxbelow
num go() {
	memset(dp, 0, sizeof(dp));
	dp[0][N+2][0][N+2][0] = 1;
	for (int i = 1; i < N; i++) {
		memset(dp[i&1], 0, sizeof(dp[i&1]));
		for (int minup = 0; minup <= N+2; minup ++) {
			for (int maxdown = 0; maxdown <= N+2; maxdown ++) {
				for (int minabove = 0; minabove <= N+2; minabove ++) {
					for (int maxbelow = 0; maxbelow <= N+2; maxbelow++) {
						num v = dp[!(i&1)][minup][maxdown][minabove][maxbelow];
						if (v == 0) continue;
						int y = Y[i];
						// insert the up only if maxbelow < y
						if (maxbelow < y) {
							dp[i&1][min(minup, y)][maxdown][minabove][maxbelow] += v;
						}
						// insert the down only if minabove > y
						if (minabove > y) {
							dp[i&1][minup][max(maxdown, y)][minabove][maxbelow] += v;
						}
						// insert the left depending
						if ((y > Y[0]) ? (maxdown < y) : (minup > y)) {
							dp[i&1][minup][maxdown][minabove][maxbelow] += v;
						}
						// insert the right
						if (y > Y[N]) {
							dp[i&1][minup][maxdown][min(minabove, y)][maxbelow] += v;
						} else {
							dp[i&1][minup][maxdown][minabove][max(maxbelow, y)] += v;
						}
					}
				}
			}
		}
	}
	num res = 1;
	for (int i = 1; i < N; i++) {
		res *= 4;
	}
	for (int minup = 0; minup <= N+2; minup ++) {
		for (int maxdown = 0; maxdown <= N+2; maxdown ++) {
			for (int minabove = 0; minabove <= N+2; minabove ++) {
				for (int maxbelow = 0; maxbelow <= N+2; maxbelow ++) {
					res -= dp[(N-1)&1][minup][maxdown][minabove][maxbelow];
				}
			}
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N; N ++;
		cin >> pts[0].second >> pts[N].second;
		pts[0].first = 0;
		pts[N].first = 1000000000;
		for (int i = 1; i < N; i++) {
			cin >> pts[i].first >> pts[i].second;
			assert(pts[0].first < pts[i].first && pts[i].first < pts[N].first);
		}
		sort(pts, pts + N);
		vector<int> ys;
		for (int i = 0; i <= N; i++) {
			ys.push_back(pts[i].second);
		}
		sort(ys.begin(), ys.end());
		assert(unique(ys.begin(), ys.end()) == ys.end());
		for (int i = 0; i <= N; i++) {
			Y[i] = int(lower_bound(ys.begin(), ys.end(), pts[i].second) - ys.begin()) + 1;
			assert(0 < Y[i] && Y[i] < N+2);
		}
		num res = go();
		cout << "Case #" << t << ": " << res << '\n';
	}

	return 0;
}
