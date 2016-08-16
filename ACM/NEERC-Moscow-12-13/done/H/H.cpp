#include<bits/stdc++.h>
using namespace std;

typedef double ld;

const int MAXN = 4020;

vector<ld> cnt[MAXN];
vector<ld> val[MAXN];
vector<int> expo[MAXN];
//ld dp[MAXN][MAXN];

int A[MAXN];

int par[MAXN];
int getpar(int a) { return par[a] == a ? a : (par[a] = getpar(par[a])); }
bool merge(int a, int b) { a = getpar(a), b = getpar(b); if(a == b) return false; par[b] = a; return true; }

int main() {
	ios_base::sync_with_stdio(0);

	for(int i = 0; i < MAXN; i++) {
		cnt[i].resize(MAXN - i);
		val[i].resize(MAXN - i);
		expo[i].resize(MAXN - i);
	}

	cnt[0][0] = 1;
	for(int p = 1; p < MAXN; p++) {
		expo[0][p] = expo[0][p - 1];
		cnt[0][p] = cnt[0][p - 1] * p;
		val[0][p] = val[0][p - 1] * p + cnt[0][p - 1];
		while(cnt[0][p] >= 2) {
			cnt[0][p] /= 2;
			val[0][p] /= 2;
			expo[0][p] ++;
		}
	}

	for(int n = 1; n < MAXN; n++) {
		if(n >= 2) {
			expo[n][0] = expo[n - 2][1];
			cnt[n][0] = cnt[n - 2][1] * (n - 1);
			val[n][0] = val[n - 2][1] * (n - 1);
			while(cnt[n][0] >= 2) {
				cnt[n][0] /= 2;
				val[n][0] /= 2;
				expo[n][0] ++;
			}
			//dp[n][0] = (val[n][0] / cnt[n][0]).get();
			//cerr << n << ' ' << dp[n][0] << '\n';
		}
		for(int p = 1; n + p < MAXN; p++) {
			expo[n][p] = max(expo[n - 1][p], expo[n][p - 1]);
			ld dn = 1, dp = 1;
			for(int i = expo[n - 1][p]; i < expo[n][p]; i++) dn *= 2;
			for(int i = expo[n][p - 1]; i < expo[n][p]; i++) dp *= 2;
			cnt[n][p] = cnt[n - 1][p] / dn * n + cnt[n][p - 1] / dp * p;
			val[n][p] = val[n - 1][p] / dn * n + val[n][p - 1] / dp * p + cnt[n][p - 1] / dp;
			while(cnt[n][p] >= 2) {
				cnt[n][p] /= 2;
				val[n][p] /= 2;
				expo[n][p] ++;
			}
			//dp[n][p] = (val[n][p] / cnt[n][p]).get();
			//cerr << n << ' ' << p << ' ' << dp[n][p] << '\n';
		}
	}
	cerr << "hi\n";
	int T; cin >> T;

	for(int t = 0; t < T; t++) {
		int N; cin >> N;
		int C; cin >> C;

		for(int i = 0; i < N; i++) {
			par[i] = i;
		}

		int numcyc = 0;
		int numpath = 0;
		int numblank = 0;
		for(int i = 0; i < N; i++) {
			cin >> A[i]; A[i] --;
			if(A[i] == -1) {
				numblank ++;
			} else if(!merge(i, A[i])) {
				numcyc ++;
			}
		}
		for(int i = 0; i < N; i++) {
			if(A[i] == -1 && par[i] != i) numpath++;
		}

		numblank -= numpath;

		ld res = val[numblank][numpath] / cnt[numblank][numpath];
		res += numcyc;
		res *= C;
		cout << fixed << setprecision(10) << res << '\n';
	}

	return 0;
}

// n = 0, p = 0
// cnt[0][0] = 1
// val[0][0] = 0
// n = 1, p = 0
// cnt[1][0] = 0
// val[1][0] = 0
//
// n >= 2
// cnt[n][0] = (n-1) cnt[n - 2][1]
// val[n][0] = (n-1) val[n - 2][1]
// p >= 1
// cnt[n][p] = n * cnt[n - 1][p] + p * cnt[n][p - 1]
// val[n][p] = n * val[n - 1][p] + p * val[n][p - 1] + cnt[n][p - 1]
