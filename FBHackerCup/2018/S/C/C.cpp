#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

template <typename T> T& setmin(T& a, T b) { if (b < a) a = b; return a; }

const int MAXN = 60;
int N;
ll T;
ll A[2][MAXN];
ll pref[2][MAXN];

ll dp[MAXN][MAXN];

ll cost(ll a) {
	return a * (T - a);
}

ll go() {
	cin >> N;
	T = 0;
	for (int z = 0; z < 2; z++) {
	//for (int z = 1; z >= 0; z--) {
		pref[z][0] = 0;
		for (int i = 0; i < N; i++) {
			cin >> A[z][i];
			T += A[z][i];
			pref[z][i+1] = pref[z][i] + A[z][i];
		}
	}
	assert(T == pref[0][N] + pref[1][N]);
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N; j++) {
			dp[i][j] = INF;
		}
	}
	dp[0][0] = 0;
	for (int i = 0; i <= N; i++) {
		dp[i][0] = dp[0][i] = 0;
	}
	// we can jump some amount ahead and place a vertical, or we can extend by 1
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N; j++) {
			const ll v = dp[i][j];
			//cerr << i  << ' ' << j << ' ' << v << '\n';

			if (i == j) continue;

			for (int k = min(i,j)+1; k <= N; k++) {
				int ni = i, nj = j;
				ll vertCost;
				if (i < j) {
					ni = k;
					if (ni <= j) {
						vertCost = pref[0][ni] - pref[0][i];
					} else {
						vertCost = pref[0][i] + pref[1][j];
					}
				} else {
					nj = k;
					if (nj <= i) {
						vertCost = pref[1][nj] - pref[1][j];
					} else {
						vertCost = pref[0][i] + pref[1][j];
					}
				}
				vertCost = cost(vertCost);

				ll nv = v + vertCost;
				if (i < j) {
					nv += cost(pref[0][i] + pref[1][i]);
				} else {
					nv += cost(pref[0][j] + pref[1][j]);
				}
				// we'll start at the latest possible point
				for (int a = min(i, j)+1; a < min(ni, nj); a++) {
					if (i < j) {
						nv += cost(pref[0][a] - pref[0][i]);
						nv += cost(pref[0][i] + pref[1][a]);
					} else {
						nv += cost(pref[1][a] - pref[1][j]);
						nv += cost(pref[1][j] + pref[0][a]);
					}
				}

				//cerr << i << ' ' << j << ' ' << ni << ' ' << nj << ' ' << nv << '\n';
				setmin(dp[ni][nj], nv);

				for (int a = min(ni, nj)-1; a > min(i, j); a--) {
					if (i < j) {
						nv -= 2 * (pref[0][a] - pref[0][i]) * (pref[0][i] + pref[1][a]);
					} else {
						nv -= 2 * (pref[1][a] - pref[1][j]) * (pref[1][j] + pref[0][a]);
					}
					if (ni < nj) {
						nv += 2 * (pref[0][ni] - pref[0][a]) * (T - pref[0][ni] - pref[1][a]);
					} else {
						nv += 2 * (pref[1][nj] - pref[1][a]) * (T - pref[1][nj] - pref[0][a]);
					}
					setmin(dp[ni][nj], nv);
				}
			}
		}
	}
	return dp[N][N];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N_CASES; cin >> N_CASES;
	for (int case_num = 1; case_num <= N_CASES; case_num ++) {
		auto res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
